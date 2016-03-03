/**
	SPI Template
	CommandWithParameter.h
	Purpose: This command shows the recommended implementation of a command with parameters

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#ifndef COMMANDWITHPARAMETER_H
#define COMMANDWITHPARAMETER_H

#include "SpecificCommandBase.h"

#include "DataEntryBool.h"
#include "DataEntryInt.h"
#include "DataEntryUnsignedInt.h"

namespace SPI
{
	namespace Template
	{
		class CommandWithParameter : public SpecificCommandBase
		{
			public:
				CommandWithParameter(std::shared_ptr<SpecificCore> specificCore);
				virtual ~CommandWithParameter();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual bool isCommonCommand();
				virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();

				virtual double calculateEstimatedDuration();

				virtual void additionalCommandCheck(int& returnCode, std::string& message);
				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryBool> _myBoolParameter;
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> _myIntParameter;
				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt> _myUnsignedIntParameter;
		};
	}
}
#endif // COMMANDWITHPARAMETER_H
