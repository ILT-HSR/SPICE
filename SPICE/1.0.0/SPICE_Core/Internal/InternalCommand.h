/**
	SPICE BIG
	InternalCommand.h
	Purpose: Class for core internal handling of a command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef INTERNALCOMMAND_H
#define INTERNALCOMMAND_H

#include <memory>
#include <thread>
#include <string>
#include <queue>

#include "IMandatoryCommandCallback.h"
#include "ICommandCallback.h"
#include "ICommandHandlerCallback.h"
#include "CommandBase.h"
#include "CommunicationInformations.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class InternalCommand : public SPICE::BIG::ICommandCallback, public IMandatoryCommandCallback
			{
					// Methoden
				public:
					enum SubStates
					{
						NotStarted,
						Processing,
						PauseRequested,
						AsynchPaused,
						ResponseWaiting,
						FinishedOk,
						FinishedWarning,
						FinishedAbort,
						FinishedError
					};

					/**
						Class for core internal handling of a command

						@param baseCommand CommandBase for this internalCommand
						@param requestId requestId of this command
						@param isMandatoryCommand Set to true, if command is a mandatory command
						@param commandHandlerCallback Command handler callback to call core functions
						@param communicationInformations CommunicationInformations needed for eventConnections
					*/
					InternalCommand(std::shared_ptr<SPICE::BIG::CommandBase> baseCommand, int requestId, bool isMandatoryCommand, std::shared_ptr<ICommandHandlerCallback> commandHandlerCallback, std::shared_ptr<CommunicationInformations> communicationInformations);
					virtual ~InternalCommand();

			// ----------- InternalCommand -------------
					/**
						Starts the new thread and the execution of the command

						@return 0 if thread could be started successful
					*/
					int start();
					/**
						Sets the pausing-flag, processing has to call enterPause
					*/
					void pause();
					/**
						Set the doingContinue-Flag. Entered pause will be left.

						@return True if subState is "asynchPaused" and the doingContinue-Flag could be set
					*/
					bool doContinue();
					/**
						Sets the aborting flag. Processing has to abort
					*/
					void abort();
					/**
						Sets the resetting flag. Processing has to reset
					*/
					void reset();
					/**
						Joins the thread of the command. Function returns after ending of the thread
					*/
					void joinThread();

					/**
						Gets the xml command description for the current command

						@param queuePosition The current position in the queue. 0 for active command
						@return	The requested XML command description
					*/
					std::string getXMLCommandDescription(int queuePosition);

					/**
						Returns if the command is a mandatory command

						@return True if command is a mandatory command
					*/
					bool isMandatoryCommand();
					/**
						Gets the requestId of the command

						@return requestId of the command
					*/
					int getRequestId();
					/**
						Gets the current subState of the command

						@return Current subState
					*/
					SubStates getSubState();

					/**
						Sets the value of the command parameter

						@param name Name of the command parameter
						@param value Value for the command parameter
						@return True if the command parameter with this name exists and the value is valid
					*/
					bool setCommandParameterValue(std::string name, std::string value);
					/**
						Checks if the command parameters are ok (all needed Parameters are set)

						@param errorMessage Error message with information about missing parameter
						@return True if all command parameters are ok. 
					*/
					bool checkCommandParametersOk(std::string& errorMessage);

					/**
						Gets information if command is ready to start

						@return True if ready to start
					*/
					bool readyForStart();
					/**
						Gets the name of the command

						@return Name of the command
					*/
					std::string getCommandName();
					/**
						Gets the contained base command

						@return CommandBase
					*/
					std::shared_ptr<SPICE::BIG::CommandBase> getBaseCommand();

			// ----------- Implementierung ICommandCallback ---------------
					/**
						For polling whether the command has to pause

						@return True when it has to enter the pause
					*/
					virtual bool hasToPause();
					/**
						To call after command is ready to enter pause. StateMachine gets the information that the pause state is reached. Returns if pause-state is left or reset is called.
					*/
					virtual void enterPause();
					/**
						For polling whether the command has to abort. 

						@return True when it has to abort.
					*/
					virtual bool hasToAbort();
					/**
						For polling whether the command has to reset. 

						@return True when it has to reset.
					*/
					virtual bool hasToReset();
					/**
						For polling whether the command has to abort or reset. 

						@return True when it has to abort or reset.
					*/
					virtual bool hasToAbortOrReset();

					/**
						To throw a generated error and maybe trigger the sending of an error event. 

						@param commandError Generated error which is to throw
						@return The selected continuation task name
					*/
					virtual std::string throwError(std::shared_ptr<SPICE::BIG::CommandError> commandError);

					/**
						Gets the count of waiting commands.

						@return count of waiting commands
					*/
					virtual unsigned int getWaitingCommandsCount();
					/**
						Gets the count of active commands (currently executing)

						@return count of active commands
					*/
					virtual unsigned int getActiveCommandsCount();
					/**
						Gets a map with the requestIds and commandNames of all active commands.

						@return Map with requestIds and commandNames of all active commands.
					*/
					virtual std::map<int, std::string> getActiveCommandsList();

					/**
						Provides the possibility to set content to the responseData in the responseEvent.

						@param responseEventData xml-formated string with content for the responseData
					*/
					virtual void setResponseEventData(std::string responseEventData);

					/**
						Gets the parameter of the state machine as DataSet.

						@return The state machine parameter as DataSet
					*/
					virtual std::shared_ptr<SPICE::BIG::DataSet> getParameterSet();
					/**
						Gets the configuration of a configuration level as DataSet.

						@param configLevel the configuration level which is requested
						@param password Password to get access to the configLevel. Default is an empty string.
						@return The requested configuration as DataSet. Returns null if configLevel not exists or a wrong password is given.
					*/
					virtual std::shared_ptr<SPICE::BIG::DataSet> getConfigurationSet(int configLevel, std::string password = "");

					/**
						Gets a new instance of the xml parser

						@return XMLParser
					*/
					virtual std::shared_ptr<SPICE::BIG::IXMLParser> getNewXMLParserInstance();

					/**
						Is called when a configurationSet has been changed. Possible safe-actions can be executed.

						@param configLevel The configuration level of the changed configurationSet
					*/
					virtual void configurationSetHasChanged(int configLevel);


			// --------------- Impelmentierung IMandatoryCommandCallback ------------
					/**
						Gets the current state of the state machine

						@return State of the state machine
					*/
					virtual SessionData::States getCurrentState();
					/**
						Sets the given state as new state of the state machine

						@param state New state for the state machine
					*/
					virtual void setCurrentState(SessionData::States state);
					/**
						Sets the given state when the response of the command is send (short before)

						@param state New state for the state machine
					*/
					virtual void setStateWithResponse(SessionData::States state);

					/**
						Function to pause all active commands.

						@return True if all active commands reached subState "asynchPaused"
					*/
					virtual bool pauseAll();
					/**
						Function to continue all paused commands.
					*/
					virtual void continueAll();
					/**
						Function to abort all active commands.

						@return True if all commands are aborted
					*/
					virtual bool abortAll();
					/**
						Function ro reset all active commands.

						@return True if all commands are resetted
					*/
					virtual bool resetAll();

					/**
						Gets the count of all active commands (mandatory + other active commands)

						@return Count of all active commands
					*/
					virtual unsigned int getTotalActiveCommandsCount();
					/**
						Gets the count of mandatory commands. 0 or 1. 2 is only possible if a reset is called during an other mandatory command is active.

						@return Count of mandatory commands
					*/
					virtual unsigned int getMandatoryCommandsCount();

					/**
						Locks the service provider for the calling PMS. Used by command "LockDevice"

						@param lockId Id which is used to lock the device and other commands will be checked with.
						@param lockTimeout Timeout in seconds until device is unlocked automatically. If 0 there won't be an automatic unlock.
					*/
					virtual void lock(std::string lockId, double lockTimeout);
					/**
						Unlocks the service provider. Used by command "UnlockDevice"
					*/
					virtual void unlock();
					/**
						Sets the eventReceiverURI for the current session

						@param eventReceiverURI URI of the service consumer.
					*/
					virtual void setEventReceiverURI(std::string eventReceiverURI);
					/**
						Sets the id of the process management system.

						@param pmsId Id of the PMS
					*/
					virtual void setPMSId(std::string pmsId);
					/**
						Sets the service provider into the simulation mode. Used by command "Reset"

						@param simulationMode True sets simulation mode active
					*/
					virtual void setSimulationMode(bool simulationMode);
					/**
						Sets the id of the device to be able to identify at the service consumer. Used by command "Reset"

						@param deviceId ID of the device
					*/
					virtual void setDeviceId(std::string deviceId);
					/**
						Sets the timeout until an errorhandling state is changed into an error state.

						@param timeoutSeconds timeout in seconds
					*/
					virtual void setErrorHandlingTimeout(double timeoutSeconds);

			// ------------ InternalCommand -------------------
				private:
					/**
						Basic execution of a command. Will call the processing function of the base command.
					*/
					void basicProcessing();
					/**
						Internal function to wait on a response. Returns if no eventConnection is waiting anymore or resetting is called.

						@return ErrorDetected: True if an error occured during one or more eventConnections
					*/
					bool responseWaiting();
					/**
						Creates the responseEvent

						@param returnCode The returnCode for the command
						@param message Human readable message according to the returnCode
						@param responseData The responseData which can be set by the base command
						@return The created XML responseEvent
					*/
					std::string getXMLResponseEvent(int returnCode, std::string message, std::string responseData);
					/**
						Creates the errorEvent

						@param returnCode The returnCode for the command
						@param message Human readable message according to the returnCode
						@param continuationTask The continuation task xml file which is generated by the error
						@return The created XML errorEvent
					*/
					std::string getXMLErrorEvent(int returnCode, std::string message, std::string continuationTask);
					/**
						Function to send an event. Creates a new eventConnection.

						@param xmlEvent The xml event content
						@param eventType The event type
					*/
					void sendEvent(std::string xmlEvent, std::string eventType);

					/**
						Sets the current subState of the command

						@param newSubState New subState
					*/
					void setSubState(SubStates newSubState);

					// Daten
				public:
				protected:
				private:
					std::shared_ptr<ICommandHandlerCallback> _commandHandlerCallback;
					std::atomic<bool> _aborting;
					std::atomic<bool> _resetting;
					std::atomic<bool> _pausing;
					std::atomic<bool> _doingContinue;
					std::atomic<int> _requestId;
					std::atomic<bool> _isMandatoryCommand;
					SubStates _subState;

					std::mutex _commandMutex;
					std::thread* _processThread;

					std::shared_ptr<SPICE::BIG::CommandBase> _baseCommand;

					time_t _startedTime;
					std::atomic<int> _dataWaitingCount;

					bool _setStateWithResponse;
					SessionData::States _setStateWithResponseNewState;

					std::queue<std::shared_ptr<SPICE::BIG::CommandError>> _errorQueue;
					std::queue<std::shared_ptr<SPICE::BIG::IEthernetEventConnection>> _eventConnectionQueue;

					std::map<std::string, std::shared_ptr<SPICE::BIG::DataEntry>> _commandParametersMap;

					std::string _responseEventData;
					std::shared_ptr<CommunicationInformations> _communicationInformations;

			};
		}
	}
}
#endif // INTERNALCOMMAND_H
