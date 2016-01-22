/**
	SPI Template
	SpecificReset.h
	Purpose: Implementation of the specific reset which is called during the reset

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#ifndef SPECIFICRESET_H
#define SPECIFICRESET_H

#include "SpecificResetBase.h"

#include "SpecificCoreContainer.h"

namespace SPI
{
	namespace Template
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
