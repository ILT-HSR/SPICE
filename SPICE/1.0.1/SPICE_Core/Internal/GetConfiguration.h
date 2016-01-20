/**
	SPICE Core
	GetConfiguration.h
	Purpose: Provides the functionality of the getConfiguration-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef GETCONFIGURATION_H
#define GETCONFIGURATION_H

#include "CommandBase.h"

#include "DataEntryInt.h"
#include "DataEntryString.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class GetConfiguration : public SPICE::BIG::CommandBase
		    {
			    public:
				    GetConfiguration();
				    virtual ~GetConfiguration();

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
		    };
		}
	}
}
#endif // GETCONFIGURATION_H
