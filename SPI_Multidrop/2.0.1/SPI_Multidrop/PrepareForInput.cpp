/**
	SPI Multidrop
	PrepareForInput.cpp
	Purpose: Implementation of the prepare for input command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "PrepareForInput.h"

namespace SPI
{
	namespace Multidrop
	{
		PrepareForInput::PrepareForInput(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore)
		{
		}
		PrepareForInput::~PrepareForInput()
		{
		}

		std::string PrepareForInput::getCommandName()
		{
			return "PrepareForInput";
		}
		std::string PrepareForInput::getCommandDescription()
		{
			return "This command puts the device into a state where it is ready to accept new labware.";
		}

		double PrepareForInput::calculateEstimatedDuration()
		{
			return 5;
		}

		bool PrepareForInput::processing()
		{
			return _specificCore->prepareForInOutput(_commandCallback);
		}
	}
}