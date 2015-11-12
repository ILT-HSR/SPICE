/**
	SPI Multidrop
	DispenseByColumn.h
	Purpose: Implementation of the dispense by column command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef DISPENSEBYCOLUMN_H
#define DISPENSEBYCOLUMN_H

#include "SpecificCommandBase.h"
#include "DataEntryInt.h"

namespace SPI
{
	namespace Multidrop
	{
		class DispenseByColumn : public SpecificCommandBase
		{
			public:
				DispenseByColumn(std::shared_ptr<SpecificCore> specificCore);
				virtual ~DispenseByColumn();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();
				virtual double calculateEstimatedDuration();

				virtual void additionalCommandCheck(int& returnCode, std::string& message);

				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> _volume;
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> _startColumn;
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> _stopColumn;
		};
	}
}
#endif // DISPENSEBYCOLUMN_H
