/**
	SPICE Core
	SetConfiguration.h
	Purpose: Provides the functionality of the setConfiguration-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef SETCONFIGURATION_H
#define SETCONFIGURATION_H

#include "CommandBase.h"

#include "DataEntryString.h"
#include "DataEntryInt.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class SetConfiguration : public SPICE::BIG::CommandBase
		    {
			    public:
				    SetConfiguration();
				    virtual ~SetConfiguration();

				    virtual std::string getCommandName();
				    virtual std::string getCommandDescription();
				    virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();
				    virtual double calculateEstimatedDuration();
					virtual bool isCommonCommand();

				    virtual void additionalCommandCheck(int& returnCode, std::string& message);

				    virtual bool processing();
				    virtual bool simulationProcessing();
				
			    protected:
			    private:
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> _configLevel;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _password;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _configXML;

		    };
		}
	}
}
#endif // SETCONFIGURATION_H
