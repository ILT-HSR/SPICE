/**
	SPICE Core
	SessionData.cpp
	Purpose: Class containing all data relating to the current state and session

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "SessionData.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			SessionData::SessionData() :
				_currentState(States::Startup),
				_locked(false),
				_lockId(""),
				_lockTimeout(0),
				_PMSId(""),
				_deviceId(""),
				_errorHandlingTimeout(0),
				_simulationMode(false),
				_eventReceiverURI("")
			{
			}
			SessionData::~SessionData()
			{
			}

			void SessionData::setState(SessionData::States state)
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				_currentState = state;
			}
			SessionData::States SessionData::getState()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				return _currentState;
			}
			std::string SessionData::getStateAsString()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				return stateToString(_currentState);
			}
			std::string SessionData::stateToString(SessionData::States state)
			{
				std::string returnValue = "";
				switch(state)
				{
				case States::Startup :
					returnValue = "startup";
					break;
				case States::Resetting :
					returnValue = "resetting";
					break;
				case States::Standby :
					returnValue = "standby";
					break;
				case States::Initializing :
					returnValue = "initializing";
					break;
				case States::Idle :
					returnValue = "idle";
					break;
				case States::Busy :
					returnValue = "busy";
					break;
				case States::Paused :
					returnValue = "paused";
					break;
				case States::ErrorHandling :
					returnValue = "errorHandling";
					break;
				case States::InError :
					returnValue = "inError";
					break;
				}
				return returnValue;
			}

			std::string SessionData::getLockId()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				return _lockId;
			}
			bool SessionData::isLocked()
			{
				return _locked;
			}
			void SessionData::lock(std::string lockId, double lockTimeout)
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				_lockId = lockId;
				_locked = true;
				_lockTimeout = lockTimeout;
			}
			void SessionData::unlock()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				_lockId = "";
				_locked = false;
			}

			void SessionData::setPMSId(std::string pmsId)
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				_PMSId = pmsId;
			}
			std::string SessionData::getPMSId()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				return _PMSId;
			}

			void SessionData::setDeviceId(std::string deviceId)
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				_deviceId = deviceId;
			}
			std::string SessionData::getDeviceId()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				return _deviceId;
			}

			void SessionData::setErrorHandlingTimeout(double errorHandlingTimeout)
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
		
				_errorHandlingTimeout = errorHandlingTimeout;
			}
			double SessionData::getErrorHandlingTimeout()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				return _errorHandlingTimeout;
			}

			void SessionData::setSimulationMode(bool activate)
			{
				_simulationMode = activate;
			}
			bool SessionData::simulationModeActive()
			{
				return _simulationMode;
			}

			void SessionData::setEventReceiverURI(std::string eventReceiverURI)
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				_eventReceiverURI = eventReceiverURI;
			}
			std::string SessionData::getEventReceiverURI()
			{
				std::lock_guard<std::mutex> lockGuard(_sessionDataMutex);
				return _eventReceiverURI;
			}
		}
	}
}