/**
	SPI Multidrop
	Shake.h
	Purpose: Implementation of the shake command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef SHAKE_H
#define SHAKE_H

#include "SpecificCommandBase.h"

#include "DataEntryDuration.h"

namespace SPI
{
	namespace Multidrop
	{
		class Shake : public SpecificCommandBase
		{
			public:
				Shake(std::shared_ptr<SpecificCore> specificCore, std::shared_ptr<SPICE::BIG::DataSet> parameterSet);
				virtual ~Shake();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();
				virtual double calculateEstimatedDuration();
	
				virtual void additionalCommandCheck(int& returnCode, std::string& message);

				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryDuration> _shakeDuration;
				std::shared_ptr<SPICE::BIG::DataSet> _parameterSet;
		};
	}
}
#endif // SHAKE_H
