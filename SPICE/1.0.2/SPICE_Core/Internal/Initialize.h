/**
	SPICE Core
	Initialize.h
	Purpose: Provides the functionality of the initialize-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef INITIALIZE_H
#define INITIALIZE_H

#include "MandatoryCommandBase.h"
#include "SpecificBase.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    class Initialize : public MandatoryCommandBase
		    {
			    public:
				    Initialize();
				    virtual ~Initialize();

				    void setSpecificInit(std::shared_ptr<SPICE::BIG::SpecificBase> specificInit);

				    virtual std::string getCommandName();
				    virtual std::string getCommandDescription();

				    virtual void additionalCommandCheck(int& returnCode, std::string& message);

				    virtual double calculateEstimatedDuration();
				    virtual bool processing();
				    virtual bool simulationProcessing();

			    protected:
			    private:
				    bool internalProcessing(bool simulationMode);
				    std::shared_ptr<SPICE::BIG::SpecificBase> _specificInit;

		    };
		}
	}
}
#endif // INITIALIZE_H
