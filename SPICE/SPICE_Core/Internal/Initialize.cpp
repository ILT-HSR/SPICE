/**
	SPICE Core
	Initialize.cpp
	Purpose: Provides the functionality of the initialize-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "Initialize.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    Initialize::Initialize() :
			    MandatoryCommandBase(),
			    _specificInit(nullptr)
		    {
		    }
		    Initialize::~Initialize()
		    {
		    }

		    void Initialize::setSpecificInit(std::shared_ptr<SPICE::BIG::SpecificBase> specificInit)
		    {
			    _specificInit = specificInit;
		    }

		    std::string Initialize::getCommandName()
		    {
			    return "Initialize";
		    }
		    std::string Initialize::getCommandDescription()
		    {
			    return "This command initializes the device.";
		    }

		    void Initialize::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state != SessionData::States::Standby)
			    {
				    returnCode = 9;
				    message = "Command (Initialize) not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Standby)";
			    }
			    else if(_mandatoryCommandCallback->getMandatoryCommandsCount() > 0)
			    {
				    returnCode = 9;
				    message = "Command (Initialize) not allowed in this state (" + SessionData::stateToString(state) + ") (Other mandatory command active)";
			    }
		    }

		    double Initialize::calculateEstimatedDuration()
		    {
			    return _specificInit->calculateEstimatedDuration();
		    }
		    bool Initialize::processing()
		    {
			    return internalProcessing(false);
		    }
		    bool Initialize::simulationProcessing()
		    {
			    return internalProcessing(true);
		    }
		    bool Initialize::internalProcessing(bool simulationMode)
		    {
			    _specificInit->setCommandCallback(_commandCallback);
			    _mandatoryCommandCallback->setCurrentState(SessionData::States::Initializing);

			    bool returnValue = false;
			    if(!simulationMode)
			    {
				    returnValue = _specificInit->processing();
			    }
			    else
			    {
				    returnValue = _specificInit->simulationProcessing();
			    }

			    if(returnValue)
			    {
				    if(!_commandCallback->hasToReset())
				    {
                        _mandatoryCommandCallback->setStateWithResponse(SessionData::States::Idle);
				    }
			    }

			    _specificInit->setCommandCallback(nullptr);
			    return returnValue;
		    }
		}
	}
}