/**
	SPICE Core
	GetConfiguration.cpp
	Purpose: Provides the functionality of the getConfiguration-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "GetConfiguration.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    GetConfiguration::GetConfiguration()
		    {
		    }
		    GetConfiguration::~GetConfiguration()
		    {
		    }

		    std::string GetConfiguration::getCommandName()
		    {
			    return "GetConfiguration";
		    }
		    std::string GetConfiguration::getCommandDescription()
		    {
			    return "The GetConfiguration command is used to a retrieve the configuration of a device. It can only be invoked in the standby state.";
		    }

		    std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> GetConfiguration::createAndGetAdditionalCommandParameters()
		    {
                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> configLevel(new SPICE::BIG::DataEntryTypes::DataEntryInt("configLevel", 1, 2147483647));
                configLevel->setAdditionalInformations("This parameter is defines the access level.","","");
                configLevel->setIsNeeded(true);

                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> password(new SPICE::BIG::DataEntryTypes::DataEntryString("password", true));
                password->setAdditionalInformations("This parameter contains the password for the corresponding configLevel.", "", "");
                password->setIsNeeded(false);

			    _configLevel = configLevel;
			    _password = password;

				std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
				returnVector.push_back(configLevel);
				returnVector.push_back(password);
				return returnVector;
		    }

		    double GetConfiguration::calculateEstimatedDuration()
		    {
			    return 1;
		    }
            bool GetConfiguration::isCommonCommand()
			{
                return true;
			}

		    void GetConfiguration::additionalCommandCheck(int& returnCode, std::string& message)
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
				    message = "Invalid command parameter: no configuration for this configLevel and password";
			    }
		    }

		    bool GetConfiguration::processing()
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

				_commandCallback->setResponseEventData(configSet->getXMLParameterSet());
			
			    return true;
		    }

		    bool GetConfiguration::simulationProcessing()
		    {
			    return processing();
		    }
		}
	}
}