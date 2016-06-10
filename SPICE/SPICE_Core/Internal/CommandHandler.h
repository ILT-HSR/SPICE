/**
	SPICE Core
	CommandHandler.h
	Purpose: Class as central coordinator of incoming, waiting, active and ending commands

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.3 2016_06_10
*/

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <thread>
#include <mutex>

#include "ICommandHandlerCallback.h"
#include "InternalCommand.h"
#include "CoreData.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class CommandHandler : public ICommandHandlerCallback
			{
				public:
					/**
						Class as central coordinator of incoming, waiting, active and ending commands
					*/
					CommandHandler();
					virtual ~CommandHandler();

					/**
						Start the command handler own thread

						@return 0 if thread could be started (1 if exception during start of thread, 2 if thread already started)
					*/
					int startCommandHandler();

					/**
						Add a new async command to the command handler.
					*/
					void newCommand(std::shared_ptr<InternalCommand> newCommand);

					/**
						Gets the core data.

						@return core data
					*/
					virtual std::shared_ptr<CoreData> getCoreData();

					/**
						Gives an information to the command handler that the command execution is finished and it should not be listed as active anymore.
						Requirement to send the responseEvent.

						@param requestId Id of the command
					*/
					virtual void commandNotActiveAnymore(int requestId);
					/**
						Gives an information to the command handler that the command has ended (includes sending of responseEvent and waiting to the response)
						Command handler will move the command to the waste.

						@param requestId Id of the command.
					*/
					virtual void commandHasEnded(int requestId);
				
					/**
						Gives an information to the command handler that the throwing command has an error and the state errorHandling has to be reached.
						The command handler will try to pause all other active commands.

						@param throwingCommandRequestId Id of the command with the error
						@return True if all other commands reached the subState "asynchPaused"
					*/
					virtual bool enterErrorHandling(int throwingCommandRequestId);
					/**
						Gives an information to the command handler that an active errorHandling state can be left.

						@param continueAllCommands Defines if all active commands should be continued or not.
					*/
					virtual void leaveErrorHandling(bool continueAllCommands);

					/**
						Function to pause all active commands.

						@param callingCommandRequestId Id of the calling command (has not to be paused)
						@return True if all active commands reached subState "asynchPaused"
					*/
					virtual bool pauseAll(int callingCommandRequestId);
					/**
						Function to continue all paused commands.
					*/
					virtual void continueAll();
					/**
						Function to abort all active commands.

						@param callingCommandRequestId Id of the calling command (has not to be aborted)
						@return True if all commands are aborted
					*/
					virtual bool abortAll(int callingCommandRequestId);
					/**
						Function ro reset all active commands.

						@param callingCommandRequestId Id of the calling command (has not to be reseted)
						@return True if all commands are resetted
					*/
					virtual bool resetAll(int callingCommandRequestId);

					/**
						Function to lock the commandHandler by a thread to execute more then one command.
					*/
					virtual void lockCommandHandler();
					/**
						To unlock a locked command handler by the locking thread.
					*/
					virtual void unlockCommandHandler();

					/**
						Gets the count of mandatory commands. 0 or 1. 2 is only possible if a reset is called during an other mandatory command is active.

						@return Count of mandatory commands
					*/
					virtual unsigned int getCommandCountMandatory();
					/**
						Gets the count of waiting commands.

						@return Count of waiting commands
					*/
					virtual unsigned int getCommandCountWaiting();
					/**
						Gets the count of active commands.

						@return Count of active commands
					*/
					virtual unsigned int getCommandCountActive();
					/**
						Gets the count of all active commands (mandatory + other active commands)

						@return Count of all active commands
					*/
					virtual unsigned int getCommandCountTotalActive();
					/**
						Gets a map with requestId and name of the currently active commands.

						@return Map with requestId and command name
					*/
					virtual std::map<int, std::string> getCommandListActive();

					/**
						Creates the sync response XML for a sync GetStatus-command

						@param silaNamespace The from the service consumer used sila namespace to be able to answer with adapted namespaces
						@param silaNSPrefix Prefix if the silaNamespace should not be included over a global namespace.
						@return The created sync response xml
					*/
					std::string getXMLStatus(std::string silaNamespace, std::string silaNSPrefix);
					/**
						Creates the sync response XML for a sync GetDeviceIdentificion-command

						@param serverAddress The address which should be given as device address
						@param silaNamespace The from the service consumer used sila namespace to be able to answer with adapted namespaces
						@param silaNSPrefix Prefix if the silaNamespace should not be included over a global namespace.
						@return The created sync response xml
					*/
					std::string getXMLDeviceIdentification(std::string serverAddress, std::string silaNamespace, std::string silaNSPrefix);

				protected:
				private:
					/**
						Main-Cycle of the command handler which cleans ending commands, starts waiting commands and takes care of
						aborting or reseting other commands in case of a command ending error
					*/
					void commandHandlerCycle();

					std::shared_ptr<CoreData> _coreData;

					std::vector<std::shared_ptr<InternalCommand>> _mandatoryCommands;
					std::vector<std::shared_ptr<InternalCommand>> _waitingCommands;
					std::map<int, std::shared_ptr<InternalCommand>> _activeCommands;
					std::map<int, std::shared_ptr<InternalCommand>> _notActiveAnymoreCommands;
					std::queue<std::shared_ptr<InternalCommand>> _endingCommands;

					std::atomic<bool> _stopCommandHandlerThread;
					std::mutex _commandHandlerMutex;
					std::unique_lock<std::mutex>* _uniqueLocker;
					std::thread* _commandHandlerThread;
					std::thread::id _lockingThreadId;
					std::atomic<bool> _commandHandlerLocked;
			};
		}
	}
}
#endif // COMMANDHANDLER_H
