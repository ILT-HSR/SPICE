/**
	SPICE Core
	GetParameters.h
	Purpose: Provides the functionality of the getParameters-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef GETPARAMETERS_H
#define GETPARAMETERS_H

#include "CommandBase.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class GetParameters : public SPICE::BIG::CommandBase
		    {
			    public:
				    GetParameters();
				    virtual ~GetParameters();

				    virtual std::string getCommandName();
				    virtual std::string getCommandDescription();
				    virtual double calculateEstimatedDuration();
					virtual bool isCommonCommand();

				    virtual bool processing();
				    virtual bool simulationProcessing();
				
			    protected:
			    private:

		    };
		}
	}
}
#endif // GETPARAMETERS_H
