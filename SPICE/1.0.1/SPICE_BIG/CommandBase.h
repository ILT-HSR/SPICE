/**
	SPICE BIG
	CommandBase.h
	Purpose: Abstract base class for implementation specific commands

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef COMMANDBASE_H
#define COMMANDBASE_H

#include <string>
#include <memory>

#include "ICommandCallback.h"

namespace SPICE
{
	namespace BIG
	{
		class CommandBase
		{
			// Methoden
		public:
			/**
				Abstract base class for implementation specific commands
			*/
			CommandBase();
			virtual ~CommandBase();

			/**
				Sets the command callback interface which can be used by the command to get access to core functions. DO NOT OVERRIDE (FINAL)

				@param commandCallback The new setted command callback.
			*/
			void setCommandCallback(std::shared_ptr<ICommandCallback> commandCallback);

			/**
				Gets the name of the command as string.

				@return Command name as string.
			*/
			virtual std::string getCommandName() = 0;
			/**
				Gets the description of the command.

				@return Description.
			*/
			virtual std::string getCommandDescription() = 0;
			/**
				Can be implemented if the command has additional parameters (additional to requestId and lockId). Additional created parameters should be stored in the deduction and returnd in a vector.

				@return Vector with the additional command parameters
			*/
			virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();
			/**
				Can be implemented if the command has data sets for the ResponseData. Is used to generate the correct WSDL-File.

				@param responseDescription Can be used to give a human readable response description.
				@return Vector with the additional data sets to return.
			*/
			virtual std::vector<std::shared_ptr<SPICE::BIG::DataSet>> getResponseDataInformation(std::string& responseDescription);
			/**
				Gets the estimated duration of the command

				@return estimated duration in seconds
			*/
			virtual double calculateEstimatedDuration() = 0;
			/**
				Returns if the command is a common command

				@return True if command is a common command
			*/
			virtual bool isCommonCommand() = 0;

			/**
				Is called to evaluate if the command is valid. If not, the SiLA-request is rejected.
				Can be implemented if additional checks are required (as example specific dependencies between parameters).

				@param returnCode The current prepared returnCode for the sync-message. Can be changed if rejection is needed.
				@param message Human readable message according to the returnCode. Should be changed if returnCode is changed.
			*/
			virtual void additionalCommandCheck(int& returnCode, std::string& message);

			/**
				Is called to evalute if the command is ready to be executed. Default is: no other commands are active (no parallel execution)
				Can be implemented if parallel execution is needed.

				@return True if command is ready to start
			*/
			virtual bool readyForStart();
			/**
				Executes the implementation specific actions for a command.

				@return True if command execution or command abort has been successful. False, if a not recoverable error occurred.
			*/
			virtual bool processing() = 0;
			/**
				Executes a simulation processing if service provider is in simulation mode. Default is: Wait over the estimated time.
				Can be implemented if an other behaviour is requested (example: give some simulation commands to the specific device core)

				@return True if command execution or command abort has been successful. False, if a not recoverable error occurred.
			*/
			virtual bool simulationProcessing();

		protected:
			std::shared_ptr<ICommandCallback> _commandCallback;

		private:
		};
	}
}
#endif // COMMANDBASE_H
