/**
	SPI Multidrop
	Prime.cpp
	Purpose: Implementation of the prime command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "Prime.h"

namespace SPI
{
	namespace Multidrop
	{
		Prime::Prime(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore),
			_volume(nullptr)
		{
		}
		Prime::~Prime()
		{
		}

		std::string Prime::getCommandName()
		{
			return "Prime";
		}
		std::string Prime::getCommandDescription()
		{
			return "This command is used to flush the tubing.";
		}

		std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> Prime::createAndGetAdditionalCommandParameters()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 10000));
					volume->setAdditionalInformations("This parameter represent the volume to be primed. Minimum is depending to the tube type. (Standard tube: 10ul, Small tube: 1ul)", "ul","10");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropCombiNL :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 50, 50000));
					volume->setAdditionalInformations("This parameter represent the volume to be primed.", "nl","1000");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 5, 1000));
					volume->setAdditionalInformations("This parameter represent the volume to be primed. Value has to be a multiple of 5ul. Maximum is depending on the current plate. (96 - 1000ul / 384 - 100ul)", "ul","100");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 5, 1000));
					volume->setAdditionalInformations("This parameter represent the volume to be primed. Value has to be a multiple of 5ul. Maximum is depending on the current plate. (96 - 1000ul / 384 - 100ul)", "ul","100");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 999));
					volume->setAdditionalInformations("This parameter represent the volume to be primed.", "ul","120");
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
		double Prime::calculateEstimatedDuration()
		{
			if(_volume->getIsSet())
			{
				return _specificCore->calculateTimeEmptyTubesAndPrime(_volume->getValue());
			}
			return 0;
		}

		void Prime::additionalCommandCheck(int& returnCode, std::string& message)
		{
			if(_specificCore->getDeviceType() == SpecificCore::DeviceType::Multidrop384 || _specificCore->getDeviceType() == SpecificCore::DeviceType::MultidropDW)
			{
				if(_volume->getValue() % 5 != 0)
				{
					returnCode = 11;
					message = "Invalid command parameter: volume has to be a multiple of 5.";
					return;
				}
			}
		}

		bool Prime::processing()
		{
			return _specificCore->prime(_volume->getValue(), _commandCallback);
		}
	}
}