/**
	SPI Template
	CommandWithResponseData.cpp
	Purpose: This command shows the recommended implementation of a command with response data

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#include "CommandWithResponseData.h"

namespace SPI
{
	namespace Template
	{
		CommandWithResponseData::CommandWithResponseData(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore),
			_responseDataSet(nullptr)
		{
		}
		CommandWithResponseData::~CommandWithResponseData()
		{
		}

		std::string CommandWithResponseData::getCommandName()
		{
			return "CommandWithResponseData";
		}
		std::string CommandWithResponseData::getCommandDescription()
		{
			return "This command shows the recommended implementation of a command with response data";
		}
		bool CommandWithResponseData::isCommonCommand()
		{
			return false;
		}
		std::vector<std::shared_ptr<SPICE::BIG::DataSet>> CommandWithResponseData::getResponseDataInformation(std::string& responseDescription)
		{
			// This method is a little bit problematic cause the sila-standard has a weak definition how the responseData is descripted within the WSDL.
			// So if this function is not implemented, the responseData would not be defined within the WSDL. But it's still possible to give
			// responseData to the ServiceConsumer. Anyway at the moment their isn't a ServiceConsumer witch would analyse the responseXML. So you could left this method.

			// set a description
			responseDescription = "My response data";

			// Create the parameters
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryBool> myResponseBool(new SPICE::BIG::DataEntryTypes::DataEntryBool("myResponseBool"));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> myResponseInt(new SPICE::BIG::DataEntryTypes::DataEntryInt("myResponseInt"));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt> myResponseUnsignedInt(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt("myResponseUnsignedInt"));

			// combine to a dataSet
			std::shared_ptr<SPICE::BIG::DataSet> responseDataSet(new SPICE::BIG::DataSet());
			responseDataSet->addDataEntry(myResponseBool);
			responseDataSet->addDataEntry(myResponseInt);
			responseDataSet->addDataEntry(myResponseUnsignedInt);
			_responseDataSet = responseDataSet;

			// Create a vector and add the parameters
			std::vector<std::shared_ptr<SPICE::BIG::DataSet>> returnVector;
			returnVector.push_back(responseDataSet);

			return returnVector;
		}

		double CommandWithResponseData::calculateEstimatedDuration()
		{
			return _specificCore->calculateTimeCommandWithResponseData();
		}

		bool CommandWithResponseData::processing()
		{
			bool returnValue = _specificCore->commandWithResponseData(_responseDataSet, _commandCallback);
			_commandCallback->setResponseEventData(_responseDataSet->getXMLParameterSet());
			return returnValue;
		}
	}
}