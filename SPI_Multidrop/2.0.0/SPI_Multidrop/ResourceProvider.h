/**
	SPI Multidrop
	ResourceProvider.h
	Purpose: Implementation of the specific resource provider

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef RESOURCEPROVIDER_H
#define RESOURCEPROVIDER_H

#include "IResourceProvider.h"
#include "SpecificCoreContainer.h"

namespace SPI
{
	namespace Multidrop
	{
		class ResourceProvider : public SPICE::BIG::IResourceProvider
		{
			public:

				/**
					Implementation of the specific resource provider
				*/
				ResourceProvider(std::shared_ptr<SpecificCoreContainer> specificCoreContainer);
				virtual ~ResourceProvider();

				/**
					Returns a vector with instances of all commands. Can be used by the core to generate a WSDL file.

					@result Vector with instances of all specific commands (one instance per command).
				*/
				virtual std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> getInstanceOfAllCommands();
				/**
					Has to check if a command for the given commandName can be provided. If not a nullptr has to be returned.

					@param commandName Name of the requested command
					@result New instance according to the requested command. Nullptr if no command for this commandName is available.
				*/
				virtual std::shared_ptr<SPICE::BIG::CommandBase> createCommand(std::string commandName);
				/**
					Has to return a new instance of the device specific initialization routine. Will be called when a Initialize command has to be executed.

					@result New instance of the device specific initialization routine
				*/
				virtual std::shared_ptr<SPICE::BIG::SpecificBase> getSpecificInit();
				/**
					Has to return a new instance of the device specific setParameters routine. Will be called when a SetParameters command has to be executed.

					@result New instance of the device specific setParameters routine
				*/
				virtual std::shared_ptr<SPICE::BIG::SpecificBase> getSpecificSetParameters();
				/**
					Has to return a new instance of the device specific reset routine. Will be called when a Reset command has to be executed.

					@result New instance of the device specific reset routine
				*/
				virtual std::shared_ptr<SPICE::BIG::SpecificResetBase> getSpecificReset();

				/**
					Provides the parameter set to the core

					@result Returns the parameterSet
				*/
				virtual std::shared_ptr<SPICE::BIG::DataSet> getParameterSet();
				/**
					Provides the diffrent configuration sets to the core

					@param configLevel configLevel of the requested configuration set
					@param password Password to get access to the configuration set. Empty string if no password is requiered.
					@result Return the configuration set if available. Return nullptr no configuration for this configLevel or password wrong
				*/
				virtual std::shared_ptr<SPICE::BIG::DataSet> getConfigurationSet(int configLevel, std::string password = "");
				/**
					Is called when a configurationSet has been changed. Possible safe-actions can be executed.

					@param configLevel The configuration level of the changed configurationSet
				*/
				virtual void configurationSetHasChanged(int configLevel);

				/**
					Provides the requiered device identification

					@param uriPathName To set the device path name in the uri (example: URI http://192.168.10.5/Dispenser then it is "Dispenser").
					@param deviceManufacturer Manufacturer of the device
					@param deviceName Human readable name of the device
					@param deviceSerialNumber Serialnumber of the device
					@param deviceFirmwareVersion Firmware-Version of the device if available
					@param includeConverterIndentification If set to true, the converter informations (name, version etc) will be included to the deviceIdentification
				*/
				virtual void getDeviceIdentification(std::string& uriPathName, int& silaDevicClass, std::string& deviceManufacturer, std::string& deviceName, std::string& deviceSerialNumber, std::string& deviceFirmwareVersion, bool& includeConverterIdentification);

				/**
					Provides the sila deviceClass to the core;

					@return The deviceClass
				*/
				virtual int getDeviceClass();

				/**
					Provides access to a map of configuration parameters for the core behaviour. Access by name of the parameter, because parameters can change depending on core version.

					@param parameterName Name of the parameter to get
					@return Value of the parameter. Returns an empty string if parameter doesn't exists
				*/
				virtual std::string getCoreConfigurationParameter(std::string parameterName);

				/**
					Gets a new instance of the xml parser

					@return XMLParser
				*/
				virtual std::shared_ptr<SPICE::BIG::IXMLParser> getNewXMLParserInstance();

		private:

			std::shared_ptr<SpecificCoreContainer> _specificCoreContainer;
			std::shared_ptr<SPICE::BIG::DataSet> _parameterSetCombi;
			std::shared_ptr<SPICE::BIG::DataSet> _parameterSetMicroDW384;
			std::map<int, std::shared_ptr<SPICE::BIG::DataSet>> _configSetsCombi;
			std::map<int, std::shared_ptr<SPICE::BIG::DataSet>> _configSetsMicroDW384;

			static const std::string CONFIG_SET_COMBI_FILE;
			static const std::string CONFIG_SET_MICRODW384_FILE;
		};
	}
}
#endif // RESOURCEPROVIDER_H
