/**
	SPI Multidrop
	SpecificReset.h
	Purpose: Implementation of the specific reset which is called during the reset

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef SPECIFICRESET_H
#define SPECIFICRESET_H

#include <memory>

#include "SpecificResetBase.h"

#include "SpecificCoreContainer.h"

namespace SPI
{
	namespace Multidrop
	{
		class SpecificReset : public SPICE::BIG::SpecificResetBase
		{
			public:
				SpecificReset(std::shared_ptr<SpecificCoreContainer> coreContainer);
				virtual ~SpecificReset();

				virtual double calculateEstimatedDuration();
				virtual bool processing(bool wasInSimulationMode, bool willBeInSimulationMode);

			protected:
			private:
				std::shared_ptr<SpecificCoreContainer> _specificCoreContainer;
		};
	}
}
#endif // SPECIFICRESET_H
