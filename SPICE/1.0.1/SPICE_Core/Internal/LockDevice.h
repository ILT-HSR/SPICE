/**
	SPICE Core
	LockDevice.h
	Purpose: Provides the functionality of the lockDevice-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef LOCKDEVICE_H
#define LOCKDEVICE_H

#include "MandatoryCommandBase.h"

#include "DataEntryString.h"
#include "DataEntryDuration.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class LockDevice : public MandatoryCommandBase
		    {
			    public:
				    LockDevice(std::string lockId);
				    virtual ~LockDevice();

				    virtual std::string getCommandName();
				    virtual std::string getCommandDescription();

				    virtual void additionalCommandCheck(int& returnCode, std::string& message);

				    virtual bool processing();

				    virtual std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> createAndGetAdditionalCommandParameters();

			    protected:
			    private:
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryDuration> _lockTimeout;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _eventReceiverURI;
				    std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> _pmsId;
					std::string _lockId;
		    };
		}
	}
}
#endif // LOCKDEVICE_H
