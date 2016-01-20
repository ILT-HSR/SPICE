/**
	SPI Multidrop
	Shake.cpp
	Purpose: Implementation of the shake command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "Shake.h"

namespace SPI
{
	namespace Multidrop
	{
		Shake::Shake(std::shared_ptr<SpecificCore> specificCore, std::shared_ptr<SPICE::BIG::DataSet> parameterSet) :
			SpecificCommandBase(specificCore),
			_shakeDuration(nullptr),
			_parameterSet(parameterSet)
		{
		}
		Shake::~Shake()
		{
		}

		std::string Shake::getCommandName()
		{
			return "Shake";
		}
		std::string Shake::getCommandDescription()
		{
			return "This command is used to shake a labware item.";
		}

		std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> Shake::createAndGetAdditionalCommandParameters()
		{
			std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
			case SpecificCore::DeviceType::MultidropCombiNL :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryDuration> shakeDuration(new SPICE::BIG::DataEntryTypes::DataEntryDuration("shakeDuration"));
					shakeDuration->setAdditionalInformations("This parameter defines the shaking duration. (Range: 1..300s, PT1S..PT300S)", "","PT10S");
					_shakeDuration = shakeDuration;
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
				{
					return returnVector;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
			case SpecificCore::DeviceType::MultidropMicro :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryDuration> shakeDuration(new SPICE::BIG::DataEntryTypes::DataEntryDuration("shakeDuration"));
					shakeDuration->setAdditionalInformations("This parameter defines the shaking duration. (Range: 1..60s, PT1S..PT60S)", "","PT10S");
					_shakeDuration = shakeDuration;
				}
				break;
			default :
				break;
			}
			_shakeDuration->setIsNeeded(true);

			returnVector.push_back(_shakeDuration);
			return returnVector;
		}
		double Shake::calculateEstimatedDuration()
		{
			if(_shakeDuration->getIsSet())
			{
				return _shakeDuration->getValue() + 5;
			}
			return 0;
		}

		void Shake::additionalCommandCheck(int& returnCode, std::string& message)
		{
			double duration = _shakeDuration->getValue();
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
			case SpecificCore::DeviceType::MultidropCombiNL :
				if(duration < 1 || duration > 300)
				{
					returnCode = 11;
					message = "Invalid command parameter: Duration has to be in rang 1..300s (PT1S..PT300s)";
					return;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
			case SpecificCore::DeviceType::MultidropMicro :
				if(duration < 1 || duration > 300)
				{
					returnCode = 11;
					message = "Invalid command parameter: Duration has to be in rang 1..60s (PT1S..PT60s)";
					return;
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
			default :
				break;

			}
			unsigned int durationInt = (unsigned int)duration;
			if(duration != (double)durationInt)
			{
				returnCode = 11;
				message = "Invalid command parameter: Duration has to be a multiple of 1s";
				return;
			}
		}

		bool Shake::processing()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
			case SpecificCore::DeviceType::MultidropCombiNL :
				{
					unsigned short shakeDistance = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("ShakeDistance")->getValueAsString());
					unsigned short shakeSpeed = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("ShakeSpeed")->getValueAsString());
					return _specificCore->shake((unsigned int)(_shakeDuration->getValue()), shakeDistance, shakeSpeed, _commandCallback);
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
				{
					return false;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
			case SpecificCore::DeviceType::MultidropMicro :
				{
					return _specificCore->shake((unsigned int)(_shakeDuration->getValue()), _commandCallback);
				}
				break;
			default :
				return false;
				break;
			}
		}
	}
}