/**
	SPICE Core
	Reset.cpp
	Purpose: Provides the functionality of the reset-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "Reset.h"

#include <thread>

namespace SPICE
{
    namespace Core
	{
	    namespace Internal
	    {
		    Reset::Reset() :
			    MandatoryCommandBase(),
			    _specificReset(nullptr),
			    _deviceId(nullptr),
			    _eventReceiverURI(nullptr),
			    _pmsId(nullptr),
			    _errorHandlingTimeout(nullptr),
			    _simulationMode(nullptr)
		    {
		    }
		    Reset::~Reset()
		    {
		    }

		    void Reset::setSpecificReset(std::shared_ptr<SPICE::BIG::SpecificResetBase> specificReset)
		    {
			    _specificReset = specificReset;
		    }

		    std::string Reset::getCommandName()
		    {
			    return "Reset";
		    }
		    std::string Reset::getCommandDescription()
		    {
			    return "This command is used to reset the Service Provider at any time from any state.";
		    }

		    void Reset::additionalCommandCheck(int& returnCode, std::string& message)
		    {
			    SessionData::States state = _mandatoryCommandCallback->getCurrentState();
			    if(state == SessionData::States::Resetting)
			    {
				    returnCode = 9;
				    message = "Command (Reset) not allowed in this state (" + SessionData::stateToString(state) + ")";
			    }
		    }

		    double Reset::calculateEstimatedDuration()
		    {
			    return _specificReset->calculateEstimatedDuration();
		    }
		    bool Reset::processing()
		    {
			    return internalProcessing(false);
		    }

		    bool Reset::simulationProcessing()
		    {
			    return internalProcessing(true);
		    }

		    bool Reset::internalProcessing(bool wasSimulationMode)
		    {
			    _specificReset->setCommandCallback(_commandCallback);
			    _mandatoryCommandCallback->setCurrentState(SessionData::States::Resetting);

			    // 1. Reset all commands
			    _mandatoryCommandCallback->resetAll();

			    bool waitingUntilResetted = true;
			    while (waitingUntilResetted)
			    {
				    if(_mandatoryCommandCallback->getTotalActiveCommandsCount() <= 1)
				    {
					    waitingUntilResetted = false;
				    }
				    else
				    {
					    std::this_thread::sleep_for(std::chrono::milliseconds(50));
				    }
			    }

			    _mandatoryCommandCallback->setSimulationMode(_simulationMode->getValue());

			    // 2. Reset device-specific
			    bool returnValue = _specificReset->processing(wasSimulationMode, _simulationMode->getValue());

			    // 3. Reset/Set Data-Informations
			    _mandatoryCommandCallback->unlock();
			    _mandatoryCommandCallback->setDeviceId(_deviceId->getValueAsString());
			    _mandatoryCommandCallback->setPMSId(_pmsId->getValueAsString());
			    _mandatoryCommandCallback->setEventReceiverURI(_eventReceiverURI->getValueAsString());
			    _mandatoryCommandCallback->setErrorHandlingTimeout(_errorHandlingTimeout->getValue());

                _mandatoryCommandCallback->setStateWithResponse(SessionData::States::Standby);

			    _specificReset->setCommandCallback(nullptr);
			    return returnValue;
		    }

		    std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> Reset::createAndGetAdditionalCommandParameters()
		    {
                std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> deviceId(new SPICE::BIG::DataEntryTypes::DataEntryString("deviceId", true));
                deviceId->setAdditionalInformations("The identification the device returns to identify itself at the PMS.","","");
                deviceId->setIsNeeded(true);

				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> eventReceiverURI(new SPICE::BIG::DataEntryTypes::DataEntryString("eventReceiverURI", false));
                eventReceiverURI->setAdditionalInformations("Connection information of the Service Consumers event Receiver.", "", "");
                eventReceiverURI->setIsNeeded(true);

				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryString> pmsId(new SPICE::BIG::DataEntryTypes::DataEntryString("PMSId", false));
                pmsId->setAdditionalInformations("Id of the PMS in order to identify the PMS that locked a device.", "", "");
                pmsId->setIsNeeded(true);

   				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryDuration> errorHandlingTimeout(new SPICE::BIG::DataEntryTypes::DataEntryDuration("errorHandlingTimeout"));
                errorHandlingTimeout->setAdditionalInformations("Timeout until an errorhandling state is changed into an error state.", "", "");
                errorHandlingTimeout->setIsNeeded(true);

				std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryBool> simulationMode(new SPICE::BIG::DataEntryTypes::DataEntryBool("simulationMode"));
                simulationMode->setAdditionalInformations("Selects simulation mode.", "", "");
                simulationMode->setIsNeeded(true);

			    _deviceId = deviceId;
			    _eventReceiverURI = eventReceiverURI;
			    _pmsId = pmsId;
			    _errorHandlingTimeout = errorHandlingTimeout;
			    _simulationMode = simulationMode;

				std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
				returnVector.push_back(deviceId);
				returnVector.push_back(eventReceiverURI);
				returnVector.push_back(pmsId);
				returnVector.push_back(errorHandlingTimeout);
				returnVector.push_back(simulationMode);
				return returnVector;
		    }
        }
	}
}
