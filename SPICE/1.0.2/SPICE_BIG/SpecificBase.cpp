/**
	SPICE BIG
	SpecificBase.cpp
	Purpose: Abstract base class for implementation of specific init / setParameters routine

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "SpecificBase.h"

#include <thread>

namespace SPICE
{
	namespace BIG
	{
		SpecificBase::SpecificBase() :
			_commandCallback(nullptr)
		{
		}
		SpecificBase::~SpecificBase()
		{
		}

		void SpecificBase::setCommandCallback(std::shared_ptr<ICommandCallback> commandCallback)
		{
			_commandCallback = commandCallback;
		}

		bool SpecificBase::simulationProcessing()
		{
			long neededCycles = (long)(calculateEstimatedDuration() * 50);

			while(neededCycles > 0)
			{
				neededCycles -= 1;
				std::this_thread::sleep_for(std::chrono::milliseconds(20));

				if(_commandCallback->hasToAbortOrReset())
				{
					neededCycles = -1;
				}
				if(_commandCallback->hasToPause())
				{
					_commandCallback->enterPause();
				}
			}
			return !_commandCallback->hasToReset();
		}
	}
}