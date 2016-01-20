/**
	SPICE Core
	DoContinue.cpp
	Purpose: Provides the functionality of the doContinue-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DoContinue.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    DoContinue::DoContinue() :
			    MandatoryCommandBase()
		    {
		    }
		    DoContinue::~DoContinue()
		    {
		    }

		    std::string DoContinue::getCommandName()
		    {
			    return "DoContinue";
		    }
		    std::string DoContinue::getCommandDescription()
		    {
			    return "This command enables the continuation of the process for a paused system.";
		    }

		    void DoContinue::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state != SessionData::States::Paused)
			    {
				    returnCode = 9;
				    message = "Command (DoContinue) not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Paused)";
			    }
			    else if(_mandatoryCommandCallback->getMandatoryCommandsCount() > 0)
			    {
				    returnCode = 9;
				    message = "Command (DoContinue) not allowed in this state (" + SessionData::stateToString(state) + ") (Other mandatory command active)";
			    }
		    }

		    bool DoContinue::processing()
		    {
			    if(_commandCallback->getActiveCommandsCount() > 0)
			    {
				    _mandatoryCommandCallback->setCurrentState(SessionData::States::Busy);
			    }
			    else
			    {
				    _mandatoryCommandCallback->setCurrentState(SessionData::States::Idle);
			    }
			    _mandatoryCommandCallback->continueAll();
			    return true;
		    }
		}
	}
}