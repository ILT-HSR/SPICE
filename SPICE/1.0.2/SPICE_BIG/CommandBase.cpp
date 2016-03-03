/**
	SPICE BIG
	CommandBase.cpp
	Purpose: Abstract base class for implementation specific commands

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "CommandBase.h"

#include <thread>

namespace SPICE
{
	namespace BIG
	{
		CommandBase::CommandBase() :
			_commandCallback(nullptr)
		{
		}
		CommandBase::~CommandBase()
		{
		}

		void CommandBase::setCommandCallback(std::shared_ptr<ICommandCallback> commandCallback)
		{
			_commandCallback = commandCallback;
		}

		std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> CommandBase::createAndGetAdditionalCommandParameters()
		{
			std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
			return returnVector;
		}
		std::vector<std::shared_ptr<SPICE::BIG::DataSet>> CommandBase::getResponseDataInformation(std::string& responseDescription)
		{
			responseDescription = "Empty response";
			std::vector<std::shared_ptr<SPICE::BIG::DataSet>> returnVector;
			return returnVector;
		}
	
		void CommandBase::additionalCommandCheck(int& returnCode, std::string& message)
		{
		}

		bool CommandBase::readyForStart()
		{
			return (_commandCallback->getActiveCommandsCount() == 0);
		}
		bool CommandBase::simulationProcessing()
		{
			long neededCycles = (long)(calculateEstimatedDuration() * 50);

			while(neededCycles > 0)
			{
				neededCycles -= 1;
				std::this_thread::sleep_for(std::chrono::milliseconds(20));

				if(_commandCallback->hasToAbortOrReset())
				{
					neededCycles = -1;
				}
				if(_commandCallback->hasToPause())
				{
					_commandCallback->enterPause();
				}
			}
			return !_commandCallback->hasToReset();
		}
	}
}