/**
	SPI Template
	CommandWithSpecificSimulation.cpp
	Purpose: This command shows the recommended implementation of a command with a specific simulation

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#include "CommandWithSpecificSimulation.h"

namespace SPI
{
	namespace Template
	{
		CommandWithSpecificSimulation::CommandWithSpecificSimulation(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore)
		{
		}
		CommandWithSpecificSimulation::~CommandWithSpecificSimulation()
		{
		}

		std::string CommandWithSpecificSimulation::getCommandName()
		{
			return "CommandWithSpecificSimulation";
		}
		std::string CommandWithSpecificSimulation::getCommandDescription()
		{
			return "This command shows the recommended implementation of a command with a specific simulation";
		}
		bool CommandWithSpecificSimulation::isCommonCommand()
		{
			return false;
		}

		double CommandWithSpecificSimulation::calculateEstimatedDuration()
		{
			return _specificCore->calculateTimeCommandWithSpecificSimulation();
		}

		bool CommandWithSpecificSimulation::processing()
		{
			return _specificCore->commandWithSpecificSimulation(_commandCallback);
		}
		bool CommandWithSpecificSimulation::simulationProcessing()
		{
			return _specificCore->simulateCommandWithSpecificSimulation(_commandCallback);
		}
	}
}