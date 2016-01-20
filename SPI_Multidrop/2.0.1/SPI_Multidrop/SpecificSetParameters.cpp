/**
	SPI Multidrop
	SpecificSetParameters.cpp
	Purpose: Implementation of the specific parameters which is called during the SetParameters-command. Used to be able to change given parameters.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "SpecificSetParameters.h"

namespace SPI
{
	namespace Multidrop
	{
		SpecificSetParameters::SpecificSetParameters(std::shared_ptr<SpecificCore> specificCore, std::shared_ptr<SPICE::BIG::DataSet> parameterSet) : SpecificBase(),
			_specificCore(specificCore),
			_parameterSet(parameterSet)
		{
		}
		SpecificSetParameters::~SpecificSetParameters()
		{
		}

		double SpecificSetParameters::calculateEstimatedDuration()
		{
			return 5;
		}

		bool SpecificSetParameters::processing()
		{
			switch(_specificCore->getDeviceType())
			{
			case SpecificCore::DeviceType::MultidropCombi :
				{
					unsigned short plateType = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("PlateType")->getValueAsString());
					bool useDefaultPlateTypeConfig = _parameterSet->getFirstDataEntryByName("UseDefaultPlateTypeConfig")->getValueAsString() == "true";
					unsigned int predispenseVolume = (unsigned int)std::stoul(_parameterSet->getFirstDataEntryByName("PredispenseVolume")->getValueAsString());
					unsigned int dispensingHeight = (unsigned int)std::stoul(_parameterSet->getFirstDataEntryByName("DispensingHeight")->getValueAsString());
					unsigned short dispensingOrder = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("DispensingOrder")->getValueAsString());
					int dispenseOffsetX = (int)std::stoi(_parameterSet->getFirstDataEntryByName("DispenseOffsetX")->getValueAsString());
					int dispenseOffsetY = (int)std::stoi(_parameterSet->getFirstDataEntryByName("DispenseOffsetX")->getValueAsString());
					unsigned short cassetteType = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("CassetteType")->getValueAsString());
					unsigned short pumpSpeed = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("PumpSpeed")->getValueAsString());
					return _specificCore->setParameters(plateType, useDefaultPlateTypeConfig, predispenseVolume, dispensingHeight, dispensingOrder, dispenseOffsetX, dispenseOffsetY, cassetteType, pumpSpeed, _commandCallback);
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
					unsigned short plateType = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("PlateType")->getValueAsString());
					return _specificCore->setParameters(plateType, _commandCallback);
				}
				break;
			case SpecificCore::DeviceType::MultidropMicro :
				{
					unsigned short plateType = (unsigned short)std::stoul(_parameterSet->getFirstDataEntryByName("PlateType")->getValueAsString());
					return _specificCore->setParameters(plateType, _commandCallback);
				}
				break;
			default :
				break;
			}
			return false;
		}
	}
}