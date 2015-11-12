/**
	SPI Multidrop
	SpecificInit.h
	Purpose: Implementation of the specific init which is called during the initialization

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef SPECIFICINIT_H
#define SPECIFICINIT_H

#include <memory>

#include "SpecificBase.h"

#include "SpecificCore.h"
#include "DataSet.h"

namespace SPI
{
	namespace Multidrop
	{
		class SpecificInit : public SPICE::BIG::SpecificBase
		{
			public:
				SpecificInit(std::shared_ptr<SpecificCore> specificCore, std::shared_ptr<SPICE::BIG::DataSet> configSet);
				virtual ~SpecificInit();

				virtual double calculateEstimatedDuration();
				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SpecificCore> _specificCore;
				std::shared_ptr<SPICE::BIG::DataSet> _configSet;
		};
	}
}
#endif // SPECIFICINIT_H
