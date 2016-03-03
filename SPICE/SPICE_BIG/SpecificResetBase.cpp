/**
	SPICE BIG
	SpecificResetBase.cpp
	Purpose: Abstract base class for implementation specific reset routine

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "SpecificResetBase.h"

namespace SPICE
{
	namespace BIG
	{
		SpecificResetBase::SpecificResetBase() :
			_commandCallback(nullptr)
		{
		}
		SpecificResetBase::~SpecificResetBase()
		{
		}

		void SpecificResetBase::setCommandCallback(std::shared_ptr<ICommandCallback> commandCallback)
		{
			_commandCallback = commandCallback;
		}
	}
}