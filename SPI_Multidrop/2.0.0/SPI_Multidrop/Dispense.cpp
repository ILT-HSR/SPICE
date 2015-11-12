/**
	SPI Multidrop
	Dispense.cpp
	Purpose: Implementation of the dispense command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "Dispense.h"

namespace SPI
{
	namespace Multidrop
	{
		Dispense::Dispense(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore),
			_volume(nullptr)
		{
		}
		Dispense::~Dispense()
		{
		}

		std::string Dispense::getCommandName()
		{
			return "Dispense";
		}
		std::string Dispense::getCommandDescription()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
			case SpecificCore::DeviceType::MultidropCombiNL :
				return "This command dispenses the plate. The liquid system must have been primed.";
				break;
			case SpecificCore::DeviceType::MultidropDW :
			case SpecificCore::DeviceType::Multidrop384 :
				return "This command dispenses the plate. Primes 10ul into the priming vessel before dispensing.";
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				return "This command dispenses the plate. Primes 2ul into the priming vessel before dispensing.";
				break;
			default :
				return "";
				break;
			}
		}

		std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> Dispense::createAndGetAdditionalCommandParameters()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 2500));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed.", "ul","5");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropCombiNL :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 50, 50000));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed.", "nl","1000");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 5, 1000));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed. Value has to be a multiple of 5ul. Maximum is depending on the current plate. (96 - 1000ul / 384 - 100ul)", "ul","5");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 5, 1000));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed. Value has to be a multiple of 5ul. Maximum is depending on the current plate. (96 - 1000ul / 384 - 140ul)", "ul","5");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 50));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed.", "ul","5");
					_volume = volume;
				}
				break;
			default :
				break;
			}
			_volume->setIsNeeded(true);

			std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
			returnVector.push_back(_volume);
			return returnVector;
		}
		double Dispense::calculateEstimatedDuration()
		{
			if(_volume->getIsSet())
			{
				return _specificCore->calculateTimeDispense(_volume->getValue());
			}
			return 0;
		}

		void Dispense::additionalCommandCheck(int& returnCode, std::string& message)
		{
			if(_specificCore->getDeviceType() == SpecificCore::DeviceType::Multidrop384 || _specificCore->getDeviceType() == SpecificCore::DeviceType::MultidropDW)
			{
				if(_volume->getValue() % 5 != 0)
				{
					returnCode = 11;
					message = "Invalid command parameter: volume has to be a multiple of 5";
					return;
				}
			}
		}

		bool Dispense::processing()
		{
			return _specificCore->dispense(_volume->getValue(), _commandCallback);
		}
	}
}