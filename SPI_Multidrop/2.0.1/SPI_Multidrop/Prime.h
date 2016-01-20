/**
	SPI Multidrop
	Prime.h
	Purpose: Implementation of the prime command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef PRIME_H
#define PRIME_H

#include "SpecificCommandBase.h"
#include "DataEntryInt.h"

namespace SPI
{
	namespace Multidrop
	{
		class Prime : public SpecificCommandBase
		{
			public:
				Prime(std::shared_ptr<SpecificCore> specificCore);
				virtual ~Prime();

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
#endif // PRIME_H
