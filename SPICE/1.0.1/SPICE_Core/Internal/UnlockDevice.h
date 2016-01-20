/**
	SPICE Core
	UnlockDevice.h
	Purpose: Provides the functionality of the unlockDevice-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef UNLOCKDEVICE_H
#define UNLOCKDEVICE_H

#include "MandatoryCommandBase.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class UnlockDevice : public MandatoryCommandBase
		    {
			    public:
				    UnlockDevice();
				    virtual ~UnlockDevice();

				    virtual std::string getCommandName();
				    virtual std::string getCommandDescription();

				    virtual void additionalCommandCheck(int& returnCode, std::string& message);

				    virtual bool processing();

			    protected:
			    private:
		    };
		}
	}
}
#endif // UNLOCKDEVICE_H
