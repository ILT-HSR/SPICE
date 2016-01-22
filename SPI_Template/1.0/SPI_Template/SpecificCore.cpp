/**
	SPI Template
	SpecificCore.cpp
	Purpose: Central class of a specific implemention. Usually used to communicate with the device. An instance is just deleted and new generated with the reset command. Every command can use it.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_22
*/

#include "SpecificCore.h"

namespace SPI
{
	namespace Template
	{
		SpecificCore::SpecificCore(bool simulationMode, std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider) :
			_simulationMode(simulationMode),
			_resourceProvider(resourceProvider)
		{
		}
		SpecificCore::~SpecificCore()
		{
		}

		// Implementations of the expected time calculations.
		// They can be fix or calculated depending on parameters and configurations.
		// If no time can be calculated, return 0;
		double SpecificCore::calculateTimePrepareForReset()
		{
			return 0;
		}
		double SpecificCore::calculateTimeInitialize()
		{
			return 5;
		}
		double SpecificCore::calculateTimeSetParameters()
		{
			return 0;
		}
		double SpecificCore::calculateTimeCommandMinimum()
		{
			return 10;
		}
		double SpecificCore::calculateTimeCommandWithParameter(bool myBoolParameter, int myIntParameter, unsigned int myUnsingedIntParameter)
		{
			if(myBoolParameter)
			{
				return 9;
			}
			return 3;
		}
		double SpecificCore::calculateTimeCommandWithResponseData()
		{
			return 8;
		}
		double SpecificCore::calculateTimeCommandWithParallelExecution()
		{
			return 8;
		}
		double SpecificCore::calculateTimeCommandWithSpecificSimulation()
		{
			return 8;
		}

		// Implementation of the functions
		// Use commandCallback to check cyclic if a command should pause, abort or reset
		// Pause -> reach stable state and call commanCallback-Function enterPause
		// Abort -> abort immediate and try to reach a stable state.
		// Reset -> do nothing else, try direct to return. Actions should be done withing the prepareForReset-Function
		// The bool returnValue stands for success execution. Return false if their is a not recoverable error.
		void SpecificCore::prepareForReset(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// if any running functions, bring them to end

			// if any open connections -> close them.

			// after prepareForReset the SpecificCore-instance will be deleted and a new created.

			// instead to all other commands: no checks for reset, abort and pause needed.
		}
		bool SpecificCore::initialize(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// if any connection needed -> open them during the initialize

			// do configurations to the device if required

			// do any initalization steps at the device if needed.

			// instead to the normal commands: only checks for reset needed. not for abort and pause.

			return true;
		}
		bool SpecificCore::setParameters(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// if any changes dependent to the parameterSet should be written to the device, do it here.

			return true;
		}
		bool SpecificCore::commandMinimum(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// --- example: typical working cycle ---
			// start process here
			startProcess();
			bool active = true;
			while(active)
			{
				// wait on end of process and check for interupts from the ServiceConsumer
				if(!processIsActive())
				{
					active = false;
				}
				else if(commandCallback->hasToReset())
				{
					return false;
				}
				else if(commandCallback->hasToAbort())
				{
					return abortProcess();
				}
				else if(commandCallback->hasToPause())
				{
					pauseProcess();
					commandCallback->enterPause();
					continueProcess();
				}
				else
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
				}
			}

			// --- example: how to get a value from the parameters or configurations ---
			bool boolParameter = _resourceProvider->getParameterSet()->getFirstDataEntryByName("BoolParameter")->getValueAsString() == "true";
			int intParameter = std::stoi(_resourceProvider->getParameterSet()->getFirstDataEntryByName("IntParameter")->getValueAsString());
			unsigned int uintParameter = (unsigned int)std::stoul(_resourceProvider->getParameterSet()->getFirstDataEntryByName("UnsignedIntParameter")->getValueAsString());

			// --- example: how to throw errors
			// -> fatal error
			{
				std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("the error message", "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
				commandCallback->throwError(error);
				return false;
			}

			// -> error with selection, the given continuation task at the CommandError-constructor is the default continuation task
			{
				std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("the error message", "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
				error->addContinuationTask("AbortAll", "Aborts this command and empty the command buffer", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
				error->addContinuationTask("AbortThis", "Aborts / Ends this command", SPICE::BIG::ContinuationTask::TaskTypes::abortCurrentCommand);
				error->addContinuationTask("Retry", "Tries to continue the command", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand);
				commandCallback->throwError(error);
				if(error->getSelectedTaskType() == SPICE::BIG::ContinuationTask::TaskTypes::continueCommand)
				{
					// do something to acknowledge the command and continue
				}
				else
				{
					// do something else, like things to abort
					return false;
				}
			}

			return true;
		}
		bool SpecificCore::commandWithParameter(bool myBoolParameter, int myIntParameter, unsigned int myUnsingedIntParameter, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// nothing special here.
			return true;
		}
		bool SpecificCore::commandWithResponseData(std::shared_ptr<SPICE::BIG::DataSet> responseDataSet, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// --- example how to set responseData ---
			responseDataSet->getFirstDataEntryByName("myResponseBool")->setValueString("true");
			responseDataSet->getFirstDataEntryByName("myResponseInt")->setValueString("-15");

			if(!responseDataSet->getFirstDataEntryByName("myResponseUnsignedInt")->setValueString("15"))
			{
				// possibility to react on invalid data or data outside the limits
			}

			return true;
		}
		bool SpecificCore::commandWithParallelExecution(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// nothing special here. Just take care about situations with race contitions.
			return true;
		}
		bool SpecificCore::commandWithSpecificSimulation(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// nothing special here
			return true;
		}

		bool SpecificCore::simulateCommandWithSpecificSimulation(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			// here you could implement the simulation behaviour for this command (CommandWithSpecificSimulation)
			// if you don't implement it (here and in the command), default behaviour is: the expected execution time will be calculated
			// and it will wait this time (also with the required checking for reset / abort / pause)

			return true;
		}

		// --- this functions are just helper, to be able to show a typical working cycle ---
		void SpecificCore::startProcess()
		{
		}
		bool SpecificCore::abortProcess()
		{
			return true;
		}
		void SpecificCore::pauseProcess()
		{
		}
		void SpecificCore::continueProcess()
		{
		}
		bool SpecificCore::processIsActive()
		{
			return false;
		}

	}
}