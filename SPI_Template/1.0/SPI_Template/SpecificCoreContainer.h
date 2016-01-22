/**
	SPI Template
	SpecificCoreContainer.h
	Purpose: Class which contains the specific core instance. Includes the from the "SpecificReset" called reset-function to delete and create specific core instance.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_22
*/

#ifndef SPECIFICCORECONTAINER_H
#define SPECIFICCORECONTAINER_H

#include "IResourceProvider.h"
#include "SpecificCore.h"

namespace SPI
{
	namespace Template
	{
		class SpecificCoreContainer
		{
			public:
				SpecificCoreContainer(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider);
				virtual ~SpecificCoreContainer();

				std::shared_ptr<SpecificCore> getSpecificCore();

				void resetSpecificCore(bool simulationMode);

			protected:
			private:
				std::shared_ptr<SPICE::BIG::IResourceProvider> _resourceProvider;
				std::shared_ptr<SpecificCore> _specificCore;
		};
	}
}
#endif // SPECIFICCORECONTAINER_H
