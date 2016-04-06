/**
	SPICE Core
	CommandHandler.cpp
	Purpose: Class as central coordinator of incoming, waiting, active and ending commands

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_01_20
*/

#include "CommandHandler.h"

#include <sstream>
#include <iostream>

#include "GeneralFunctions.h"
#include "SiLASOAPFunctions.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			CommandHandler::CommandHandler() :
				_coreData(std::shared_ptr<CoreData>(new CoreData())),
				_stopCommandHandlerThread(false),
				_uniqueLocker(nullptr),
				_commandHandlerThread(nullptr),
				_lockingThreadId(std::this_thread::get_id()),
				_commandHandlerLocked(false)
			{
				//ctor
			}

			CommandHandler::~CommandHandler()
			{
				//dtor
				if(_commandHandlerThread != nullptr)
				{
					_stopCommandHandlerThread = true;
					if(_commandHandlerThread->joinable())
					{
						_commandHandlerThread->join();
					}
					delete _commandHandlerThread;
				}
			}

			int CommandHandler::startCommandHandler()
			{
				if(_commandHandlerThread == nullptr)
				{
					try
					{
						_stopCommandHandlerThread = false;
						_commandHandlerThread = new std::thread(&CommandHandler::commandHandlerCycle, this);
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
			void CommandHandler::newCommand(std::shared_ptr<InternalCommand> newCommand)
			{
				lockCommandHandler();

				if(newCommand->isMandatoryCommand())
				{
					_mandatoryCommands.push_back(newCommand);
					newCommand->start();
				}
				else
				{
					if(_waitingCommands.size() == 0 && newCommand->readyForStart())
					{
						_activeCommands[newCommand->getRequestId()] = newCommand;
						newCommand->start();
					}
					else
					{
						_waitingCommands.push_back(newCommand);
					}
				}
				if(!newCommand->isMandatoryCommand() && _coreData->getSessionData()->getState() == SessionData::States::Idle)
				{
					_coreData->getSessionData()->setState(SessionData::States::Busy);
				}

				unlockCommandHandler();
			}

			std::shared_ptr<CoreData> CommandHandler::getCoreData()
			{
				return _coreData;
			}

			void CommandHandler::commandNotActiveAnymore(int requestId)
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				std::shared_ptr<InternalCommand> changeCommand = nullptr;

				for(unsigned int i = 0; i < _mandatoryCommands.size(); i++)
				{
					if(_mandatoryCommands.at(i)->getRequestId() == requestId)
					{
						changeCommand = _mandatoryCommands.at(i);
						_mandatoryCommands.erase(_mandatoryCommands.begin() + i);
						i = _mandatoryCommands.size();
					}

				}

				if(changeCommand == nullptr)
				{
					try
					{
						changeCommand = _activeCommands.at(requestId);
						_activeCommands.erase(requestId);
					} catch (std::exception e)
					{
						std::cerr << "Exeception at commandSendsResponseEvent";
						return;
					}
				}
				_notActiveAnymoreCommands[requestId] = changeCommand;
			}
			void CommandHandler::commandHasEnded(int requestId)
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				std::shared_ptr<InternalCommand> changeCommand = nullptr;

				try
				{
					changeCommand = _notActiveAnymoreCommands.at(requestId);
					_notActiveAnymoreCommands.erase(requestId);
				}
				catch (std::exception e)
				{
					std::cerr << "Exception at commandHasEnded: command missing in \"notActiveAnymore\"";
					return;
				}
				_endingCommands.push(changeCommand);
			}
			bool CommandHandler::enterErrorHandling(int throwingCommandRequestId)
			{
				if(_coreData->getSessionData()->getState() == SessionData::States::Busy)
				{
					_coreData->getSessionData()->setState(SessionData::States::ErrorHandling);

					bool continuePausing = true;

					while(continuePausing)
					{
						if(pauseAll(throwingCommandRequestId))
						{
							continuePausing = false;
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
					}
					return true;
				}
				return false;
			}
			void CommandHandler::leaveErrorHandling(bool continueAllCommands)
			{
				if(continueAllCommands)
				{
					continueAll();
				}
				_coreData->getSessionData()->setState(SessionData::States::Busy);
			}

			bool CommandHandler::pauseAll(int callingCommandRequestId)
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				bool allPaused = true;

				for(std::map<int, std::shared_ptr<InternalCommand>>::iterator i = _activeCommands.begin(); i != _activeCommands.end(); i++)
				{
					if(i->first != callingCommandRequestId)
					{
						InternalCommand::SubStates subState = i->second->getSubState();
						if(subState != InternalCommand::SubStates::AsynchPaused &&
							subState != InternalCommand::SubStates::FinishedAbort &&
							subState != InternalCommand::SubStates::FinishedError &&
							subState != InternalCommand::SubStates::FinishedOk &&
							subState != InternalCommand::SubStates::FinishedWarning &&
							subState != InternalCommand::SubStates::NotStarted)
						{
							allPaused = false;
							if(subState != InternalCommand::SubStates::PauseRequested)
							{
								i->second->pause();
							}
						}
					}
				}
				return allPaused;
			}
			void CommandHandler::continueAll()
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				for(std::map<int, std::shared_ptr<InternalCommand>>::iterator i = _activeCommands.begin(); i != _activeCommands.end(); i++)
				{
					i->second->doContinue();
				}
			}
			bool CommandHandler::abortAll(int callingCommandRequestId)
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				bool allAborted = true;

				while(!_waitingCommands.empty())
				{
					_waitingCommands.pop_back();
				}

				for(std::map<int, std::shared_ptr<InternalCommand>>::iterator i = _activeCommands.begin(); i != _activeCommands.end(); i++)
				{
					if(i->first != callingCommandRequestId)
					{
						i->second->abort();
						allAborted = false;
					}
				}

				return allAborted;
			}
			bool CommandHandler::resetAll(int callingCommandRequestId)
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				bool allReseted = true;

				while(!_waitingCommands.empty())
				{
					_waitingCommands.pop_back();
				}

				for(std::map<int, std::shared_ptr<InternalCommand>>::iterator i = _activeCommands.begin(); i != _activeCommands.end(); i++)
				{
					if(i->first != callingCommandRequestId)
					{
						i->second->reset();
						allReseted = false;
					}
				}

				for(std::vector<std::shared_ptr<InternalCommand>>::iterator i = _mandatoryCommands.begin(); i != _mandatoryCommands.end(); i++)
				{
					if(i->get()->getRequestId() != callingCommandRequestId)
					{
						i->get()->reset();
						allReseted = false;
					}
				}

				return allReseted;
			}

			void CommandHandler::lockCommandHandler()
			{
				std::unique_lock<std::mutex>* newUniqueLocker = new std::unique_lock<std::mutex>(_commandHandlerMutex, std::defer_lock);

				newUniqueLocker->lock();

				_uniqueLocker = newUniqueLocker;
				_lockingThreadId = std::this_thread::get_id();
				_commandHandlerLocked = true;
			}
			void CommandHandler::unlockCommandHandler()
			{
				if(std::this_thread::get_id() == _lockingThreadId && _uniqueLocker != nullptr && _commandHandlerLocked)
				{
					std::unique_lock<std::mutex>* oldUniqueLocker = _uniqueLocker;
					_uniqueLocker = nullptr;
					_commandHandlerLocked = false;
					oldUniqueLocker->unlock();
					delete oldUniqueLocker;
				}
			}

			unsigned int CommandHandler::getCommandCountMandatory()
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				return _mandatoryCommands.size();
			}
			unsigned int CommandHandler::getCommandCountWaiting()
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				return _waitingCommands.size();
			}
			unsigned int CommandHandler::getCommandCountActive()
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				return _activeCommands.size();
			}
			unsigned int CommandHandler::getCommandCountTotalActive()
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				return _activeCommands.size() + _mandatoryCommands.size();
			}
			std::map<int, std::string> CommandHandler::getCommandListActive()
			{
				std::unique_lock<std::mutex> localUniqueLock(_commandHandlerMutex, std::defer_lock);
				if(!(_commandHandlerLocked && std::this_thread::get_id() == _lockingThreadId))
				{
					localUniqueLock.lock();
				}

				std::map<int, std::string> commandMap;

				for (std::vector<std::shared_ptr<InternalCommand>>::iterator i = _mandatoryCommands.begin(); i != _mandatoryCommands.end(); i++)
				{
					commandMap[i->get()->getRequestId()] = i->get()->getCommandName();
				}
				for(std::map<int, std::shared_ptr<InternalCommand>>::iterator i = _activeCommands.begin(); i != _activeCommands.end(); i++)
				{
					commandMap[i->first] = i->second->getCommandName();
				}

				return commandMap;
			}

			std::string CommandHandler::getXMLStatus(std::string silaNamespace, std::string silaNSPrefix)
			{
				time_t _startedTime = time(NULL);

				std::ostringstream strout;
				strout << "<" << silaNSPrefix << "deviceId>" << _coreData->getSessionData()->getDeviceId() << "</" << silaNSPrefix << "deviceId>";
				strout << "<" << silaNSPrefix << "state>" << _coreData->getSessionData()->getStateAsString() << "</" << silaNSPrefix << "state>";

				lockCommandHandler();

				if(getCommandCountTotalActive() + getCommandCountWaiting() > 0)
				{
					strout << "<" << silaNSPrefix << "subStates>";

					for(std::vector<std::shared_ptr<InternalCommand>>::iterator i = _mandatoryCommands.begin(); i != _mandatoryCommands.end(); i++)
					{
						strout << i->get()->getXMLCommandDescription(0);
					}
					for(std::map<int, std::shared_ptr<InternalCommand>>::iterator i = _activeCommands.begin(); i != _activeCommands.end(); i++)
					{
						strout << i->second->getXMLCommandDescription(0);
					}
					int positionCounter = 1;
					for(std::vector<std::shared_ptr<InternalCommand>>::iterator i = _waitingCommands.begin(); i != _waitingCommands.end(); i++)
					{
						strout << i->get()->getXMLCommandDescription(positionCounter);
						positionCounter++;
					}
					strout << "</" << silaNSPrefix << "subStates>";
				}
				else
				{
					if(_coreData->getResourceProvider()->getCoreConfigurationParameter("NICELABFIX_SUBSTATES") == "true")
					{
						strout << "<" << silaNSPrefix << "subStates></" << silaNSPrefix << "subStates>";
					}
				}

				unlockCommandHandler();

				std::string lockedValue = "false";
				if(_coreData->getSessionData()->isLocked())
				{
					lockedValue = "true";
				}
				strout << "<" << silaNSPrefix << "locked>" << lockedValue << "</" << silaNSPrefix << "locked>";
				strout << "<" << silaNSPrefix << "PMSId>" << _coreData->getSessionData()->getPMSId() << "</" << silaNSPrefix << "PMSId>";
				time_t timeNow = time(NULL);
				strout << "<" << silaNSPrefix << "currentTime>" << SPICE::BIG::GeneralFunctions::convertTimeToUTCString(timeNow) << "</" << silaNSPrefix << "currentTime>";

				std::string returnValue = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML("GetStatus", 1, "Success", "***DURATION***", _coreData->getResourceProvider()->getDeviceClass(), strout.str(), silaNamespace, silaNSPrefix);

				std::string duration = SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(difftime(time(NULL), _startedTime));
				returnValue.replace(returnValue.find("***DURATION***"), 14, duration);

				return returnValue;
			}
			std::string CommandHandler::getXMLDeviceIdentification(std::string serverAddress, std::string silaNamespace, std::string silaNSPrefix)
			{
				time_t _startedTime = time(NULL);

				std::string uriPathName;
				int silaDeviceClass;
				std::string deviceManufacturer;
				std::string deviceName;
				std::string deviceSerialNumber;
				std::string deviceFirmwareVersion;
				bool includeConverterIdentification;
				_coreData->getResourceProvider()->getDeviceIdentification(uriPathName, silaDeviceClass, deviceManufacturer, deviceName, deviceSerialNumber, deviceFirmwareVersion, includeConverterIdentification);

				std::ostringstream strout;
				strout << "<" << silaNSPrefix << "deviceDescription>";
				strout << "<" << silaNSPrefix << "Wsdl>http://" << serverAddress << uriPathName << "?wsdl</" << silaNSPrefix << "Wsdl>";
				strout << "<" << silaNSPrefix << "SiLAInterfaceVersion>1.3.08</" << silaNSPrefix << "SiLAInterfaceVersion>";
				strout << "<" << silaNSPrefix << "SiLADeviceClass>" << silaDeviceClass << "</" << silaNSPrefix << "SiLADeviceClass>";
				strout << "<" << silaNSPrefix << "SiLADeviceClassVersion>1.2.01</" << silaNSPrefix << "SiLADeviceClassVersion>";
				if(includeConverterIdentification)
				{
					strout << "<" << silaNSPrefix << "DeviceManufacturer>http://ilt.hsr.ch / " << deviceManufacturer << "</" << silaNSPrefix << "DeviceManufacturer>";
					strout << "<" << silaNSPrefix << "DeviceName>SPICE / " << deviceName << "</" << silaNSPrefix << "DeviceName>";
					strout << "<" << silaNSPrefix << "DeviceSerialNumber>" << _coreData->getSerialNumber() << " / " << deviceSerialNumber << "</" << silaNSPrefix << "DeviceSerialNumber>";
					strout << "<" << silaNSPrefix << "DeviceFirmwareVersion>v1.0.3 (20160406) / " << deviceFirmwareVersion << "</" << silaNSPrefix << "DeviceFirmwareVersion>";
				}
				else
				{
					strout << "<" << silaNSPrefix << "DeviceManufacturer>" << deviceManufacturer << "</" << silaNSPrefix << "DeviceManufacturer>";
					strout << "<" << silaNSPrefix << "DeviceName>" << deviceName << "</" << silaNSPrefix << "DeviceName>";
					strout << "<" << silaNSPrefix << "DeviceSerialNumber>" << _coreData->getSerialNumber() << " / " << deviceSerialNumber << "</" << silaNSPrefix << "DeviceSerialNumber>";
					strout << "<" << silaNSPrefix << "DeviceFirmwareVersion>" << deviceFirmwareVersion << "</" << silaNSPrefix << "DeviceFirmwareVersion>";
				}
				strout << "</" << silaNSPrefix << "deviceDescription>";

				std::string returnValue = SPICE::BIG::SiLASOAPFunctions::createSyncResponseXML("GetDeviceIdentification", 1, "Success", "***DURATION***", _coreData->getResourceProvider()->getDeviceClass(), strout.str(), silaNamespace, silaNSPrefix);

				std::string duration = SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(difftime(time(NULL), _startedTime));
				returnValue.replace(returnValue.find("***DURATION***"), 14, duration);

				return returnValue;
			}

			void CommandHandler::commandHandlerCycle()
			{
				std::unique_lock<std::mutex> lock(_commandHandlerMutex, std::defer_lock);
				bool abortAllFlag = false;

				while (!_stopCommandHandlerThread)
				{
					// Clean ending commands
					if(_endingCommands.size() > 0)
					{
						lock.lock();
						std::shared_ptr<InternalCommand> endingCommand = _endingCommands.front();
						_endingCommands.pop();
						lock.unlock();

						endingCommand->joinThread();

						if(endingCommand->getSubState() == InternalCommand::SubStates::FinishedError)
						{
							_coreData->getSessionData()->setState(SessionData::States::InError);
						}
						else if(endingCommand->getSubState() == InternalCommand::SubStates::FinishedAbort)
						{
							abortAllFlag = true;
						}
					}

					// Try starting commands
					if((_coreData->getSessionData()->getState() == SessionData::States::Idle ||
						_coreData->getSessionData()->getState() == SessionData::States::Busy) && !abortAllFlag)
					{
						// checkStarts
						lockCommandHandler();
						if(_waitingCommands.size() > 0)
						{
							std::shared_ptr<InternalCommand> nextWaiting = _waitingCommands[0];
							if(nextWaiting->readyForStart())
							{
								nextWaiting->start();
								_activeCommands[nextWaiting->getRequestId()] = nextWaiting;
								_waitingCommands.erase(_waitingCommands.begin());
							}
						}
						unlockCommandHandler();
					}

					// If InError abort all running commands
					if(_coreData->getSessionData()->getState() == SessionData::States::InError)
					{
						abortAllFlag = true;
					}

					// AbortAll and EmptyBuffer
					if(abortAllFlag)
					{
						lockCommandHandler();
						switch(getCommandCountTotalActive())
						{
						case 0 :
							abortAllFlag = false;
							break;
						case 1 :
							if(getCommandCountMandatory() == 1)
							{
								std::string mandatoryCommandName = _mandatoryCommands.at(0)->getCommandName();
								if(mandatoryCommandName == "Reset" || mandatoryCommandName == "Abort")
								{
									abortAllFlag = false;
								}
							}
							break;
						default :
							break;
						}
						unlockCommandHandler();
						if(abortAllFlag)
						{
							abortAllFlag = !abortAll(-1);
						}
					}


					// If all commands end go to state idle
					if(_coreData->getSessionData()->getState() == SessionData::States::Busy &&
						getCommandCountActive() == 0 && getCommandCountWaiting() == 0)
					{
						_coreData->getSessionData()->setState(SessionData::States::Idle);
					}

					// If no ending commands, no other commands can start. So do a wait.
					if(_endingCommands.size() == 0)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
					}
				}
			}
		}
	}
}
