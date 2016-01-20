/**
	SPI Multidrop
	Dispense.h
	Purpose: Implementation of the dispense command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef DISPENSE_H
#define DISPENSE_H

#include "SpecificCommandBase.h"

#include "DataEntryInt.h"

namespace SPI
{
	namespace Multidrop
	{
		class Dispense : public SpecificCommandBase
		{
			public:
				Dispense(std::shared_ptr<SpecificCore> specificCore);
				virtual ~Dispense();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();
				virtual double calculateEstimatedDuration();

				virtual void additionalCommandCheck(int& returnCode, std::string& message);

				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> _volume;
		};
	}
}
#endif // DISPENSE_H
