/**
	SPI Multidrop
	EmptyTubes.cpp
	Purpose: Implementation of the empty tubes command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "EmptyTubes.h"

namespace SPI
{
	namespace Multidrop
	{
		EmptyTubes::EmptyTubes(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore),
			_volume(nullptr)
		{
		}
		EmptyTubes::~EmptyTubes()
		{
		}

		std::string EmptyTubes::getCommandName()
		{
			return "EmptyTubes";
		}
		std::string EmptyTubes::getCommandDescription()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
			case SpecificCore::DeviceType::MultidropCombiNL :
				return "Empty the pump. The given volume is pumped backwards.";
				break;
			case SpecificCore::DeviceType::MultidropDW :
			case SpecificCore::DeviceType::Multidrop384 :
				return "Empty the pump. A volume of 880ul is pumped backwards.";
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				return "Empty the pump. A volume of 150ul is pumped backwards.";
				break;
			default :
				return "";
				break;
			}
		}

		std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> EmptyTubes::createAndGetAdditionalCommandParameters()
		{
			std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;

			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 10000));
					volume->setAdditionalInformations("This parameter defines the volume, which needs to be pumped in order to empty the tubing. Minimum is depending to the tube type. (Standard tube: 10ul, Small tube: 1ul)", "ul","10");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropCombiNL :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("time", 1, 300));
					volume->setAdditionalInformations("This parameter defines the time, how long it should empty the tubing.", "s","10");
					_volume = volume;
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
				{
					return returnVector;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
				{
					return returnVector;
				}
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 999));
					volume->setAdditionalInformations("This parameter defines the volume, which needs to be pumped in order to empty the tubing.", "ul","150");
					_volume = volume;
				}
				break;
			default :
				break;
			}
			_volume->setIsNeeded(true);

			returnVector.push_back(_volume);
			return returnVector;
		}
		double EmptyTubes::calculateEstimatedDuration()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
			case SpecificCore::DeviceType::MultidropMicro :
				if(_volume->getIsSet())
				{
					return _specificCore->calculateTimeEmptyTubesAndPrime(_volume->getValue());
				}
				break;
			case SpecificCore::DeviceType::MultidropCombiNL :
				if(_volume->getIsSet())
				{
					return _volume->getValue() + 10;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
			case SpecificCore::DeviceType::MultidropDW :
				return _specificCore->calculateTimeEmptyTubesAndPrime(0);
				break;
			default :
				return 0;
				break;
			}
			return 0;
		}

		bool EmptyTubes::processing()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
				return _specificCore->emptyTubesVolume(_volume->getValue(), _commandCallback);
				break;
			case SpecificCore::DeviceType::MultidropCombiNL :
				return _specificCore->emptyTubesTime(_volume->getValue(), _commandCallback);
				break;
			case SpecificCore::DeviceType::MultidropDW :
			case SpecificCore::DeviceType::Multidrop384 :
				return _specificCore->emptyTubes(_commandCallback);
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				return _specificCore->emptyTubesVolume(_volume->getValue(), _commandCallback);
				break;
			default :
				return "";
				break;
			}
		}
	}
}