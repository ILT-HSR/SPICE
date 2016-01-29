/**
	SPICE Core
	Pause.h
	Purpose: Provides the functionality of the pause-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef PAUSE_H
#define PAUSE_H

#include "MandatoryCommandBase.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class Pause : public MandatoryCommandBase
		    {
			    public:
				    Pause();
				    virtual ~Pause();

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
#endif // PAUSE_H
