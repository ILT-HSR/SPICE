/**
	SPI Multidrop
	SpecificReset.cpp
	Purpose: Implementation of the specific reset which is called during the reset

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "SpecificReset.h"

namespace SPI
{
	namespace Multidrop
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
			return 5;
		}

		bool SpecificReset::processing(bool wasInSimulationMode, bool willBeInSimulationMode)
		{
			if(!_specificCoreContainer->getSpecificCore()->isReadyToReset())
			{
				std::shared_ptr<SpecificCore> specificCore = _specificCoreContainer->getSpecificCore();

				specificCore->reset(_commandCallback);

				specificCore->closeConnection();
			}
			_specificCoreContainer->resetSpecificCore(willBeInSimulationMode);
			return true;
		}
	}
}