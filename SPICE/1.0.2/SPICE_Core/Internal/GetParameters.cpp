/**
	SPICE Core
	GetParameters.cpp
	Purpose: Provides the functionality of the getParameters-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "GetParameters.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    GetParameters::GetParameters()
		    {
		    }
		    GetParameters::~GetParameters()
		    {
		    }

		    std::string GetParameters::getCommandName()
		    {
			    return "GetParameters";
		    }
		    std::string GetParameters::getCommandDescription()
		    {
			    return "This command is used to retrieve the available parameter values of the device.";
		    }

		    double GetParameters::calculateEstimatedDuration()
		    {
			    return 1;
		    }
            bool GetParameters::isCommonCommand()
			{
                return true;
			}

		    bool GetParameters::processing()
		    {
			    _commandCallback->setResponseEventData(_commandCallback->getParameterSet()->getXMLParameterSet());
			    return true;
		    }

		    bool GetParameters::simulationProcessing()
		    {
			    return processing();
		    }
		}
	}
}