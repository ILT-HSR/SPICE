/**
	SPI Multidrop
	SpecificInit.cpp
	Purpose: Implementation of the specific init which is called during the initialization

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "SpecificInit.h"

namespace SPI
{
	namespace Multidrop
	{
		SpecificInit::SpecificInit(std::shared_ptr<SpecificCore> specificCore, std::shared_ptr<SPICE::BIG::DataSet> configSet) : SpecificBase(),
			_specificCore(specificCore),
			_configSet(configSet)
		{
		}
		SpecificInit::~SpecificInit()
		{
		}

		double SpecificInit::calculateEstimatedDuration()
		{
			return 5;
		}

		bool SpecificInit::processing()
		{
			if(!_specificCore->openConnection())
			{
				std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("RS232-Connection-Error: Could not open a RS232-Port - Check that a serial port is attached.", "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
				_commandCallback->throwError(error);
				return false;
			}
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
				{
					unsigned short plateType = (unsigned short)std::stoul(_configSet->getFirstDataEntryByName("PlateType")->getValueAsString());
					bool useDefaultPlateTypeConfig = _configSet->getFirstDataEntryByName("UseDefaultPlateTypeConfig")->getValueAsString() == "true";
					unsigned int predispenseVolume = (unsigned int)std::stoul(_configSet->getFirstDataEntryByName("PredispenseVolume")->getValueAsString());
					unsigned int dispensingHeight = (unsigned int)std::stoul(_configSet->getFirstDataEntryByName("DispensingHeight")->getValueAsString());
					unsigned short dispensingOrder = (unsigned short)std::stoul(_configSet->getFirstDataEntryByName("DispensingOrder")->getValueAsString());
					int dispenseOffsetX = (int)std::stoi(_configSet->getFirstDataEntryByName("DispenseOffsetX")->getValueAsString());
					int dispenseOffsetY = (int)std::stoi(_configSet->getFirstDataEntryByName("DispenseOffsetX")->getValueAsString());
					unsigned short cassetteType = (unsigned short)std::stoul(_configSet->getFirstDataEntryByName("CassetteType")->getValueAsString());
					unsigned short pumpSpeed = (unsigned short)std::stoul(_configSet->getFirstDataEntryByName("PumpSpeed")->getValueAsString());
					return _specificCore->init(plateType, useDefaultPlateTypeConfig, predispenseVolume, dispensingHeight, dispensingOrder, dispenseOffsetX, dispenseOffsetY, cassetteType, pumpSpeed, _commandCallback);
				}
				break;
			case SpecificCore::DeviceType::MultidropCombiNL :
				{
					return false;
				}
				break;
			case SpecificCore::DeviceType::MultidropDW :
			case SpecificCore::DeviceType::Multidrop384 :
				{
					unsigned short plateType = (unsigned short)std::stoul(_configSet->getFirstDataEntryByName("PlateType")->getValueAsString());
					return _specificCore->init(plateType, _commandCallback);
				}
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				{
					unsigned short plateType = (unsigned short)std::stoul(_configSet->getFirstDataEntryByName("PlateType")->getValueAsString());
					return _specificCore->init(plateType, _commandCallback);
				}
				break;
			default :
				break;
			}
			return false;
		}
	}
}