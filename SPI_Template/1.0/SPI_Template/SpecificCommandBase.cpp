/**
	SPI Template
	SpecificCommandBase.cpp
	Purpose: Base for all the specific commands. Gives possibility to implement a general used function one time and be available at every command.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_20
*/

#include "SpecificCommandBase.h"

namespace SPI
{
	namespace Template
	{
		SpecificCommandBase::SpecificCommandBase(std::shared_ptr<SpecificCore> specificCore) :
			CommandBase(),
			_specificCore(specificCore)
		{
		}
		SpecificCommandBase::~SpecificCommandBase()
		{
		}

	}
}
