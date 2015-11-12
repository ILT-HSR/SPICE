/**
	SPICE Core
	Abort.cpp
	Purpose: Provides the functionality of the abort-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "Abort.h"

#include <thread>

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    Abort::Abort() :
			    MandatoryCommandBase()
		    {
		    }
		    Abort::~Abort()
		    {
		    }

		    std::string Abort::getCommandName()
		    {
			    return "Abort";
		    }
		    std::string Abort::getCommandDescription()
		    {
			    return "This command aborts all running and pending asynchronous commands of the device.";
		    }

		    void Abort::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state != SessionData::States::Busy)
			    {
				    returnCode = 9;
				    message = "Command (Abort) not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Busy)";
			    }
				else if(_mandatoryCommandCallback->getMandatoryCommandsCount() > 0)
			    {
				    returnCode = 9;
				    message = "Command (Abort) not allowed in this state (" + SessionData::stateToString(state) + ") (Other mandatory command active)";
			    }
		    }

		    bool Abort::processing()
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state == SessionData::States::Busy)
			    {
				    bool finishedAborting = false;
				    while (!finishedAborting && !_commandCallback->hasToReset())
				    {
					    finishedAborting = _mandatoryCommandCallback->abortAll();

					    std::this_thread::sleep_for(std::chrono::milliseconds(50));
				    }
			    }
			    return true;
		    }
		}
	}
}