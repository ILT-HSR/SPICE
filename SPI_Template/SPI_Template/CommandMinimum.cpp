/**
	SPI Template
	CommandMinimum.cpp
	Purpose: This command shows the minimum required implementation of a command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_20
*/

#include "CommandMinimum.h"

namespace SPI
{
	namespace Template
	{
		CommandMinimum::CommandMinimum(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore)
		{
		}
		CommandMinimum::~CommandMinimum()
		{
		}

		std::string CommandMinimum::getCommandName()
		{
			return "CommandMinimum";
		}
		std::string CommandMinimum::getCommandDescription()
		{
			return "This command shows the minimum required implementation of a command";
		}
		bool CommandMinimum::isCommonCommand()
		{
			return false;
		}

		double CommandMinimum::calculateEstimatedDuration()
		{
			return _specificCore->calculateTimeCommandMinimum();
		}

		bool CommandMinimum::processing()
		{
			return _specificCore->commandMinimum(_commandCallback);
		}
	}
}