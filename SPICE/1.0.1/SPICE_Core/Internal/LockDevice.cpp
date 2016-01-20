/**
	SPICE Core
	LockDevice.cpp
	Purpose: Provides the functionality of the lockDevice-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "LockDevice.h"

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    LockDevice::LockDevice(std::string lockId) :
			    MandatoryCommandBase(),
			    _lockTimeout(nullptr),
			    _eventReceiverURI(nullptr),
			    _pmsId(nullptr),
				_lockId(lockId)
		    {
		    }
		    LockDevice::~LockDevice()
		    {
		    }

		    std::string LockDevice::getCommandName()
		    {
			    return "LockDevice";
		    }
		    std::string LockDevice::getCommandDescription()
		    {
			    return "This command locks the device for exclusive use.";
		    }

		    void LockDevice::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state != SessionData::States::Standby && state != SessionData::States::ErrorHandling)
			    {
				    returnCode = 9;
				    message = "Command (LockDevice) not allowed in this state (" + SessionData::stateToString(state) + ") (Expected: Standby or  ErrorHandling)";
			    }
			    else if(_mandatoryCommandCallback->getMandatoryCommandsCount() > 0)
			    {
				    returnCode = 9;
				    message = "Command (LockDevice) not allowed in this state (" + SessionData::stateToString(state) + ") (Other mandatory command active)";
			    }
		    }

		    bool LockDevice::processing()
		    {
                if(_lockTimeout->getIsSet())
				{
    			    _mandatoryCommandCallback->lock(_lockId, _lockTimeout->getValue());
				}
				else
				{
                    _mandatoryCommandCallback->lock(_lockId, 0);
				}

			    _mandatoryCommandCallback->setEventReceiverURI(_eventReceiverURI->getValueAsString());

				if(_pmsId->getIsSet())
				{
					_mandatoryCommandCallback->setPMSId(_pmsId->getValueAsString());
				}

			    return true;
		    }

		    std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> LockDevice::createAndGetAdditionalCommandParameters()
		    {
                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryDuration> lockTimeout(new SPICE::BIG::DataEntryTypes::DataEntryDuration("lockTimeout"));
                lockTimeout->setAdditionalInformations("If this parameter is omitted, no timeout will be set. Otherwise the device will unlock itself if it does not receive any commands within the timeout period.","","");
                lockTimeout->setIsNeeded(false);

				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> eventReceiverURI(new SPICE::BIG::DataEntryTypes::DataEntryString("eventReceiverURI", false));
                eventReceiverURI->setAdditionalInformations("Service URI of the Service Consumer's event Receiver (used in case of timeout that will issue a reset with it).", "", "");
                eventReceiverURI->setIsNeeded(true);

				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> pmsId(new SPICE::BIG::DataEntryTypes::DataEntryString("pmsId", false));
                pmsId->setAdditionalInformations("Id of the PMS in order to identify the PMS that locked a device.", "", "");
                pmsId->setIsNeeded(false);

			    _lockTimeout = lockTimeout;
			    _eventReceiverURI = eventReceiverURI;
			    _pmsId = pmsId;

				std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
				returnVector.push_back(lockTimeout);
				returnVector.push_back(eventReceiverURI);
				returnVector.push_back(pmsId);
				return returnVector;
		    }
		}		
	}
}