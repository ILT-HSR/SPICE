/**
	SPI Template
	CommandWithResponseData.h
	Purpose: This command shows the recommended implementation of a command with response data

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#ifndef COMMANDWITHRESPONSEDATA_H
#define COMMANDWITHRESPONSEDATA_H

#include "SpecificCommandBase.h"

#include "DataEntryBool.h"
#include "DataEntryInt.h"
#include "DataEntryUnsignedInt.h"

namespace SPI
{
	namespace Template
	{
		class CommandWithResponseData : public SpecificCommandBase
		{
			public:
				CommandWithResponseData(std::shared_ptr<SpecificCore> specificCore);
				virtual ~CommandWithResponseData();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual bool isCommonCommand();
				virtual std::vector<std::shared_ptr<SPICE::BIG::DataSet>> getResponseDataInformation(std::string& responseDescription);

				virtual double calculateEstimatedDuration();

				virtual bool processing();

			protected:
			private:
				std::shared_ptr<SPICE::BIG::DataSet> _responseDataSet;
		};
	}
}
#endif // COMMANDWITHRESPONSEDATA_H
