/**
	SPI Template
	SpecificSetParameters.cpp
	Purpose: Implementation which is called during the SetParameters-command. Used to be able to react on changed parameters (write it to the device as example).

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#include "SpecificSetParameters.h"

namespace SPI
{
	namespace Template
	{
		SpecificSetParameters::SpecificSetParameters(std::shared_ptr<SpecificCore> specificCore) : SpecificBase(),
			_specificCore(specificCore)
		{
		}
		SpecificSetParameters::~SpecificSetParameters()
		{
		}

		double SpecificSetParameters::calculateEstimatedDuration()
		{
			return _specificCore->calculateTimeSetParameters();
		}

		bool SpecificSetParameters::processing()
		{
			return _specificCore->setParameters(_commandCallback);
		}
	}
}