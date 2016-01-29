/**
	SPICE Core
	ConnectionHandler.cpp
	Purpose: Class to handle incoming connections

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "ConnectionHandler.h"

#include <sstream>

#include "CommandBase.h"
#include "MandatoryCommandBase.h"
#include "SiLASOAPFunctions.h"

#include "Abort.h"
#include "DoContinue.h"
#include "GetConfiguration.h"
#include "GetParameters.h"
#include "Initialize.h"
#include "LockDevice.h"
#include "Pause.h"
#include "Reset.h"
#include "SetConfiguration.h"
#include "SetParameters.h"
#include "UnlockDevice.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			ConnectionHandler::ConnectionHandler(std::shared_ptr<CommandHandler> commandHandler) :
				_commandHandler(commandHandler),
				_wsdlGenerator(std::shared_ptr<WSDLGenerator>(new WSDLGenerator()))
			{
			}
			ConnectionHandler::~ConnectionHandler()
			{
			}

			bool ConnectionHandler::handleEthernetConnection(std::string uriPath, std::string uriParameters,
				std::string serverAddress, std::string soapAction,
				std::string content, std::string& responseSOAPAction, std::string& responseContent,
				std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> ethernetEventConnectionFactory)
			{
				if(uriParameters == "wsdl")
				{
					handleWSDLConnection(serverAddress, responseContent);
				}
				else if(uriParameters == "wsdlnicelab") // TEMP-FIX
				{
					handleWSDLConnection(serverAddress, responseContent);
					responseContent = SPICE::BIG::SiLASOAPFunctions::dequoteXMLFile(responseContent);
				}
				else
				{
					std::string soapCommand = "";
					std::string soapNamespace = "";
					bool foundSOAPAction = false;

					std::string possibleNamespaces[2] = {"http://sila-standard.org/", "http://coop.sila/"};
					for(int i = 0; i < 2; i++)
					{
						if(soapAction.find(possibleNamespaces[i], 0) == 0 && soapAction.length() > possibleNamespaces[i].length())
						{
							soapCommand = soapAction.substr(possibleNamespaces[i].length());
							soapNamespace = possibleNamespaces[i].substr(0, possibleNamespaces[i].length() - 1);
							foundSOAPAction = true;
							i = 2;
						}
					}

					if(foundSOAPAction)
					{
						handleCommandConnection(serverAddress, soapCommand, soapNamespace, content, responseSOAPAction, responseContent, ethernetEventConnectionFactory);
					}
					else
					{
						return false;
					}

				}
				return true;
			}

			void ConnectionHandler::prepareWSDLFile()
			{
				std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> commands;

				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new Abort()));
				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new DoContinue()));
				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new GetConfiguration()));
				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new GetParameters()));
				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new LockDevice("")));
				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new Pause()));
				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new SetConfiguration()));
				commands.push_back(std::shared_ptr<SPICE::BIG::CommandBase>(new UnlockDevice()));

				std::shared_ptr<SetParameters> setParametersCommand(new SetParameters());
				setParametersCommand->setSpecificSetParameters(_commandHandler->getCoreData()->getResourceProvider()->getSpecificSetParameters());
				commands.push_back(setParametersCommand);
				std::shared_ptr<Reset> resetCommand(new Reset());
				resetCommand->setSpecificReset(_commandHandler->getCoreData()->getResourceProvider()->getSpecificReset());
				commands.push_back(resetCommand);
				std::shared_ptr<Initialize> initCommand(new Initialize());
				initCommand->setSpecificInit(_commandHandler->getCoreData()->getResourceProvider()->getSpecificInit());
				commands.push_back(initCommand);

				std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> specificCommands = _commandHandler->getCoreData()->getResourceProvider()->getInstanceOfAllCommands();

				for(unsigned int i = 0; i < specificCommands.size(); i++)
				{
					commands.push_back(specificCommands.at(i));
				}

				_wsdlGenerator->generateWSDLFile(commands);
			}


			void ConnectionHandler::handleCommandConnection(std::string serverAddress, std::string silaCommand, std::string silaNamespace,
				std::string content, std::string& responseSOAPAction, std::string& responseContent,
				std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> ethernetEventConnectionFactory)
			{
				std::shared_ptr<SPICE::BIG::IXMLParser> xmlParser = _commandHandler->getCoreData()->getResourceProvider()->getNewXMLParserInstance();

				std::string silaNSPrefix = _commandHandler->getCoreData()->getResourceProvider()->getCoreConfigurationParameter("SILA_NS_PREFIX");

				if(xmlParser->loadXMLStructure(content))
				{
					if(xmlParser->selectElementByNameAndNS(silaCommand, silaNamespace))
					{
						std::vector<std::string> childs = xmlParser->getChildElementsList();

						std::map<std::string, std::string> childsMap;
						std::string requestIdString = "";
						std::string lockId = "";

						for(unsigned int i = 0; i < childs.size(); i++)
						{
							std::string child = childs.at(i);
							if(child == "requestId")
							{
								requestIdString = xmlParser->getChildElementInnerText(child);
							}
							else if(child == "lockId")
							{
								lockId = xmlParser->getChildElementInnerText(child);
							}
							else
							{
								childsMap[child] = xmlParser->getChildElementInnerText(child);
							}
						}

						bool isMandatoryCommand = true;
						int requestId = -1;
						if(checkRequestIdOk(requestIdString, requestId))
						{
							std::shared_ptr<SPICE::BIG::CommandBase> command = nullptr;
							std::shared_ptr<MandatoryCommandBase> mandatoryCommand = nullptr;
							if(silaCommand == "GetStatus")
							{
								responseSOAPAction = silaNamespace + "/GetStatusResponse";
								responseContent = _commandHandler->getXMLStatus(silaNamespace, silaNSPrefix);
								return;
					//			LogHandler::WriteLogLine("GetStatus: 1 - Success");
							}
							else
							{
								if(checkLockIdOk(lockId))
								{
									if(silaCommand == "Reset")
									{
										std::shared_ptr<Reset> resetCommand(new Reset());
										resetCommand->setSpecificReset(_commandHandler->getCoreData()->getResourceProvider()->getSpecificReset());
										mandatoryCommand = resetCommand;
									}
									else if(silaCommand == "Initialize")
									{
										std::shared_ptr<Initialize> initializeCommand(new Initialize());
										initializeCommand->setSpecificInit(_commandHandler->getCoreData()->getResourceProvider()->getSpecificInit());
										mandatoryCommand = initializeCommand;
									}
									else if(silaCommand == "LockDevice")
									{
										if(lockId == "") 
										{
											responseSOAPAction = silaNamespace + "/LockDeviceResponse";
											responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML("LockDevice", 5, "Error on lockId (Command LockDevice - lockId must not be empty)", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
											return;
//											LogHandler::WriteLogLine("Rejected: 5 - Error on lockId (should not be empty)");
										}
										else
										{
											mandatoryCommand = std::shared_ptr<MandatoryCommandBase>(new LockDevice(lockId));
										}
									}
									else if(silaCommand == "Abort")
									{
										mandatoryCommand = std::shared_ptr<MandatoryCommandBase>(new Abort());
									}
									else if(silaCommand == "DoContinue")
									{
										mandatoryCommand = std::shared_ptr<MandatoryCommandBase>(new DoContinue());
									}
									else if(silaCommand == "GetDeviceIdentification")
									{
										responseSOAPAction = silaNamespace + "/GetDeviceIdentificationResponse";
										responseContent = _commandHandler->getXMLDeviceIdentification(serverAddress, silaNamespace, silaNSPrefix);
										return;
								//		LogHandler::WriteLogLine("GetDeviceIdentification: 1 - Success");
									}
									else if(silaCommand == "Pause")
									{
										mandatoryCommand = std::shared_ptr<MandatoryCommandBase>(new Pause());
									}
									else if(silaCommand == "UnlockDevice")
									{
										mandatoryCommand = std::shared_ptr<MandatoryCommandBase>(new UnlockDevice());
									}
									else
									{
										isMandatoryCommand = false;
										SessionData::States state = _commandHandler->getCoreData()->getSessionData()->getState();

										if(_commandHandler->getCommandCountMandatory() == 0)
										{
											if(silaCommand == "GetConfiguration" || silaCommand == "SetConfiguration") // implementierung, dass dieses per configuration zu oder abgeschaltet werden kann
											{
												if(state == SessionData::States::Standby)
												{
													if(silaCommand == "GetConfiguration")
													{
														command = std::shared_ptr<SPICE::BIG::CommandBase>(new GetConfiguration());
													}
													else
													{
														command = std::shared_ptr<SPICE::BIG::CommandBase>(new SetConfiguration());
													}
												}
												else
												{
													responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
													responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 9, "Command (" + silaCommand + ") not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Standby)", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
													return;
												}
											}
											else if(state == SessionData::States::Idle || state == SessionData::States::Busy)
											{
												if(silaCommand == "SetParameters")
												{
													std::shared_ptr<SetParameters> setParametersCommand(new SetParameters());
													setParametersCommand->setSpecificSetParameters(_commandHandler->getCoreData()->getResourceProvider()->getSpecificSetParameters());
													command = setParametersCommand;
												}
												else if(silaCommand == "GetParameters")
												{
													command = std::shared_ptr<SPICE::BIG::CommandBase>(new GetParameters());
												}
												else
												{
													command = _commandHandler->getCoreData()->getResourceProvider()->createCommand(silaCommand);
													if(command != nullptr)
													{
														std::string cmdName = command->getCommandName();
														if(cmdName == "Abort" || cmdName == "DoContinue" || cmdName == "GetDeviceIdentification" ||
															cmdName == "GetStatus" || cmdName == "Initialize" || cmdName == "Pause" ||
															cmdName == "UnlockDevice" || cmdName == "LockDevice" || cmdName == "Reset") // Check that mandatory commands can not be overwriten.
														{
															command = nullptr;
														}
													}
												}
											}
											else
											{
												responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
												responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 9, "Command (" + silaCommand + ") not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Idle or Busy)", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
												return;
											}
										}
										else
										{
											responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
											responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 9, "Command (" + silaCommand + ") not allowed in this state (" + SessionData::stateToString(state) + ") (Mandatory command active)", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
											return;
										}
									}
									if(isMandatoryCommand)
									{
										command = mandatoryCommand;
									}

								}
								else
								{
									responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
									responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 5, "Error on lockId (" + silaCommand + ")", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
									return;
//									LogHandler::WriteLogLine("Rejected: 5 - Error on lockId");
								}
							}
							if(command == nullptr)
							{
								responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
								responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 11, "Invalid command parameter: unknown command (" + silaCommand + ")", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
								return;
//								LogHandler::WriteLogLine("Rejected: 11 - Invalid command parameter: unkown command");
							}
							else
							{
								// create communicationInformations (maybe fall away in future...)
								std::shared_ptr<CommunicationInformations> communicationInformations(new CommunicationInformations(ethernetEventConnectionFactory, silaNamespace));

								// create internal command
								std::shared_ptr<InternalCommand> internalCommand(new InternalCommand(command, requestId, isMandatoryCommand, _commandHandler, communicationInformations));

								// link command to internal command
								command->setCommandCallback(internalCommand);
							
								// link mandatory to internal command
								if(mandatoryCommand != nullptr)
								{
									mandatoryCommand->setMandatoryCommandCallback(internalCommand);
								}

								// assigning parameters
								for(std::map<std::string, std::string>::iterator i = childsMap.begin(); i != childsMap.end(); i++)
								{
									if(!internalCommand->setCommandParameterValue(i->first, i->second)) // TODO: maybe not needed cause reacts on not expected parameters
									{
										responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
										responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 11, "Invalid command parameter: " + i->first + " - \"" + i->second + "\" (" + silaCommand + ")", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);

										return;
//										LogHandler::WriteLogLine("Rejected: 11 - Invalid command parameter: " + i->first + " - \"" + i->second + "\"");
									}
								}

								int returnCode = -1;
								// check CommandParameters
								std::string message = "";
								if(internalCommand->checkCommandParametersOk(message))
								{
									// execute syncPart
									returnCode = 2;
									message = "Asynchronous command accepted";
									command->additionalCommandCheck(returnCode, message);

									responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
									responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, returnCode, message, "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
//									LogHandler::WriteLogLine(SiLASOAPFunctions::ConvertToString(returnCode) + " " + message);

									if(returnCode == 2)
									{
										_commandHandler->newCommand(internalCommand);
									}
									return;
								}
								else
								{
									responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
									responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 11, "Invalid command parameter: " + message + " (" + silaCommand + ")", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
									return;
//									LogHandler::WriteLogLine("Rejected: 11 - Invalid command parameter: " + message);
								}
							}
						}
						else
						{
							responseSOAPAction = silaNamespace + "/" + silaCommand + "Response";
							responseContent = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML(silaCommand, 6, "Error on requestId (" + silaCommand + ")", "PT1S", _commandHandler->getCoreData()->getResourceProvider()->getDeviceClass(), "", silaNamespace, silaNSPrefix);
							return;
//							LogHandler::WriteLogLine("Rejected: 6 - Error on requestId");
						}
				/*	}
					else
					{
						SiLASOAPFunctions::CreateFullSiLASyncResponse(responseData, silaNamespace, silaCommand, 11, "Invalid command parameter: Command in SoapData not found", 1, DeviceIdentification::GetSiLADeviceClass());
						LogHandler::WriteLogLine("Rejected: 11 - Invalid command parameter: Command in SoapData not found");
					}
					if(responseData == "")
					{
						SiLASOAPFunctions::CreateFullSiLASyncResponse(responseData, silaNamespace, silaCommand, 11, "Invalid command parameter: returnValue used to be null (programming fault)", 1, DeviceIdentification::GetSiLADeviceClass());
						LogHandler::WriteLogLine("Rejected: 11 - Invalid command parameter: returnValue used to be null");
					}
				}
				else
				{
					silaNamespace = "http://sila-standard.org";
					SiLASOAPFunctions::CreateFullSiLASyncResponse(responseData, silaNamespace, "SOAPAction", 11, "Invalid command parameter: SOAPAction not found - Wrong Namespace?", 1, DeviceIdentification::GetSiLADeviceClass());
					LogHandler::WriteLogLine("Rejected: 11 - Invalid command parameter: SOAPAction not found - Wrong Namespace?");
				}
				return responseData;*/

					}
					else
					{
						// TODO: imlementierung xmlStructure error
					}
				}
				else
				{
					// TODO: implementierung xmlStructure error
				}
			}
			void ConnectionHandler::handleWSDLConnection(std::string serverAddress, std::string& responseContent)
			{
				std::string wsdlFile = _wsdlGenerator->getWSDLFile();

				std::string strLocation = "http://" + serverAddress + "/" + _commandHandler->getCoreData()->getResourceProvider()->getCoreConfigurationParameter("URI_PATHNAME");

				wsdlFile.replace(wsdlFile.find(_wsdlGenerator->getAddressDummy()), _wsdlGenerator->getAddressDummy().length(), strLocation);

				responseContent = wsdlFile;
			}

			bool ConnectionHandler::checkLockIdOk(std::string& lockId)
			{
				if(lockId != _commandHandler->getCoreData()->getSessionData()->getLockId() && _commandHandler->getCoreData()->getSessionData()->isLocked())
				{
					return false;
				}
				return true;
			}
			bool ConnectionHandler::checkRequestIdOk(std::string& requestIdString, int& requestId)
			{
				requestId = -1;
				if(requestIdString.length() == 0)
				{
					return false; // requestId missing
				}
				try
				{
					requestId = std::stoi(requestIdString);
				}
				catch (std::exception exp)
				{
					requestId = -1;
					return false; // requestId has an invalid value
				}
				if(requestId < 1 || requestId > 2147483647)
				{
					return false; // requestId has an invalid value
				}
				// TODO: implementierung mit commandhandler führung von requestId-Liste
				return true;
			}

		}
	}
}