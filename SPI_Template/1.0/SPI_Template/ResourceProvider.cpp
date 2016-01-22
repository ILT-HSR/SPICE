/**
	SPI Template
	ResourceProvider.cpp
	Purpose: Implementation of the specific resource provider

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_22
*/

#include "ResourceProvider.h"

#include "XMLParserPoco.h"

#include "DataEntryUnsignedShort.h"
#include "DataEntryUnsignedInt.h"
#include "DataEntryBool.h"

#include "CommandMinimum.h"
#include "CommandWithParameter.h"
#include "CommandWithResponseData.h"
#include "CommandWithParallelExecution.h"
#include "CommandWithSpecificSimulation.h"
#include "SpecificInit.h"
#include "SpecificSetParameters.h"
#include "SpecificReset.h"


namespace SPI
{
	namespace Template
	{

		ResourceProvider::ResourceProvider() :
			_specificCoreContainer(nullptr),
			_parameterSet(std::shared_ptr<SPICE::BIG::DataSet>(new SPICE::BIG::DataSet()))
		{
			// --- Generate parameterSet ---
			// -> Generate parameters, with names and maybe limits
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryBool> boolParameter(new SPICE::BIG::DataEntryTypes::DataEntryBool("BoolParameter"));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryInt> intParameter(new SPICE::BIG::DataEntryTypes::DataEntryInt("IntParameter", -100, 100));
			std::shared_ptr<SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt> unsignedIntParameter(new SPICE::BIG::DataEntryTypes::DataEntryUnsignedInt("UnsignedIntParameter", 0, 100));

			// -> Add description, unit and default-value
			boolParameter->setAdditionalInformations("This parameter defines a bool value","","true");
			intParameter->setAdditionalInformations("This parameter defines a int value like a speed.", "mm/s","3");
			unsignedIntParameter->setAdditionalInformations("This parameter defines an uint value like a position","mm","0");

			// -> define init-value (normaly the default-value)
			boolParameter->setValue(true);
			intParameter->setValue(3);
			unsignedIntParameter->setValue(0);

			// -> add parameters to the parameterSet
			_parameterSet->addDataEntry(boolParameter);
			_parameterSet->addDataEntry(intParameter);
			_parameterSet->addDataEntry(unsignedIntParameter);


			// --- Generate configurationSet ---
			// -> can be generated like the parameterSets. This examples shows, how to use parameterSet as configurationSet too
			_configSets[1] = _parameterSet;
			// -> Load configuration from an XML-File.
			_configSets[1]->loadParameterSetFromPath("configuration1.xml", getNewXMLParserInstance());
			// -> to password protect the configuration
			//_configSets[1]->setPassword("examplePassword");
		}
		ResourceProvider::~ResourceProvider()
		{
		}

		std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> ResourceProvider::getInstanceOfAllCommands()
		{
			std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> returnVector;

			returnVector.push_back(createCommand("CommandMinimum"));
			returnVector.push_back(createCommand("CommandWithParameter"));
			returnVector.push_back(createCommand("CommandWithResponseData"));
			returnVector.push_back(createCommand("CommandWithParallelExecution"));
			returnVector.push_back(createCommand("CommandWithSpecificSimulation"));
			return returnVector;
		}
		std::shared_ptr<SPICE::BIG::CommandBase> ResourceProvider::createCommand(std::string commandName)
		{
			std::shared_ptr<SPICE::BIG::CommandBase> command = nullptr;
			if(commandName == "CommandMinimum")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new CommandMinimum(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "CommandWithParameter")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new CommandWithParameter(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "CommandWithResponseData")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new CommandWithResponseData(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "CommandWithParallelExecution")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new CommandWithParallelExecution(_specificCoreContainer->getSpecificCore()));
			}
			else if(commandName == "CommandWithSpecificSimulation")
			{
				command = std::shared_ptr<SPICE::BIG::CommandBase>(new CommandWithSpecificSimulation(_specificCoreContainer->getSpecificCore()));
			}
			return command;
		}
		std::shared_ptr<SPICE::BIG::SpecificBase> ResourceProvider::getSpecificInit()
		{
			std::shared_ptr<SpecificInit> specificInit(new SpecificInit(_specificCoreContainer->getSpecificCore()));
			return specificInit;
		}
		std::shared_ptr<SPICE::BIG::SpecificBase> ResourceProvider::getSpecificSetParameters()
		{
			std::shared_ptr<SpecificSetParameters> specificSetParameters(new SpecificSetParameters(_specificCoreContainer->getSpecificCore()));
			return specificSetParameters;
		}
		std::shared_ptr<SPICE::BIG::SpecificResetBase> ResourceProvider::getSpecificReset()
		{
			std::shared_ptr<SpecificReset> specificReset(new SpecificReset(_specificCoreContainer));
			return specificReset;
		}

		std::shared_ptr<SPICE::BIG::DataSet> ResourceProvider::getParameterSet()
		{
			return _parameterSet;
		}
		std::shared_ptr<SPICE::BIG::DataSet> ResourceProvider::getConfigurationSet(int configLevel, std::string password)
		{
			if(_configSets.find(configLevel) != _configSets.end())
			{
				std::string configPassword = _configSets[configLevel]->getPassword();
				if(configPassword == password || configPassword == "")
				{
					return _configSets[configLevel];
				}
			}
			return nullptr;
		}
		void ResourceProvider::configurationSetHasChanged(int configLevel)
		{
			if(_configSets.find(configLevel) != _configSets.end())
			{
				_configSets[configLevel]->writeParameterSetToPath("configuration" + std::to_string(configLevel) + ".xml");
			}
		}

		void ResourceProvider::getDeviceIdentification(std::string& uriPathName, int& silaDevicClass, std::string& deviceManufacturer, std::string& deviceName, std::string& deviceSerialNumber, std::string& deviceFirmwareVersion, bool& includeConverterIdentification)
		{
			uriPathName = "/" + getCoreConfigurationParameter("URI_PATHNAME");
			silaDevicClass = getDeviceClass();
			deviceManufacturer = "Your company name / Webpage URI";
			deviceName = "Your device name";
			deviceSerialNumber = "Your serial number";
			deviceFirmwareVersion = "Your firmware version";
			includeConverterIdentification = true; // true -> includes the identification of SPICE
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
				return "false"; // default: "false"
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

		void ResourceProvider::setSpecificCoreContainer(std::shared_ptr<SpecificCoreContainer> specificCoreContainer)
		{
			_specificCoreContainer = specificCoreContainer;
		}

	}
}