/**
	SPICE Core
	SetConfiguration.cpp
	Purpose: Provides the functionality of the setConfiguration-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "SetConfiguration.h"

#include "SiLASOAPFunctions.h"
#include "IXMLParser.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    SetConfiguration::SetConfiguration()
		    {
		    }
		    SetConfiguration::~SetConfiguration()
		    {
		    }

		    std::string SetConfiguration::getCommandName()
		    {
			    return "SetConfiguration";
		    }
		    std::string SetConfiguration::getCommandDescription()
		    {
			    return "The SetConfiguration command is used to set the configuration of a device. This command can only be executed in the standby state.";
		    }
		    std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> SetConfiguration::createAndGetAdditionalCommandParameters()
		    {
                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> configLevel(new SPICE::BIG::DataEntryTypes::DataEntryInt("configLevel", 1, 2147483647));
                configLevel->setAdditionalInformations("This parameter is defines the access level.","","1");
                configLevel->setIsNeeded(true);

                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> password(new SPICE::BIG::DataEntryTypes::DataEntryString("password", true));
                password->setAdditionalInformations("This parameter contains the password for the corresponding configLevel.", "", "");
                password->setIsNeeded(false);

                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> configXML(new SPICE::BIG::DataEntryTypes::DataEntryString("configXML", false));
                configXML->setAdditionalInformations("The xml Document of paramsXML is formatted as stated in the ParameterSet tag.", "", "");
                configXML->setIsNeeded(true);

			    _configLevel = configLevel;
			    _password = password;
			    _configXML = configXML;

				std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
				returnVector.push_back(configLevel);
				returnVector.push_back(password);
				returnVector.push_back(configXML);
				return returnVector;
		    }

		    double SetConfiguration::calculateEstimatedDuration()
		    {
			    return 1;
		    }
            bool SetConfiguration::isCommonCommand()
			{
                return true;
			}

		    void SetConfiguration::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    int configLevel = 1;
			    std::string password = "";

			    if(_configLevel->getIsSet())
			    {
				    configLevel = _configLevel->getValue();
			    }
			    if(_password->getIsSet())
			    {
				    password = _password->getValueAsString();
			    }
			    std::shared_ptr<SPICE::BIG::DataSet> configSet = _commandCallback->getConfigurationSet(configLevel, password);

			    if(configSet == nullptr)
			    {
				    returnCode = 11;
				    message = "Invalid command parameter: no config for this configLevel and password";
			    }
		    }

		    bool SetConfiguration::processing()
		    {
			    int configLevel = 1;
			    std::string password = "";

			    if(_configLevel->getIsSet())
			    {
				    configLevel = _configLevel->getValue();
			    }
			    if(_password->getIsSet())
			    {
				    password = _password->getValueAsString();
			    }

				std::string configXML = _configXML->getValueAsString();

				// xmlFile umwandeln
			    std::shared_ptr<SPICE::BIG::DataSet> configSet = _commandCallback->getConfigurationSet(configLevel, password);
                std::shared_ptr<SPICE::BIG::IXMLParser> xmlParser = _commandCallback->getNewXMLParserInstance();

				std::string message = configSet->parseParameterSetFromXMLFile(configXML, xmlParser);
				if(message != "")
				{
                    std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("SetConfiguration: " + message, "EnterInError", "Enters the InError state", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
				    _commandCallback->throwError(error);
				    return false;
				}
                _commandCallback->configurationSetHasChanged(configLevel);

			    return true;
		    }

		    bool SetConfiguration::simulationProcessing()
		    {
			    return processing();
		    }
        }
	}
}