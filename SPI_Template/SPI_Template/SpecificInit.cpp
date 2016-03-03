/**
	SPI Template
	SpecificInit.cpp
	Purpose: Implementation of the specific init which is called during the initialization

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#include "SpecificInit.h"

namespace SPI
{
	namespace Template
	{
		SpecificInit::SpecificInit(std::shared_ptr<SpecificCore> specificCore) : SpecificBase(),
			_specificCore(specificCore)
		{
		}
		SpecificInit::~SpecificInit()
		{
		}

		double SpecificInit::calculateEstimatedDuration()
		{
			return _specificCore->calculateTimeInitialize();
		}

		bool SpecificInit::processing()
		{
			return _specificCore->initialize(_commandCallback);
		}
	}
}