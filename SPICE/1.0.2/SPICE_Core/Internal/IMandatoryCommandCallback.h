/**
	SPICE Core
	IMandatoryCommandCallback.h
	Purpose: Interface for a core internal command. Provides access for mandatory commands to the needed core functions.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IMANDATORYCOMMANDCALLBACK_H
#define IMANDATORYCOMMANDCALLBACK_H

#include <string>

#include "SessionData.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class IMandatoryCommandCallback
			{
				// Methoden
			public:
				/**
					Gets the current state of the state machine

					@return State of the state machine
				*/
				virtual SPICE::Core::Internal::SessionData::States getCurrentState() = 0;
				/**
					Sets the given state as new state of the state machine

					@param state New state for the state machine
				*/
				virtual void setCurrentState(SPICE::Core::Internal::SessionData::States state) = 0;
				/**
					Sets the given state when the response of the command is send (short before)

					@param state New state for the state machine
				*/
				virtual void setStateWithResponse(SPICE::Core::Internal::SessionData::States state) = 0;

				/**
					Function to pause all active commands.

					@return True if all active commands reached subState "asynchPaused"
				*/
				virtual bool pauseAll() = 0;
				/**
					Function to continue all paused commands.
				*/
				virtual void continueAll() = 0;
				/**
					Function to abort all active commands.

					@return True if all commands are aborted
				*/
				virtual bool abortAll() = 0;
				/**
					Function ro reset all active commands.

					@return True if all commands are resetted
				*/
				virtual bool resetAll() = 0;

				/**
					Gets the count of all active commands (mandatory + other active commands)

					@return Count of all active commands
				*/
				virtual unsigned int getTotalActiveCommandsCount() = 0;
				/**
					Gets the count of mandatory commands. 0 or 1. 2 is only possible if a reset is called during an other mandatory command is active.

					@return Count of mandatory commands
				*/
				virtual unsigned int getMandatoryCommandsCount() = 0;

				/**
					Locks the service provider for the calling PMS. Used by command "LockDevice"

					@param lockId Id which is used to lock the device and other commands will be checked with.
					@param lockTimeout Timeout in seconds until device is unlocked automatically. If 0 there won't be an automatic unlock.
				*/
				virtual void lock(std::string lockId, double lockTimeout) = 0;
				/**
					Unlocks the service provider. Used by command "UnlockDevice"
				*/
				virtual void unlock() = 0;
				/**
					Sets the eventReceiverURI for the current session

					@param eventReceiverURI URI of the service consumer.
				*/
				virtual void setEventReceiverURI(std::string eventReceiverURI) = 0;
				/**
					Sets the id of the process management system.

					@param pmsId Id of the PMS
				*/
				virtual void setPMSId(std::string pmsId) = 0;
				/**
					Sets the service provider into the simulation mode. Used by command "Reset"

					@param simulationMode True sets simulation mode active
				*/
				virtual void setSimulationMode(bool simulationMode) = 0;
				/**
					Sets the id of the device to be able to identify at the service consumer. Used by command "Reset"

					@param deviceId ID of the device
				*/
				virtual void setDeviceId(std::string deviceId) = 0;
				/**
					Sets the timeout until an errorhandling state is changed into an error state.

					@param timeoutSeconds timeout in seconds
				*/
				virtual void setErrorHandlingTimeout(double timeoutSeconds) = 0;
			};
		}
	}
}
#endif // IMANDATORYCOMMANDCALLBACK_H
