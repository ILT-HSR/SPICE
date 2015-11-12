/**
	SPI Multidrop
	EmptyTubes.h
	Purpose: Implementation of the empty tubes command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef EMPTYTUBES_H
#define EMPTYTUBES_H

#include "SpecificCommandBase.h"
#include "DataEntryInt.h"

namespace SPI
{
	namespace Multidrop
	{
		class EmptyTubes : public SpecificCommandBase
		{
			public:
				EmptyTubes(std::shared_ptr<SpecificCore> specificCore);
				virtual ~EmptyTubes();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();
				virtual double calculateEstimatedDuration();

				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> _volume;
		};
	}
}
#endif // EMPTYTUBES_H
