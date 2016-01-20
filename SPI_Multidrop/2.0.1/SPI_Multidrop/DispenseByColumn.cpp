/**
	SPI Multidrop
	DispenseByColumn.cpp
	Purpose: Implementation of the dispense by column command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "DispenseByColumn.h"

namespace SPI
{
	namespace Multidrop
	{
		DispenseByColumn::DispenseByColumn(std::shared_ptr<SpecificCore> specificCore) :
			SpecificCommandBase(specificCore),
			_volume(nullptr),
			_startColumn(nullptr),
			_stopColumn(nullptr)
		{
		}
		DispenseByColumn::~DispenseByColumn()
		{
		}

		std::string DispenseByColumn::getCommandName()
		{
			return "DispenseByColumn";
		}
		std::string DispenseByColumn::getCommandDescription()
		{
			return "Dispenses the given volume to the given columns.";
		}

		std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> DispenseByColumn::createAndGetAdditionalCommandParameters()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 2500));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed.", "ul","5");
					_volume = volume;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> startColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("startColumn", 1, 48));
					startColumn->setAdditionalInformations("The startColumn parameter indicates the dispense start column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns / 1536 - 48 columns)", "","1");
					_startColumn = startColumn;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> stopColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("stopColumn", 1, 48));
					stopColumn->setAdditionalInformations("The stopColumn parameter indicates the dispense stop column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns / 1536 - 48 columns)", "","12");
					_stopColumn = stopColumn;
				}
				break;
			case SpecificCore::DeviceType::MultidropCombiNL :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 50, 50000));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed.", "nl","1000");
					_volume = volume;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> startColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("startColumn", 1, 48));
					startColumn->setAdditionalInformations("The startColumn parameter indicates the dispense start column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns / 1536 - 48 columns)", "","1");
					_startColumn = startColumn;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> stopColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("stopColumn", 1, 48));
					stopColumn->setAdditionalInformations("The stopColumn parameter indicates the dispense stop column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns / 1536 - 48 columns)", "","12");
					_stopColumn = stopColumn;
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 5, 1000));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed. Value has to be a multiple of 5ul. Maximum is depending on the current plate. (96 - 1000ul / 384 - 100ul)", "ul","5");
					_volume = volume;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> startColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("startColumn", 1, 24));
					startColumn->setAdditionalInformations("The startColumn parameter indicates the dispense start column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns)", "","1");
					_startColumn = startColumn;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> stopColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("stopColumn", 1, 24));
					stopColumn->setAdditionalInformations("The stopColumn parameter indicates the dispense stop column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns)", "","12");
					_stopColumn = stopColumn;
				}
				break;
			case SpecificCore::DeviceType::Multidrop384 :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 5, 1000));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed. Value has to be a multiple of 5ul. Maximum is depending on the current plate. (96 - 1000ul / 384 - 140ul)", "ul","5");
					_volume = volume;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> startColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("startColumn", 1, 24));
					startColumn->setAdditionalInformations("The startColumn parameter indicates the dispense start column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns)", "","1");
					_startColumn = startColumn;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> stopColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("stopColumn", 1, 24));
					stopColumn->setAdditionalInformations("The stopColumn parameter indicates the dispense stop column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns)", "","12");
					_stopColumn = stopColumn;
				}
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				{
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> volume(new SPICE::BIG::DataEntryTypes::DataEntryInt("volume", 1, 50));
					volume->setAdditionalInformations("This parameter represent the volume to be dispensed.", "ul","5");
					_volume = volume;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> startColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("startColumn", 1, 24));
					startColumn->setAdditionalInformations("The startColumn parameter indicates the dispense start column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns)", "","1");
					_startColumn = startColumn;
					std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> stopColumn(new SPICE::BIG::DataEntryTypes::DataEntryInt("stopColumn", 1, 24));
					stopColumn->setAdditionalInformations("The stopColumn parameter indicates the dispense stop column. Maximum is depending on the current plate. (96 - 12 columns / 384 - 24 columns)", "","12");
					_stopColumn = stopColumn;
				}
				break;
			default :
				break;
			}
			_volume->setIsNeeded(true);
			_startColumn->setIsNeeded(true);
			_stopColumn->setIsNeeded(true);

			std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> returnVector;
			returnVector.push_back(_volume);
			returnVector.push_back(_startColumn);
			returnVector.push_back(_stopColumn);
			return returnVector;
		}
		double DispenseByColumn::calculateEstimatedDuration()
		{
			if(_volume->getIsSet() && _startColumn->getIsSet() && _stopColumn->getIsSet())
			{
				return _specificCore->calculateTimeDispenseByColumn(_volume->getValue(), _startColumn->getValue(), _stopColumn->getValue());
			}
			return 0;
		}

		void DispenseByColumn::additionalCommandCheck(int& returnCode, std::string& message)
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
			if(_stopColumn->getValue() < _startColumn->getValue())
			{
				returnCode = 11;
				message = "Invalid command parameter: startColumn has to be less or equal to stopColumn";
			}
		}

		bool DispenseByColumn::processing()
		{
			return _specificCore->dispenseByColumn(_volume->getValue(), _startColumn->getValue(), _stopColumn->getValue(), _commandCallback);
		}
	}
}