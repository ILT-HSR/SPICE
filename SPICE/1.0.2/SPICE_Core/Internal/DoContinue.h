/**
	SPICE Core
	DoContinue.h
	Purpose: Provides the functionality of the doContinue-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef DOCONTINUE_H
#define DOCONTINUE_H

#include "MandatoryCommandBase.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class DoContinue : public MandatoryCommandBase
		    {
			    public:
				    DoContinue();
				    virtual ~DoContinue();

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
#endif // DOCONTINUE_H
