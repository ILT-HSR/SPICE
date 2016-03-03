/**
	SPICE Core
	SetParameters.h
	Purpose: Provides the functionality of the setParameters-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef SETPARAMETERS_H
#define SETPARAMETERS_H

#include "CommandBase.h"
#include "SpecificBase.h"

#include "DataEntryString.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class SetParameters : public SPICE::BIG::CommandBase
		    {
			    public:
				    SetParameters();
				    virtual ~SetParameters();

				    void setSpecificSetParameters(std::shared_ptr<SPICE::BIG::SpecificBase> specificSetParameters);

				    virtual std::string getCommandName();
				    virtual std::string getCommandDescription();
				    virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();
				    virtual double calculateEstimatedDuration();
					virtual bool isCommonCommand();

				    virtual bool processing();
				    virtual bool simulationProcessing();
				
			    protected:
			    private:
					bool internalProcessing(bool simulationMode);

				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _paramsXML;
				    std::shared_ptr<SPICE::BIG::SpecificBase> _specificSetParameters;

		    };
		}
	}
}
#endif // SETPARAMETERS_H
