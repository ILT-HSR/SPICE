/**
	SPI Template
	SpecificCoreContainer.cpp
	Purpose: Class which contains the specific core instance. Includes the from the "SpecificReset" called reset-function to delete and create specific core instance.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_22
*/

#include "SpecificCoreContainer.h"

namespace SPI
{
	namespace Template
	{
		SpecificCoreContainer::SpecificCoreContainer(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider) :
			_resourceProvider(resourceProvider),
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

		std::shared_ptr<SpecificCore> SpecificCoreContainer::getSpecificCore()
		{
			return _specificCore;
		}

		void SpecificCoreContainer::resetSpecificCore(bool simulationMode)
		{
			if(_specificCore != nullptr)
			{
				_specificCore = nullptr;
			}

			std::shared_ptr<SpecificCore> newSpecificCore(new SpecificCore(simulationMode, _resourceProvider));
			_specificCore = newSpecificCore;
		}
	}
}