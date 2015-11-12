/**
	SPICE Core
	Pause.cpp
	Purpose: Provides the functionality of the pause-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "Pause.h"

#include <thread>

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    Pause::Pause() :
			    MandatoryCommandBase()
		    {
		    }
		    Pause::~Pause()
		    {
		    }

		    std::string Pause::getCommandName()
		    {
			    return "Pause";
		    }
		    std::string Pause::getCommandDescription()
		    {
			    return "This command pauses the process/workflow in order to enable user intervention.";
		    }

		    void Pause::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state != SessionData::States::Busy && state != SessionData::States::Idle)
			    {
				    returnCode = 9;
				    message = "Command (Pause) not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Busy or Idle)";
			    }
			    else if(_mandatoryCommandCallback->getMandatoryCommandsCount() > 0)
			    {
				    returnCode = 9;
				    message = "Command (Pause) not allowed in this state (" + SessionData::stateToString(state) + ") (Other mandatory command active)";
			    }
		    }

		    bool Pause::processing()
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state == SessionData::States::Busy)
			    {
				    bool finishedPausing = false;
				    while (!finishedPausing)
				    {
					    finishedPausing = _mandatoryCommandCallback->pauseAll();

					    if(_commandCallback->hasToReset())
					    {
						    return true;
					    }

						std::this_thread::sleep_for(std::chrono::milliseconds(50));
				    }
				    _mandatoryCommandCallback->setCurrentState(SessionData::States::Paused);
			    }
			    else if(state == SessionData::States::Idle)
			    {
				    _mandatoryCommandCallback->setCurrentState(SessionData::States::Paused);
			    }
			    else
			    {
                    std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Pause: Wrong state for pausing", "EndCommand", "End command with warning", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
				    _commandCallback->throwError(error);
				    return false;
			    }
			    return true;
		    }
		}
	}
}