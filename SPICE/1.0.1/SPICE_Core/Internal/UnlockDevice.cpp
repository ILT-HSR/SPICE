/**
	SPICE Core
	UnlockDevice.h
	Purpose: Provides the functionality of the unlockDevice-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "UnlockDevice.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    UnlockDevice::UnlockDevice() :
			    MandatoryCommandBase()
		    {
		    }
		    UnlockDevice::~UnlockDevice()
		    {
		    }

		    std::string UnlockDevice::getCommandName()
		    {
			    return "UnlockDevice";
		    }
		    std::string UnlockDevice::getCommandDescription()
		    {
			    return "This command is used to unlock the device.";
		    }

		    void UnlockDevice::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state != SessionData::States::Standby && state != SessionData::States::ErrorHandling)
			    {
				    returnCode = 9;
				    message = "Command (UnlockDevice) not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Standby or ErrorHandling)";
			    }
			    else if(_mandatoryCommandCallback->getMandatoryCommandsCount() > 0)
			    {
				    returnCode = 9;
				    message = "Command (UnlockDevice) not allowed in this state (" + SessionData::stateToString(state) + ") (Other mandatory command active)";
			    }
		    }

		    bool UnlockDevice::processing()
		    {
			    _mandatoryCommandCallback->setPMSId("");
			    _mandatoryCommandCallback->unlock();

			    return true;
		    }
		}
	}
}