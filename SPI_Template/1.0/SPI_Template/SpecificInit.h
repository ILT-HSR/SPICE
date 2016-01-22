/**
	SPI Template
	SpecificInit.h
	Purpose: Implementation of the specific init which is called during the initialization

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#ifndef SPECIFICINIT_H
#define SPECIFICINIT_H

#include "SpecificBase.h"

#include "SpecificCore.h"

namespace SPI
{
	namespace Template
	{
		class SpecificInit : public SPICE::BIG::SpecificBase
		{
			public:
				SpecificInit(std::shared_ptr<SpecificCore> specificCore);
				virtual ~SpecificInit();

				virtual double calculateEstimatedDuration();
				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SpecificCore> _specificCore;
		};
	}
}
#endif // SPECIFICINIT_H
