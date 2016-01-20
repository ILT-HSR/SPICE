/**
	SPI Multidrop
	ResourceProvider.cpp
	Purpose: Implementation of the specific resource provider

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_10_07
*/

#include "ResourceProvider.h"

#include "XMLParserPoco.h"

#include "DataEntryUnsignedShort.h"
#include "DataEntryUnsignedInt.h"
#include "DataEntryBool.h"

#include "Dispense.h"
#include "DispenseByColumn.h"
#include "EmptyTubes.h"
#include "PrepareForInput.h"
#include "PrepareForOutput.h"
#include "Prime.h"
#include "Shake.h"
#include "SpecificInit.h"
#include "SpecificSetParameters.h"
#include "SpecificReset.h"


namespace SPI
{
	namespace Multidrop
	{

		const std::string ResourceProvider::CONFIG_SET_COMBI_FILE = "configuration1Combi.xml";
		const std::string ResourceProvider::CONFIG_SET_MICRODW384_FILE = "configuration1MicroDW384.xml";

		ResourceProvider::ResourceProvider(std::shared_ptr<SpecificCoreContainer> specificCoreContainer) :
			_specificCoreContainer(specificCoreContainer),
			_parameterSetCombi(std::shared_ptr<SPICE::BIG::DataSet>(new SPICE::BIG::DataSet())),
			_parameterSetMicroDW384(std::shared_ptr<SPICE::BIG::DataSet>(new SPICE::BIG::DataSet()))
		{
			// Generate parameterSetCombi
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort> plateType(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort("PlateType", 0, 9));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryBool> useDefaultPlateTypeConfig(new SPICE::BIG::DataEntryTypes::DataEntryBool("UseDefaultPlateTypeConfig"));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt> predispenseVolume(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt("PredispenseVolume", 1, 10000));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt> dispensingHeight(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt("DispensingHeight", 500, 5500));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort> dispensingOrder(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort("DispensingOrder", 0, 1));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> dispenseOffsetX(new SPICE::BIG::DataEntryTypes::DataEntryInt("DispenseOffsetX", -300, 300));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> dispenseOffsetY(new SPICE::BIG::DataEntryTypes::DataEntryInt("DispenseOffsetY", -300, 300));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort> cassetteType(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort("CassetteType", 0, 1));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort> pumpSpeed(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort("PumpSpeed", 1, 100));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort> shakeDistance(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort("ShakeDistance", 1, 5));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort> shakeSpeed(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort("ShakeSpeed", 1, 20));

			plateType->setAdditionalInformations("This parameter defines the used plate type","","0");
			useDefaultPlateTypeConfig->setAdditionalInformations("This parameter defines if the default PlateType config should be used. If false, given parameter values will be taken.", "","true");
			predispenseVolume->setAdditionalInformations("This parameter defines the predispensing volume. (Only used if UseDefaultPlateTypeConfig = false)","ul","20");
			dispensingHeight->setAdditionalInformations("This parameter defines the dispensing height. (Only used if UseDefaultPlateTypeConfig = false)","0.01mm","1600");
			dispensingOrder->setAdditionalInformations("This parameter defines the dispensing order. (Only used if UseDefaultPlateTypeConfig = false)","","0");
			dispenseOffsetX->setAdditionalInformations("This parameter defines the dispensing offset in x-direction. (Only used if UseDefaultPlateTypeConfig = false)","0.01mm","0");
			dispenseOffsetY->setAdditionalInformations("This parameter defines the dispensing offset in x-direction. (Only used if UseDefaultPlateTypeConfig = false)","0.01mm","0");
			cassetteType->setAdditionalInformations("This parameter defines the used cassette type","","0");
			pumpSpeed->setAdditionalInformations("This parameter defines the pump speed in % of the maximum allowed spped.","%","100");
			shakeDistance->setAdditionalInformations("This parameter defines the distance of the shaking movement.","mm","1");
			shakeSpeed->setAdditionalInformations("This parameter defines the frequency of the shaking movement.","Hz","20");

			plateType->setValue(0);
			useDefaultPlateTypeConfig->setValue(true);
			predispenseVolume->setValue(20);
			dispensingHeight->setValue(1600);
			dispensingOrder->setValue(0);
			dispenseOffsetX->setValue(0);
			dispenseOffsetY->setValue(0);
			cassetteType->setValue(0);
			pumpSpeed->setValue(100);
			shakeDistance->setValue(1);
			shakeSpeed->setValue(20);

			_parameterSetCombi->addDataEntry(plateType);
			_parameterSetCombi->addDataEntry(useDefaultPlateTypeConfig);
			_parameterSetCombi->addDataEntry(predispenseVolume);
			_parameterSetCombi->addDataEntry(dispensingHeight);
			_parameterSetCombi->addDataEntry(dispensingOrder);
			_parameterSetCombi->addDataEntry(dispenseOffsetX);
			_parameterSetCombi->addDataEntry(dispenseOffsetY);
			_parameterSetCombi->addDataEntry(cassetteType);
			_parameterSetCombi->addDataEntry(pumpSpeed);
			_parameterSetCombi->addDataEntry(shakeDistance);
			_parameterSetCombi->addDataEntry(shakeSpeed);

			// Generate parameterSetMicroDW384
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort> plateType2(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedShort("PlateType", 0, 1));
			plateType2->setAdditionalInformations("This parameter defines the used plate type (0 - 96 / 1 - 384)","","0");
			plateType2->setValue(0);
			_parameterSetMicroDW384->addDataEntry(plateType2);

			_configSetsCombi[1] = _parameterSetCombi;
			_configSetsMicroDW384[1] = _parameterSetMicroDW384;

			_configSetsCombi[1]->loadParameterSetFromPath(CONFIG_SET_COMBI_FILE, getNewXMLParserInstance());
			_configSetsMicroDW384[1]->loadParameterSetFromPath(CONFIG_SET_MICRODW384_FILE, getNewXMLParserInstance());
		}
		ResourceProvider::~ResourceProvider()
		{
		}

		std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> ResourceProvider::getInstanceOfAllCommands()
		{
			std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> returnVector;

			returnVector.push_back(createCommand("Dispense"));
			returnVector.push_back(createCommand("DispenseByColumn"));
			returnVector.push_back(createCommand("EmptyTubes"));
			returnVector.push_back(createCommand("PrepareForInput"));
			returnVector.push_back(createCommand("PrepareForOutput"));
			returnVector.push_back(createCommand("Prime"));
			if(_specificCoreContainer->getSpecificCore()->getDeviceType() != SpecificCore::DeviceType::MultidropDW)
			{
				returnVector.push_back(createCommand("Shake"));
			}

			return returnVector;
		}
		std::shared_ptr<SPICE::BIG::CommandBase> ResourceProvider::createCommand(std::string commandName)
		{
			std::shared_ptr<SPICE::BIG::CommandBase> command = nullptr;
			if(commandName == "Dispense")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new Dispense(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "DispenseByColumn")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new DispenseByColumn(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "EmptyTubes")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new EmptyTubes(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "PrepareForInput")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new PrepareForInput(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "PrepareForOutput")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new PrepareForOutput(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "Prime")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new Prime(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "Shake" && _specificCoreContainer->getSpecificCore()->getDeviceType() != SpecificCore::DeviceType::MultidropDW)
			{
				switch(_specificCoreContainer->getSpecificCore()->getDeviceType())
				{
					case SpecificCore::DeviceType::MultidropCombi :
						{
							command = std::shared_ptr<SPICE::BIG::CommandBase>(new Shake(_specificCoreContainer->getSpecificCore(), _parameterSetCombi));
						}
						break;
					case SpecificCore::DeviceType::Multidrop384 :
					case SpecificCore::DeviceType::MultidropDW :
					case SpecificCore::DeviceType::MultidropMicro :
						{
							command = std::shared_ptr<SPICE::BIG::CommandBase>(new Shake(_specificCoreContainer->getSpecificCore(), _parameterSetMicroDW384));
						}
						break;
					default :
						break;
				}
			}
			return command;
		}
		std::shared_ptr<SPICE::BIG::SpecificBase> ResourceProvider::getSpecificInit()
		{
			switch(_specificCoreContainer->getSpecificCore()->getDeviceType())
			{
				case SpecificCore::DeviceType::MultidropCombi :
					{
						std::shared_ptr<SpecificInit> specificInit(new SpecificInit(_specificCoreContainer->getSpecificCore(), _configSetsCombi[1]));
						return specificInit;
					}
					break;
				case SpecificCore::DeviceType::Multidrop384 :
				case SpecificCore::DeviceType::MultidropDW :
				case SpecificCore::DeviceType::MultidropMicro :
					{
						std::shared_ptr<SpecificInit> specificInit(new SpecificInit(_specificCoreContainer->getSpecificCore(), _configSetsMicroDW384[1]));
						return specificInit;
					}
					break;
				default :
					break;
			}
			return nullptr;
		}
		std::shared_ptr<SPICE::BIG::SpecificBase> ResourceProvider::getSpecificSetParameters()
		{
			switch(_specificCoreContainer->getSpecificCore()->getDeviceType())
			{
				case SpecificCore::DeviceType::MultidropCombi :
					{
						std::shared_ptr<SpecificSetParameters> specificSetParameters(new SpecificSetParameters(_specificCoreContainer->getSpecificCore(), _parameterSetCombi));
						return specificSetParameters;
					}
					break;
				case SpecificCore::DeviceType::Multidrop384 :
				case SpecificCore::DeviceType::MultidropDW :
				case SpecificCore::DeviceType::MultidropMicro :
					{
						std::shared_ptr<SpecificSetParameters> specificSetParameters(new SpecificSetParameters(_specificCoreContainer->getSpecificCore(), _parameterSetMicroDW384));
						return specificSetParameters;
					}
					break;
				default :
					break;
			}
			return nullptr;
		}
		std::shared_ptr<SPICE::BIG::SpecificResetBase> ResourceProvider::getSpecificReset()
		{
			std::shared_ptr<SpecificReset> specificReset(new SpecificReset(_specificCoreContainer));
			return specificReset;
		}

		std::shared_ptr<SPICE::BIG::DataSet> ResourceProvider::getParameterSet()
		{
			switch(_specificCoreContainer->getSpecificCore()->getDeviceType())
			{
				case SpecificCore::DeviceType::MultidropCombi :
					return _parameterSetCombi;
					break;
				case SpecificCore::DeviceType::Multidrop384 :
				case SpecificCore::DeviceType::MultidropDW :
				case SpecificCore::DeviceType::MultidropMicro :
					return _parameterSetMicroDW384;
					break;
				default :
					break;
			}
			return nullptr;
		}
		std::shared_ptr<SPICE::BIG::DataSet> ResourceProvider::getConfigurationSet(int configLevel, std::string password)
		{
			switch(_specificCoreContainer->getSpecificCore()->getDeviceType())
			{
				case SpecificCore::DeviceType::MultidropCombi :
					if(_configSetsCombi.find(configLevel) != _configSetsCombi.end())
					{
						std::string configPassword = _configSetsCombi[configLevel]->getPassword();
						if(configPassword == password || configPassword == "")
						{
							return _configSetsCombi[configLevel];
						}
					}
					break;
				case SpecificCore::DeviceType::Multidrop384 :
				case SpecificCore::DeviceType::MultidropDW :
				case SpecificCore::DeviceType::MultidropMicro :
					if(_configSetsMicroDW384.find(configLevel) != _configSetsMicroDW384.end())
					{
						std::string configPassword = _configSetsMicroDW384[configLevel]->getPassword();
						if(configPassword == password || configPassword == "")
						{
							return _configSetsMicroDW384[configLevel];
						}
					}
					break;
				default :
					break;
			}
			return nullptr;
		}
		void ResourceProvider::configurationSetHasChanged(int configLevel)
		{
			switch(_specificCoreContainer->getSpecificCore()->getDeviceType())
			{
				case SpecificCore::DeviceType::MultidropCombi :
					if(_configSetsCombi.find(configLevel) != _configSetsCombi.end())
					{
						_configSetsCombi[configLevel]->writeParameterSetToPath(CONFIG_SET_COMBI_FILE);
					}
					break;
				case SpecificCore::DeviceType::Multidrop384 :
				case SpecificCore::DeviceType::MultidropDW :
				case SpecificCore::DeviceType::MultidropMicro :
					if(_configSetsMicroDW384.find(configLevel) != _configSetsCombi.end())
					{
						_configSetsMicroDW384[configLevel]->writeParameterSetToPath(CONFIG_SET_MICRODW384_FILE);
					}
					break;
				default :
					break;
			}
		}

		void ResourceProvider::getDeviceIdentification(std::string& uriPathName, int& silaDevicClass, std::string& deviceManufacturer, std::string& deviceName, std::string& deviceSerialNumber, std::string& deviceFirmwareVersion, bool& includeConverterIdentification)
		{
			uriPathName = "/" + getCoreConfigurationParameter("URI_PATHNAME");
			silaDevicClass = getDeviceClass();
			deviceManufacturer = "Thermo Fisher Scientific";
			deviceName = _specificCoreContainer->getSpecificCore()->getMultidropDeviceTypeName();
			deviceSerialNumber = _specificCoreContainer->getSpecificCore()->getMultidropSerialNo();
			deviceFirmwareVersion = _specificCoreContainer->getSpecificCore()->getMultidropFirmwareVersion();
			includeConverterIdentification = true;
		}
		int ResourceProvider::getDeviceClass()
		{
			return 1;
		}
		std::string ResourceProvider::getCoreConfigurationParameter(std::string parameterName)
		{
			if(parameterName == "URI_PATHNAME")
			{
				return "Dispenser"; // default: "<DeviceClass>"
			}
			else if(parameterName == "SERIAL_NUMBER_EXTENSION")
			{
				return ""; // default: ""
			}
			else if(parameterName == "USE_WSDISCOVERY")
			{
				return "true"; // default: "false"
			}
			else if(parameterName == "NICELABFIX_SUBSTATES")
			{
				return "true"; // default: "false"
			}
			else if(parameterName == "SILA_NS_PREFIX")
			{
				return ""; // default "", example: "sila:"
			}
			return "";
		}

		std::shared_ptr<SPICE::BIG::IXMLParser> ResourceProvider::getNewXMLParserInstance()
		{
			std::shared_ptr<SPICE::XML::POCO::XMLParserPoco> xmlParser(new SPICE::XML::POCO::XMLParserPoco());
			return xmlParser;
		}

	}
}