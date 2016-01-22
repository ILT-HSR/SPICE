/**
	SPI Template
	CommandWithParameter.cpp
	Purpose: This command shows the recommended implementation of a command with parameters

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#include "CommandWithParameter.h"

namespace SPI
{
	namespace Template
	{
		CommandWithParameter::CommandWithParameter(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore),
			_myBoolParameter(nullptr),
			_myIntParameter(nullptr),
			_myUnsignedIntParameter(nullptr)
		{
		}
		CommandWithParameter::~CommandWithParameter()
		{
		}

		std::string CommandWithParameter::getCommandName()
		{
			return "CommandWithParameter";
		}
		std::string CommandWithParameter::getCommandDescription()
		{
			return "This command shows the recommended implementation of a command with parameters";
		}
		bool CommandWithParameter::isCommonCommand()
		{
			return false;
		}
		std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> CommandWithParameter::createAndGetAdditionalCommandParameters()
		{
			// Create the parameters
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryBool> myBoolParameter(new SPICE::BIG::DataEntryTypes::DataEntryBool("myBoolParameter"));
			myBoolParameter->setAdditionalInformations("This parameter provides a bool value.", "","true");
			_myBoolParameter = myBoolParameter;
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> myIntParameter(new SPICE::BIG::DataEntryTypes::DataEntryInt("myIntParameter", -100, 100));
			myIntParameter->setAdditionalInformations("This parameter provides a int value like a speed", "mm/s","5");
			_myIntParameter = myIntParameter;
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt> myUnsignedIntParameter(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt("myUnsignedIntParameter", 0, 100));
			myUnsignedIntParameter->setAdditionalInformations("This parameter provides an uint value like a position", "mm","0");
			_myUnsignedIntParameter = myUnsignedIntParameter;

			// Set, if they are required or optional
			_myBoolParameter->setIsNeeded(true);
			_myIntParameter->setIsNeeded(true);
			_myUnsignedIntParameter->setIsNeeded(true);

			// Create a vector and add the parameters
			std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
			returnVector.push_back(_myBoolParameter);
			returnVector.push_back(_myIntParameter);
			returnVector.push_back(_myUnsignedIntParameter);

			return returnVector;
		}

		double CommandWithParameter::calculateEstimatedDuration()
		{
			if(_myBoolParameter->getIsSet() && _myIntParameter->getIsSet() && _myUnsignedIntParameter->getIsSet())
			{
				return _specificCore->calculateTimeCommandWithParameter(_myBoolParameter->getValue(), _myIntParameter->getValue(), _myUnsignedIntParameter->getValue());
			}
			return 0;
		}

		void CommandWithParameter::additionalCommandCheck(int& returnCode, std::string& message)
		{
			// optional possibility to have additional command checks (additional to limits and required-definition)
			// if no problem is detected, returnCode and message should not be changed.
			// if a problem occour, returnCode and message should be set for the sync-answer

			// examples:
			// checking that a parameter is a multiple of 5
			if(_myUnsignedIntParameter->getValue() % 5 != 0)
			{
				returnCode = 11;
				message = "Invalid command parameter: myUnsignedIntParameter has to be a multiple of 5";
				return;
			}
			// checking parameter dependecies -> here the parameters should not be equal
			if(_myIntParameter->getValueAsString() == _myUnsignedIntParameter->getValueAsString())
			{
				returnCode = 11;
				message = "Invalid command parameter: myIntParameter and myUnsignedIntParameter must not be equal";
			}
		}

		bool CommandWithParameter::processing()
		{
			return _specificCore->commandWithParameter(_myBoolParameter->getValue(), _myIntParameter->getValue(), _myUnsignedIntParameter->getValue(), _commandCallback);
		}
	}
}