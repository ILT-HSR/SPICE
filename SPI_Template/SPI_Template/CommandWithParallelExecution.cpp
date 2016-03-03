/**
	SPI Template
	CommandWithParallelExecution.cpp
	Purpose: This command shows the recommended implementation of a command with the possibility of parallel execution.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#include "CommandWithParallelExecution.h"

namespace SPI
{
	namespace Template
	{
		CommandWithParallelExecution::CommandWithParallelExecution(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore)
		{
		}
		CommandWithParallelExecution::~CommandWithParallelExecution()
		{
		}

		std::string CommandWithParallelExecution::getCommandName()
		{
			return "CommandWithParallelExecution";
		}
		std::string CommandWithParallelExecution::getCommandDescription()
		{
			return "This command shows the recommended implementation of a command with the possibility of parallel execution.";
		}
		bool CommandWithParallelExecution::isCommonCommand()
		{
			return false;
		}

		double CommandWithParallelExecution::calculateEstimatedDuration()
		{
			return _specificCore->calculateTimeCommandWithParallelExecution();
		}

		bool CommandWithParallelExecution::readyForStart()
		{
			// the default implementation checks if their is an other active command. If true, the execution doesn't start.
			// this results in no parallelism. If a command is able to run parallel, readyForStart has to be implemented.

			// functionality: the command in front of the queue is check cyclic if it is ready for start. If it returns
			// true, the command will start and call the processing function.

			// example: Is able to start this command parallel to a running, but just in case the running command is also a CommandWithParallelExecution
			if(_commandCallback->getActiveCommandsCount() != 0)
			{
				std::map<int, std::string> activeList = _commandCallback->getActiveCommandsList();
				
				for(std::map<int, std::string>::iterator i = activeList.begin(); i != activeList.end(); i++)
				{
					if(i->second != getCommandName())
					{
						return false;
					}
				}
			}
			return true;
		}
		bool CommandWithParallelExecution::processing()
		{
			return _specificCore->commandWithParallelExecution(_commandCallback);
		}
	}
}