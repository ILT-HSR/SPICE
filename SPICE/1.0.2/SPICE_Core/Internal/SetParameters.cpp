/**
	SPICE Core
	SetParameters.h
	Purpose: Provides the functionality of the setParameters-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "SetParameters.h"

#include "SiLASOAPFunctions.h"
#include "IXMLParser.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    SetParameters::SetParameters()
		    {
		    }
		    SetParameters::~SetParameters()
		    {
		    }

			void SetParameters::setSpecificSetParameters(std::shared_ptr<SPICE::BIG::SpecificBase> specificSetParameters)
			{
				_specificSetParameters = specificSetParameters;
			}

		    std::string SetParameters::getCommandName()
		    {
			    return "SetParameters";
		    }
		    std::string SetParameters::getCommandDescription()
		    {
			    return "This command is used to send new parameter values to the device.";
		    }
		    std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> SetParameters::createAndGetAdditionalCommandParameters()
		    {
                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> paramsXML(new SPICE::BIG::DataEntryTypes::DataEntryString("paramsXML", false));
                paramsXML->setAdditionalInformations("The xml Document of paramsXML is formatted as stated in the ParameterSet tag.", "", "");
                paramsXML->setIsNeeded(true);

			    _paramsXML = paramsXML;

				std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
				returnVector.push_back(paramsXML);
				return returnVector;
		    }

		    double SetParameters::calculateEstimatedDuration()
		    {
			    return _specificSetParameters->calculateEstimatedDuration();
		    }
            bool SetParameters::isCommonCommand()
			{
                return true;
			}

		    bool SetParameters::processing()
		    {
			    return internalProcessing(false);
		    }

		    bool SetParameters::simulationProcessing()
		    {
			    return internalProcessing(true);
		    }

			bool SetParameters::internalProcessing(bool simulationMode)
			{
			    std::string paramsXML = _paramsXML->getValueAsString();

			    // xmlFile umwandeln
			    std::shared_ptr<SPICE::BIG::DataSet> parameterSet = _commandCallback->getParameterSet();
                std::shared_ptr<SPICE::BIG::IXMLParser> xmlParser = _commandCallback->getNewXMLParserInstance();

				std::string message = parameterSet->parseParameterSetFromXMLFile(paramsXML, xmlParser);
				if(message != "")
				{
                    std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("SetParameters: " + message, "EnterInError", "Enters the inError-state, following commands can not be executed (more secure)", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
					error->addContinuationTask("AbortSetParameters", "Aborts the SetParameters command. Can continue with next command. (maybe not secure!)", SPICE::BIG::ContinuationTask::TaskTypes::abortCurrentCommand);
				    _commandCallback->throwError(error);
				    return false;
				}

				_specificSetParameters->setCommandCallback(_commandCallback);
			    bool returnValue = false;
			    if(!simulationMode)
			    {
				    returnValue = _specificSetParameters->processing();
			    }
			    else
			    {
				    returnValue = _specificSetParameters->simulationProcessing();
			    }
				_specificSetParameters->setCommandCallback(nullptr);

			    return returnValue;
			}

		}
	}
}