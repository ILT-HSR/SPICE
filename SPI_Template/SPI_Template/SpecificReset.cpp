/**
	SPI Template
	SpecificReset.cpp
	Purpose: Implementation of the specific reset which is called during the reset

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#include "SpecificReset.h"

namespace SPI
{
	namespace Template
	{
		SpecificReset::SpecificReset(std::shared_ptr<SpecificCoreContainer> coreContainer) : SpecificResetBase(),
			_specificCoreContainer(coreContainer)
		{
		}
		SpecificReset::~SpecificReset()
		{
		}

		double SpecificReset::calculateEstimatedDuration()
		{
			return _specificCoreContainer->getSpecificCore()->calculateTimePrepareForReset();
		}

		bool SpecificReset::processing(bool wasInSimulationMode, bool willBeInSimulationMode)
		{
			_specificCoreContainer->getSpecificCore()->prepareForReset(_commandCallback);
			_specificCoreContainer->resetSpecificCore(willBeInSimulationMode);
			return true;
		}
	}
}