/**
	SPICE Core
	SessionData.h
	Purpose: Class containing all data relating to the current state and session

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef SESSIONDATA_H
#define SESSIONDATA_H

#include <string>
#include <mutex>
#include <atomic>

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class SessionData
			{
				public:
					enum States
					{
						Startup,
						Resetting,
						Standby,
						Initializing,
						Idle,
						InError,
						Busy,
						Paused,
						ErrorHandling
					};

					/**
						Class containing all data relating to the current state and session
					*/
					SessionData();
					virtual ~SessionData();

					/**
						Sets the current state of the state machine

						@param state New state
					*/
					void setState(SessionData::States state); 
					/**
						Gets the current state of the state machine

						@return Current state
					*/
					SessionData::States getState();
					/**
						Gets the current state of the state machine as string

						@return Current state as string
					*/
					std::string getStateAsString();
					/**
						Converts a given state to a string

						@return Given state as string
					*/
					static std::string stateToString(SessionData::States state);

					/**
						Gets the lockId.

						@return lockId. Returns an empty string if device isn't locked
					*/
					std::string getLockId();
					/**
						Returns if the device is locked or not.

						@return True if the device is locked
					*/
					bool isLocked();
					/**
						Locks the device with the given lockId and the lockTimeout

						@param lockId The new lockId
						@param lockTimeout Timeout until an automatic unlock will be executed. 0 deactivates the timeout.
					*/
					void lock(std::string lockId, double lockTimeout);
					/**
						Unlock a locked device
					*/
					void unlock();

					/**
						Sets the given PMSId

						@param pmsId Given PMSId
					*/
					void setPMSId(std::string pmsId);
					/**
						Gets the PMSId

						@return PMSId
					*/
					std::string getPMSId();

					/**
						Sets the given DeviceId

						@param deviceId Given DeviceId
					*/
					void setDeviceId(std::string deviceId);
					/**
						Gets the DeviceId

						@return DeviceId
					*/
					std::string getDeviceId();

					/**
						Sets the given ErrorHandlingTimeout

						@param errorHandlingTimeout Given errorHandlingTimeout in seconds
					*/
					void setErrorHandlingTimeout(double errorHandlingTimeout);
					/**
						Gets the ErrorHandlingTimeout

						@return ErrorHandlingTimeout
					*/
					double getErrorHandlingTimeout();

					/**
						Sets the simulation mode to given value.

						@param activate True activates the simulation mode
					*/
					void setSimulationMode(bool activate);
					/**
						Returns if the the simulation mode is active or not

						@return True if the simulation mode is active
					*/
					bool simulationModeActive();

					/**
						Sets the event receiver URI

						@param eventReceiverURI New event receiver URI
					*/
					void setEventReceiverURI(std::string eventReceiverURI);
					/**
						Gets the current event receiver URI

						@return Current event receiver URI
					*/
					std::string getEventReceiverURI();


				protected:
				private:
					std::mutex _sessionDataMutex;

					SessionData::States _currentState;
					std::atomic<bool> _locked;
					std::string _lockId;
					double _lockTimeout;
					std::string _PMSId;
					std::string _deviceId;
					double _errorHandlingTimeout;
					std::atomic<bool> _simulationMode;
					std::string _eventReceiverURI;
			};
		}
	}
}
#endif // SESSIONDATA_H
