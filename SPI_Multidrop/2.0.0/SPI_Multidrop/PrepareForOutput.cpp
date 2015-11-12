/**
	SPI Multidrop
	PrepareForOutput.cpp
	Purpose: Implementation of the prepare for output command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "PrepareForOutput.h"

namespace SPI
{
	namespace Multidrop
	{
		PrepareForOutput::PrepareForOutput(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore)
		{
		}
		PrepareForOutput::~PrepareForOutput()
		{
		}

		std::string PrepareForOutput::getCommandName()
		{
			return "PrepareForOutput";
		}
		std::string PrepareForOutput::getCommandDescription()
		{
			return "This command puts the device into a state to release the processed labware.";
		}

		double PrepareForOutput::calculateEstimatedDuration()
		{
			return 5;
		}

		bool PrepareForOutput::processing()
		{
			return _specificCore->prepareForInOutput(_commandCallback);
		}
	}
}