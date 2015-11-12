/**
	SPICE Core
	ICommandHandlerCallback.h
	Purpose: Interface of the internal command handler. Provides needed functionalities to the InternalCommand.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef ICOMMANDHANDLERCALLBACK_H
#define ICOMMANDHANDLERCALLBACK_H

#include <map>
#include <string>

#include "CoreData.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class ICommandHandlerCallback
			{
				// Methoden
			public:
				/**
					Gets the core data.

					@return core data.
				*/
				virtual std::shared_ptr<CoreData> getCoreData() = 0;

				/**
					Gives an information to the command handler that the command execution is finished and it should not be listed as active anymore.
					Requirement to send the responseEvent.

					@param requestId Id of the command
				*/
				virtual void commandNotActiveAnymore(int requestId) = 0;
				/**
					Gives an information to the command handler that the command has ended (includes sending of responseEvent and waiting to the response)
					Command handler will move the command to the waste.

					@param requestId Id of the command.
				*/
				virtual void commandHasEnded(int requestId) = 0;
				
				/**
					Gives an information to the command handler that the throwing command has an error and the state errorHandling has to be reached.
					The command handler will try to pause all other active commands.

					@param throwingCommandRequestId Id of the command with the error
					@return True if all other commands reached the subState "asynchPaused"
				*/
				virtual bool enterErrorHandling(int throwingCommandRequestId) = 0;
				/**
					Gives an information to the command handler that an active errorHandling state can be left.

					@param continueAll Defines if all active commands should be continued or not.
				*/
				virtual void leaveErrorHandling(bool continueAll) = 0;

				/**
					Function to pause all active commands.

					@param callingCommandRequestId Id of the calling command (has not to be paused)
					@return True if all active commands reached subState "asynchPaused"
				*/
				virtual bool pauseAll(int callingCommandRequestId) = 0;
				/**
					Function to continue all paused commands.
				*/
				virtual void continueAll() = 0;
				/**
					Function to abort all active commands.

					@param callingCommandRequestId Id of the calling command (has not to be aborted)
					@return True if all commands are aborted
				*/
				virtual bool abortAll(int callingCommandRequestId) = 0;
				/**
					Function ro reset all active commands.

					@param callingCommandRequestId Id of the calling command (has not to be reseted)
					@return True if all commands are resetted
				*/
				virtual bool resetAll(int callingCommandRequestId) = 0;

				/**
					Function to lock the commandHandler by a thread to execute more then one command.
				*/
				virtual void lockCommandHandler() = 0;
				/**
					To unlock a locked command handler by the locking thread.
				*/
				virtual void unlockCommandHandler() = 0;

				/**
					Gets the count of mandatory commands. 0 or 1. 2 is only possible if a reset is called during an other mandatory command is active.

					@return Count of mandatory commands
				*/
				virtual unsigned int getCommandCountMandatory() = 0;
				/**
					Gets the count of waiting commands.

					@return Count of waiting commands
				*/
				virtual unsigned int getCommandCountWaiting() = 0;
				/**
					Gets the count of active commands.

					@return Count of active commands
				*/
				virtual unsigned int getCommandCountActive() = 0;
				/**
					Gets the count of all active commands (mandatory + other active commands)

					@return Count of all active commands
				*/
				virtual unsigned int getCommandCountTotalActive() = 0;
				/**
					Gets a map with requestId and name of the currently active commands.

					@return Map with requestId and command name
				*/
				virtual std::map<int, std::string> getCommandListActive() = 0;
			};
		}
	}
}
#endif // ICOMMANDHANDLERCALLBACK_H
