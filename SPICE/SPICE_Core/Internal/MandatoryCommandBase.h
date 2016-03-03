/**
	SPICE BIG
	MandatoryCommandBase.h
	Purpose: Abstract base class for implementation of mandatory commands

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef MANDATORYCOMMANDBASE_H
#define MANDATORYCOMMANDBASE_H

#include <memory>

#include "IMandatoryCommandCallback.h"
#include "CommandBase.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class MandatoryCommandBase : public SPICE::BIG::CommandBase
			{
				// Methoden
			public:
				/**
					Abstract base class for implementation of mandatory commands
				*/
				MandatoryCommandBase();
				virtual ~MandatoryCommandBase();

				/**
					Sets the mandatory command callback interface which can be used by the command to get access to core functions. DO NOT OVERRIDE (FINAL)

					@param mandatoryCommandCallback The new setted mandatory command callback.
				*/
				void setMandatoryCommandCallback(std::shared_ptr<IMandatoryCommandCallback> mandatoryCommandCallback);

				/**
					Gets the estimated duration of the command

					@return estimated duration in seconds
				*/
				virtual double calculateEstimatedDuration();
				/**
					Returns if the command is a common command

					@return True if command is a common command
				*/
				virtual bool isCommonCommand();
				/**
					Is called to evalute if the command is ready to be executed. Default is: no other commands are active (no parallel execution)
					Can be implemented if parallel execution is needed.

					@return True if command is ready to start
				*/
				virtual bool readyForStart();
				/**
					Executes a simulation processing if service provider is in simulation mode. Default is: Wait over the estimated time.
					Can be implemented if an other behaviour is requested (example: give some simulation commands to the specific device core)

					@return True if command execution has been successful. False, if a not recoverable error occurred.
				*/
				virtual bool simulationProcessing();

			protected:
				std::shared_ptr<IMandatoryCommandCallback> _mandatoryCommandCallback;

			private:
			};
		}
	}
}
#endif // MANDATORYCOMMANDBASE_H
