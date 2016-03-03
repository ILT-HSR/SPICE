/**
	SPICE BIG
	InternalCommand.cpp
	Purpose: Class for core internal handling of a command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "InternalCommand.h"

#include <sstream>
#include "IXMLParser.h"
#include "GeneralFunctions.h"
#include "SiLASOAPFunctions.h"
#include <time.h>

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			InternalCommand::InternalCommand(std::shared_ptr<SPICE::BIG::CommandBase> baseCommand, int requestId, bool isMandatoryCommand, std::shared_ptr<ICommandHandlerCallback> commandHandlerCallback, std::shared_ptr<CommunicationInformations> communicationInformations) :
				_commandHandlerCallback(commandHandlerCallback),
				_aborting(false),
				_resetting(false),
				_pausing(false),
				_doingContinue(false),
				_requestId(requestId),
				_isMandatoryCommand(isMandatoryCommand),
				_subState(SubStates::NotStarted),
				_processThread(nullptr),
				_baseCommand(baseCommand),
				_dataWaitingCount(0),
				_setStateWithResponse(false),
				_setStateWithResponseNewState(SessionData::States::InError),
				_responseEventData(""),
				_communicationInformations(communicationInformations)
			{
				std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> additionalParameters = baseCommand->createAndGetAdditionalCommandParameters();

				for(unsigned int i = 0; i < additionalParameters.size(); i++)
				{
					std::shared_ptr<SPICE::BIG::DataEntry> additionalParameter = additionalParameters.at(i);
					if(_commandParametersMap.count(additionalParameter->getName()) == 0)
					{
						_commandParametersMap[additionalParameter->getName()] = additionalParameter;
					}
					else
					{
						std::string message = "Tried to add already added parameter to _commandParametersMap. Programming fault by implemented command \"" + baseCommand->getCommandName() + "\".";
						throw std::logic_error(message);
					}
				}
			}

			InternalCommand::~InternalCommand(void)
			{
				if(_processThread != nullptr)
				{
					if(_processThread->joinable())
					{
						_processThread->join();
					}
					delete _processThread;
				}
			}

			// InternalCommand
			int InternalCommand::start()
			{
				if(_processThread == nullptr && getSubState() == SubStates::NotStarted)
				{
					try
					{
						_processThread = new std::thread(&InternalCommand::basicProcessing, this);
						while(getSubState() == SubStates::NotStarted)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(5));
						}
					}
					catch(std::exception e)
					{
						return 1;
					}

					return 0;
				}
				else
				{
					return 2;
				}
			}
			void InternalCommand::pause()
			{
				std::lock_guard<std::mutex> lockGuard(_commandMutex);

				if(_subState == SubStates::Processing)
				{
					_subState = SubStates::PauseRequested;
					_pausing = true;
				}
				else if(_subState == SubStates::ResponseWaiting)
				{
					_pausing = true;
				}
			}
			bool InternalCommand::doContinue()
			{
				std::lock_guard<std::mutex> lockGuard(_commandMutex);
				if(_subState == SubStates::AsynchPaused)
				{
					_doingContinue = true;
				}
				else
				{
					return false;
				}
				return true;
			}
			void InternalCommand::abort()
			{
				_aborting = true;
			}
			void InternalCommand::reset()
			{
				_resetting = true;
			}
			void InternalCommand::joinThread()
			{
				if(_processThread != nullptr)
				{
					if(_processThread->joinable())
					{
						_processThread->join();
					}
				}
			}

			std::string InternalCommand::getXMLCommandDescription(int queuePosition)
			{
				std::lock_guard<std::mutex> lockGuard(_commandMutex);
				std::ostringstream strout;
				strout << "<subStates>";
				strout << "<requestId>" << _requestId << "</requestId>";
				strout << "<commandName>" << _baseCommand->getCommandName() << "</commandName>";
				strout << "<queuePosition>" << queuePosition << "</queuePosition>";
				if(queuePosition == 0)
				{
					strout << "<startedAt>" << SPICE::BIG::GeneralFunctions::convertTimeToUTCString(_startedTime) << "</startedAt>";
					switch(_subState)
					{
					case SubStates::AsynchPaused :
						strout << "<currentState>AsynchPaused</currentState>";
						break;
					case SubStates::PauseRequested :
						strout << "<currentState>PauseRequested</currentState>";
						break;
					case SubStates::Processing :
						strout << "<currentState>Processing</currentState>";
						break;
					case SubStates::ResponseWaiting :
						strout << "<currentState>ResponseWaiting</currentState>";
						break;
					case SubStates::NotStarted :
						strout << "<currentState></currentState>";
						break;
					default :
						return "";
						break;
					}
					strout << "<dataWaiting>" << _dataWaitingCount << "</dataWaiting>";
				}
				strout << "</subStates>";
				return strout.str();
			}
			bool InternalCommand::isMandatoryCommand()
			{
				return _isMandatoryCommand;
			}
			int InternalCommand::getRequestId()
			{
				return _requestId;
			}
			InternalCommand::SubStates InternalCommand::getSubState()
			{
				std::lock_guard<std::mutex> lockGuard(_commandMutex);
				return _subState;
			}

			bool InternalCommand::setCommandParameterValue(std::string name, std::string value)
			{
				if(_commandParametersMap.count(name) == 1)
				{
					return _commandParametersMap[name]->setValueString(value);
				}
				else
				{
					return false;
				}
			}

			bool InternalCommand::checkCommandParametersOk(std::string& errorMessage)
			{
				for(std::map<std::string, std::shared_ptr<SPICE::BIG::DataEntry>>::iterator i = _commandParametersMap.begin(); i != _commandParametersMap.end(); i++)
				{
					if(i->second->getIsNeeded() && !i->second->getIsSet())
					{
						errorMessage = "Parameter " + i->first + " is needed";
						return false;
					}
				}
				return true;
			}

			bool InternalCommand::readyForStart()
			{
				if(_isMandatoryCommand || _commandHandlerCallback->getCommandCountMandatory() == 0)
				{
					return _baseCommand->readyForStart();
				}
				else
				{
					return false;
				}
			}
			std::string InternalCommand::getCommandName()
			{
				return _baseCommand->getCommandName();
			}

			std::shared_ptr<SPICE::BIG::CommandBase> InternalCommand::getBaseCommand()
			{
				return _baseCommand;
			}


	// --------------- Implementierung ICommandCallback -------------
			bool InternalCommand::hasToPause()
			{
				return _pausing;
			}
			void InternalCommand::enterPause()
			{
				_pausing = false;
				SubStates lastActiveState = getSubState();
				setSubState(SubStates::AsynchPaused);

				while(!_doingContinue && !_resetting)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(50));
				}

				if(!_resetting)
				{
					setSubState(lastActiveState);
				}
				_doingContinue = false;
			}
			bool InternalCommand::hasToAbort()
			{
				return _aborting;
			}
			bool InternalCommand::hasToReset()
			{
				return _resetting;
			}
			bool InternalCommand::hasToAbortOrReset()
			{
				return _aborting || _resetting;
			}

			std::string InternalCommand::throwError(std::shared_ptr<SPICE::BIG::CommandError> processingError)
			{
				std::string silaNamespace = _communicationInformations->getSiLANamespace();
				if(processingError->getContTasksCount() == 0)
				{
					throw std::logic_error("Tried to throw processingError without continuation task");
				}
				else
				{
					if(!processingError->isRecoverable())
					{
						processingError->setDefaultTaskAsSelectedTask();
						_errorQueue.push(processingError);
						return processingError->getSelectedTaskName();
					}
					else
					{
						std::string commandName = _baseCommand->getCommandName();
						if(commandName != "Initialize" && commandName != "Reset")
						{
							bool waitingOnErrorHandling = true;
							while(waitingOnErrorHandling)
							{
								if(_pausing)
								{
									enterPause();
								}
								waitingOnErrorHandling = !_commandHandlerCallback->enterErrorHandling(_requestId);
								if(_resetting)
								{
									return "";
								}
								std::this_thread::sleep_for(std::chrono::milliseconds(20));
							}
						}

						std::string sendXMLEvent = getXMLErrorEvent(1000, processingError->getErrorMessage(), processingError->getXMLContinuationTaskParameter());
						std::shared_ptr<SPICE::BIG::IEthernetEventConnection> eventConnection = _communicationInformations->getEventConnectionFactory()->getNewEthernetEventConnection();
						eventConnection->setEventInformations(_commandHandlerCallback->getCoreData()->getSessionData()->getEventReceiverURI(), sendXMLEvent, "ErrorEvent");
//						std::shared_ptr<Internal::Communication::EventConnection> eventConnection(new Internal::Communication::EventConnection(_coreData->eventManager.GetEventReceiverURI(), "ErrorEvent", sendXMLEvent, soapActionPrefix));
						eventConnection->start();

						bool stillWaiting = true;
						while(stillWaiting)
						{
							if(eventConnection->hasFinished())
							{
								stillWaiting = false;
							}

							if(_resetting)
							{
								return nullptr;
							}
							std::this_thread::sleep_for(std::chrono::milliseconds(20));
						}

						bool hasConnectionError = false;
						if(!eventConnection->hadError())
						{
							std::string eventResponseContent = eventConnection->getResponseContent();

							std::shared_ptr<SPICE::BIG::IXMLParser> xmlParser = _commandHandlerCallback->getCoreData()->getResourceProvider()->getNewXMLParserInstance();
							if(xmlParser->loadXMLStructure(eventResponseContent))
							{
								if(xmlParser->selectElementByNameAndNS("ErrorEventResponse", silaNamespace))
								{
									std::string continuationTaskXML = xmlParser->getChildElementInnerText("continuationTask");
									SPICE::BIG::SiLASOAPFunctions::replaceInString(continuationTaskXML, "utf-16", "utf-8");

									std::shared_ptr<SPICE::BIG::IXMLParser> xmlParser2 = _commandHandlerCallback->getCoreData()->getResourceProvider()->getNewXMLParserInstance();
									if(xmlParser2->loadXMLStructure(continuationTaskXML))
									{
										processingError->setSelectedTask(xmlParser2->getFirstElementAtPathInnerText("/ContinuationTask/SelectedTask/TaskType"));
									}
									else
									{
										std::shared_ptr<SPICE::BIG::CommandError> connectionError(new SPICE::BIG::CommandError("Error during errorHandling: Received continuationTaskXML not valid"));
										_errorQueue.push(connectionError);
										hasConnectionError = true;
									}
								}
								else
								{
									std::shared_ptr<SPICE::BIG::CommandError> connectionError(new SPICE::BIG::CommandError("Error during errorHandling: Element ErrorEventResponse not found"));
									_errorQueue.push(connectionError);
									hasConnectionError = true;
								}
							}
							else
							{
								std::shared_ptr<SPICE::BIG::CommandError> connectionError(new SPICE::BIG::CommandError("Error during errorHandling: Received soapMessage not valid"));
								_errorQueue.push(connectionError);
								hasConnectionError = true;
							}

						}
						else
						{
							hasConnectionError = true;
						}

						if(processingError->getSelectedTaskType() == SPICE::BIG::ContinuationTask::TaskTypes::continueCommand && !hasConnectionError)
						{
							_commandHandlerCallback->leaveErrorHandling(true);
						}
						else
						{
							_errorQueue.push(processingError);
							_commandHandlerCallback->leaveErrorHandling(false);
						}
						return processingError->getSelectedTaskName();
					}
				}
			}

			unsigned int InternalCommand::getWaitingCommandsCount()
			{
				return _commandHandlerCallback->getCommandCountWaiting();
			}
			unsigned int InternalCommand::getActiveCommandsCount()
			{
				return _commandHandlerCallback->getCommandCountActive();
			}
			std::map<int, std::string> InternalCommand::getActiveCommandsList()
			{
				return _commandHandlerCallback->getCommandListActive();
			}

			void InternalCommand::setResponseEventData(std::string responseEventData)
			{
				_responseEventData = responseEventData;
			}
			std::shared_ptr<SPICE::BIG::DataSet> InternalCommand::getParameterSet()
			{
				return _commandHandlerCallback->getCoreData()->getResourceProvider()->getParameterSet();
			}
			std::shared_ptr<SPICE::BIG::DataSet> InternalCommand::getConfigurationSet(int configLevel, std::string password)
			{
				return _commandHandlerCallback->getCoreData()->getResourceProvider()->getConfigurationSet(configLevel, password);
			}

			std::shared_ptr<SPICE::BIG::IXMLParser> InternalCommand::getNewXMLParserInstance()
			{
				return _commandHandlerCallback->getCoreData()->getResourceProvider()->getNewXMLParserInstance();
			}
			void InternalCommand::configurationSetHasChanged(int configLevel)
			{
				_commandHandlerCallback->getCoreData()->getResourceProvider()->configurationSetHasChanged(configLevel);
			}


	// -------------- Implementierung IMandatoryCommandCallback ----------------
			SessionData::States InternalCommand::getCurrentState()
			{
				return _commandHandlerCallback->getCoreData()->getSessionData()->getState();
			}
			void InternalCommand::setCurrentState(SessionData::States state)
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->setState(state);
			}
			void InternalCommand::setStateWithResponse(SessionData::States state)
			{
				_setStateWithResponse = true;
				_setStateWithResponseNewState = state;
			}

			bool InternalCommand::pauseAll()
			{
				return _commandHandlerCallback->pauseAll(_requestId);
			}
			void InternalCommand::continueAll()
			{
				_commandHandlerCallback->continueAll();
			}
			bool InternalCommand::abortAll()
			{
				return _commandHandlerCallback->abortAll(_requestId);
			}
			bool InternalCommand::resetAll()
			{
				return _commandHandlerCallback->resetAll(_requestId);
			}

			unsigned int InternalCommand::getTotalActiveCommandsCount()
			{
				return _commandHandlerCallback->getCommandCountTotalActive();
			}
			unsigned int InternalCommand::getMandatoryCommandsCount()
			{
				return _commandHandlerCallback->getCommandCountMandatory();
			}

			void InternalCommand::lock(std::string lockId, double lockTimeout)
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->lock(lockId, lockTimeout);
			}
			void InternalCommand::unlock()
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->unlock();
			}
			void InternalCommand::setEventReceiverURI(std::string eventReceiverURI)
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->setEventReceiverURI(eventReceiverURI);
			}
			void InternalCommand::setPMSId(std::string pmsId)
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->setPMSId(pmsId);
			}
			void InternalCommand::setSimulationMode(bool simulationMode)
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->setSimulationMode(simulationMode);
			}
			void InternalCommand::setDeviceId(std::string deviceId)
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->setDeviceId(deviceId);
			}
			void InternalCommand::setErrorHandlingTimeout(double timeoutSeconds)
			{
				_commandHandlerCallback->getCoreData()->getSessionData()->setErrorHandlingTimeout(timeoutSeconds);
			}

			// Private
			void InternalCommand::basicProcessing()
			{
				if(true)
				{
					std::lock_guard<std::mutex> lockGuard(_commandMutex);
					_startedTime = time(NULL);
				}
				setSubState(SubStates::Processing);
				bool resultSuccess;
				if(!_commandHandlerCallback->getCoreData()->getSessionData()->simulationModeActive())
				{
					resultSuccess = _baseCommand->processing();
				}
				else
				{
					resultSuccess = _baseCommand->simulationProcessing();
				}

				if(_pausing)
				{
					enterPause();
				}

				setSubState(SubStates::ResponseWaiting);

				bool enterInError = false;
				bool abortAll = false;
				if(!_resetting)
				{
					bool errorDetected = responseWaiting();
					resultSuccess = resultSuccess && (!errorDetected);

					std::string responseEvent = "";
					if(resultSuccess && _errorQueue.size() == 0)
					{
						responseEvent = getXMLResponseEvent(3, "success", _responseEventData);
					}
					else
					{
						std::ostringstream message;
						if(_errorQueue.size() > 0)
						{
							resultSuccess = false;
							while(_errorQueue.size() > 0)
							{
								message << _errorQueue.front()->getErrorMessage();
								enterInError = enterInError || _errorQueue.front()->getSelectedTaskType() == SPICE::BIG::ContinuationTask::TaskTypes::fatalError;
								abortAll = abortAll || enterInError || _errorQueue.front()->getSelectedTaskType() == SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands;
								_errorQueue.pop();
								if(_errorQueue.size() > 0)
								{
									message << " | ";
								}
							}
						}
						else
						{
							if(_aborting)
							{
								message << "Command aborted";
							}
							else
							{
								message << "Not thrown error... (" << _baseCommand->getCommandName() << ")";
								enterInError = true;
								abortAll = true;
							}
						}
						responseEvent = getXMLResponseEvent(12, message.str(),_responseEventData);
					}

					if(enterInError)
					{
						_commandHandlerCallback->lockCommandHandler();
						setCurrentState(SessionData::States::InError);
						_commandHandlerCallback->commandNotActiveAnymore(_requestId);
						_commandHandlerCallback->unlockCommandHandler();
					}
					else
					{
						if(_setStateWithResponse)
						{
							setCurrentState(_setStateWithResponseNewState);
						}
						_commandHandlerCallback->commandNotActiveAnymore(_requestId);
					}
					sendEvent(responseEvent, "ResponseEvent");

					errorDetected = responseWaiting();
					resultSuccess = resultSuccess && (!errorDetected);
				}
				else
				{
					_commandHandlerCallback->commandNotActiveAnymore(_requestId);
				}

				if(resultSuccess || _resetting)
				{
					setSubState(SubStates::FinishedOk);
				}
				else if(enterInError)
				{
					setSubState(SubStates::FinishedError);
				}
				else if(abortAll)
				{
					setSubState(SubStates::FinishedAbort);
				}
				else
				{
					setSubState(SubStates::FinishedWarning);
				}
				// release baseCommand-Connection
				_baseCommand->setCommandCallback(nullptr);
				_baseCommand = nullptr;
				_commandHandlerCallback->commandHasEnded(_requestId);
			}
			bool InternalCommand::responseWaiting() // TODO: ansehen wie sich eventConnectionErrors auswirken
			{
				if(_pausing)
				{
					enterPause();
				}
				bool errorDetected = false;
				bool stillWaiting = _eventConnectionQueue.size() > 0;
				while (stillWaiting && !_resetting)
				{
					std::shared_ptr<SPICE::BIG::IEthernetEventConnection> eventConnection = _eventConnectionQueue.front();
					if(eventConnection->hasFinished())
					{
						errorDetected = errorDetected || eventConnection->hadError();
						_eventConnectionQueue.pop();
						stillWaiting = _eventConnectionQueue.size() > 0;
					}
					else
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
					}
					if(_pausing)
					{
						enterPause();
					}
				}
				return errorDetected;
			}
			std::string InternalCommand::getXMLResponseEvent(int returnCode, std::string message, std::string responseData)
			{
				std::lock_guard<std::mutex> lockGuard(_commandMutex);

				time_t timeNow = time(NULL);
				double secondsDuration = difftime(timeNow, _startedTime);

				std::string silaNamespace = _communicationInformations->getSiLANamespace();
				std::string silaNSPrefix = _commandHandlerCallback->getCoreData()->getResourceProvider()->getCoreConfigurationParameter("SILA_NS_PREFIX");
				return SPICE::BIG::SiLASOAPFunctions::createResponseEvent(_requestId, returnCode, message, secondsDuration, _commandHandlerCallback->getCoreData()->getResourceProvider()->getDeviceClass(), responseData, silaNSPrefix, silaNamespace);
			}
			std::string InternalCommand::getXMLErrorEvent(int returnCode, std::string message, std::string continuationTask)
			{
				std::lock_guard<std::mutex> lockGuard(_commandMutex);

				time_t timeNow = time(NULL);
				double secondsDuration = difftime(timeNow, _startedTime);

				std::string silaNamespace = _communicationInformations->getSiLANamespace();
				std::string silaNSPrefix = _commandHandlerCallback->getCoreData()->getResourceProvider()->getCoreConfigurationParameter("SILA_NS_PREFIX");
				return SPICE::BIG::SiLASOAPFunctions::createErrorEvent(_requestId, returnCode, message, secondsDuration, _commandHandlerCallback->getCoreData()->getResourceProvider()->getDeviceClass(), continuationTask, silaNSPrefix, silaNamespace);
			}
			void InternalCommand::sendEvent(std::string soapEvent, std::string eventType)
			{
				std::shared_ptr<SPICE::BIG::IEthernetEventConnection> eventConnection = _communicationInformations->getEventConnectionFactory()->getNewEthernetEventConnection();
				eventConnection->setEventInformations(_commandHandlerCallback->getCoreData()->getSessionData()->getEventReceiverURI(), soapEvent, _communicationInformations->getSiLANamespace() + "/" + eventType);
				eventConnection->start();
				_eventConnectionQueue.push(eventConnection);

/*				bool stillCleaningUpQueue = true;
				while(stillCleaningUpQueue)
				{
					std::shared_ptr<SPICE::BIG::IEthernetEventConnection> checkingConnection = _eventConnectionQueue.front();
					if(checkingConnection->HasFinished())
					{
						_eventConnectionQueue.pop();
						stillCleaningUpQueue = _eventConnectionQueue.size() > 0;
					}
					else
					{
						stillCleaningUpQueue = false;
					}
				}*/
			}
			void InternalCommand::setSubState(SubStates newSubState)
			{
				std::lock_guard<std::mutex> lockGuard(_commandMutex);
				_subState = newSubState;
			}
		}
	}
}
