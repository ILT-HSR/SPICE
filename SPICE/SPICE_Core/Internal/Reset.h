/**
	SPICE Core
	Reset.h
	Purpose: Provides the functionality of the reset-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef RESET_H
#define RESET_H

#include "MandatoryCommandBase.h"
#include "SpecificResetBase.h"

#include "DataEntryString.h"
#include "DataEntryDuration.h"
#include "DataEntryBool.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class Reset : public MandatoryCommandBase
		    {
			    public:
				    Reset();
				    virtual ~Reset();

				    void setSpecificReset(std::shared_ptr<SPICE::BIG::SpecificResetBase> specificReset);

				    virtual std::string getCommandName();
				    virtual std::string getCommandDescription();

				    virtual void additionalCommandCheck(int& returnCode, std::string& message);

				    virtual double calculateEstimatedDuration();
				    virtual bool processing();
				    virtual bool simulationProcessing();

				    virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();

			    protected:
			    private:
				    bool internalProcessing(bool wasSimulationMode);
				    std::shared_ptr<SPICE::BIG::SpecificResetBase> _specificReset;

				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _deviceId;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _eventReceiverURI;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _pmsId;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryDuration> _errorHandlingTimeout;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryBool> _simulationMode;
		    };
		}
	}
}
#endif // RESET_H
