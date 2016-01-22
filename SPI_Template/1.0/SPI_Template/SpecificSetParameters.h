/**
	SPI Template
	SpecificSetParameters.h
	Purpose: Implementation which is called during the SetParameters-command. Used to be able to react on changed parameters (write it to the device as example).

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#ifndef SPECIFICSETPARAMETERS_H
#define SPECIFICSETPARAMETERS_H

#include "SpecificBase.h"

#include "SpecificCore.h"

namespace SPI
{
	namespace Template
	{
		class SpecificSetParameters : public SPICE::BIG::SpecificBase
		{
			public:
				SpecificSetParameters(std::shared_ptr<SpecificCore> specificCore);
				virtual ~SpecificSetParameters();

				virtual double calculateEstimatedDuration();
				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SpecificCore> _specificCore;
		};
	}
}
#endif // SPECIFICSETPARAMETERS_H
