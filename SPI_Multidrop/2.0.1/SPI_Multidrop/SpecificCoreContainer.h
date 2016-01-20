/**
	SPI Multidrop
	SpecificCoreContainer.h
	Purpose: Class which contains the specific core instance. Includes the from the "SpecificReset" called reset-function to delete and create specific core instance.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef SPECIFICCORECONTAINER_H
#define SPECIFICCORECONTAINER_H

#include "SpecificCore.h"

namespace SPI
{
	namespace Multidrop
	{
		class SpecificCoreContainer
		{
			public:
				SpecificCoreContainer();
				virtual ~SpecificCoreContainer();

				std::shared_ptr<SPI::Multidrop::SpecificCore> getSpecificCore();

				void resetSpecificCore(bool simulationMode);

			protected:
			private:
				std::shared_ptr<SPI::Multidrop::SpecificCore> _specificCore;
		};
	}
}
#endif // SPECIFICCORECONTAINER_H
