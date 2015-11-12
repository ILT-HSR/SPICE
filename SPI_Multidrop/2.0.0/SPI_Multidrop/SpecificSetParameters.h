/**
	SPI Multidrop
	SpecificSetParameters.h
	Purpose: Implementation of the specific parameters which is called during the SetParameters-command. Used to be able to change given parameters.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef SPECIFICSETPARAMETERS_H
#define SPECIFICSETPARAMETERS_H

#include <memory>

#include "SpecificBase.h"

#include "SpecificCore.h"
#include "DataSet.h"

namespace SPI
{
	namespace Multidrop
	{
		class SpecificSetParameters : public SPICE::BIG::SpecificBase
		{
			public:
				SpecificSetParameters(std::shared_ptr<SpecificCore> specificCore, std::shared_ptr<SPICE::BIG::DataSet> parameterSet);
				virtual ~SpecificSetParameters();

				virtual double calculateEstimatedDuration();
				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SpecificCore> _specificCore;
				std::shared_ptr<SPICE::BIG::DataSet> _parameterSet;
		};
	}
}
#endif // SPECIFICSETPARAMETERS_H
