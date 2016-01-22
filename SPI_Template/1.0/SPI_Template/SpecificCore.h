/**
	SPI Template
	SpecificCore.h
	Purpose: Central class of a specific implemention. Usually used to communicate with the device. An instance is just deleted and new generated with the reset command. Every command can use it.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_22
*/

#ifndef SPECIFICCORE_H
#define SPECIFICCORE_H

#include <memory>
#include <queue>
#include <thread>

#include "IResourceProvider.h"
#include "ICommandCallback.h"
#include "GeneralFunctions.h"

namespace SPI
{
	namespace Template
	{
		class SpecificCore
		{
			public:

				SpecificCore(bool simulationMode, std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider);
				virtual ~SpecificCore();

				double calculateTimePrepareForReset();
				double calculateTimeInitialize();
				double calculateTimeSetParameters();
				double calculateTimeCommandMinimum();
				double calculateTimeCommandWithParameter(bool myBoolParameter, int myIntParameter, unsigned int myUnsingedIntParameter);
				double calculateTimeCommandWithResponseData();
				double calculateTimeCommandWithParallelExecution();
				double calculateTimeCommandWithSpecificSimulation();

				void prepareForReset(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool initialize(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool setParameters(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool commandMinimum(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool commandWithParameter(bool myBoolParameter, int myIntParameter, unsigned int myUnsingedIntParameter, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool commandWithResponseData(std::shared_ptr<SPICE::BIG::DataSet> responseDataSet, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool commandWithParallelExecution(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool commandWithSpecificSimulation(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);

				bool simulateCommandWithSpecificSimulation(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);

			protected:
			private:

				void startProcess();
				bool abortProcess();
				void pauseProcess();
				void continueProcess();
				bool processIsActive();

				bool _simulationMode;
				std::shared_ptr<SPICE::BIG::IResourceProvider> _resourceProvider;

		};
	}
}
#endif // SPECIFICCORE_H
