/**
	SPI Multidrop
	SpecificCoreContainer.cpp
	Purpose: Class which contains the specific core instance. Includes the from the "SpecificReset" called reset-function to delete and create specific core instance.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "SpecificCoreContainer.h"

namespace SPI
{
	namespace Multidrop
	{
		SpecificCoreContainer::SpecificCoreContainer() :
			_specificCore(nullptr)
		{
			resetSpecificCore(false);
		}
		SpecificCoreContainer::~SpecificCoreContainer()
		{
			if(_specificCore != nullptr)
			{
				_specificCore = nullptr;
			}
		}

		std::shared_ptr<SPI::Multidrop::SpecificCore> SpecificCoreContainer::getSpecificCore()
		{
			return _specificCore;
		}

		void SpecificCoreContainer::resetSpecificCore(bool simulationMode)
		{
			if(_specificCore != nullptr)
			{
				_specificCore = nullptr;
			}

			std::shared_ptr<SPI::Multidrop::SpecificCore> newSpecificCore(new SPI::Multidrop::SpecificCore(simulationMode));
			_specificCore = newSpecificCore;
		}
	}
}