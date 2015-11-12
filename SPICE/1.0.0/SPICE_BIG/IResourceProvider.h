/**
	SPICE BIG
	IResourceProvider.h
	Purpose: Interface for specific implementations to provide funktionalities to the core.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IRESOURCEPROVIDER_H
#define IRESOURCEPROVIDER_H

#include <memory>
#include <string>

#include "CommandBase.h"
#include "SpecificBase.h"
#include "SpecificResetBase.h"
#include "DataSet.h"
#include "IXMLParser.h"

namespace SPICE
{
	namespace BIG
	{
		class IResourceProvider
		{
			public:
				/**
					Returns a vector with instances of all commands. Can be used by the core to generate a WSDL file.

					@return Vector with instances of all specific commands (one instance per command).
				*/
				virtual std::vector<std::shared_ptr<CommandBase>> getInstanceOfAllCommands() = 0;
				/**
					Has to check if a command for the given commandName can be provided. If not a nullptr has to be returned.

					@param commandName Name of the requested command
					@return New instance according to the requested command. Nullptr if no command for this commandName is available.
				*/
				virtual std::shared_ptr<CommandBase> createCommand(std::string commandName) = 0;
				/**
					Has to return a new instance of the device specific initialization routine. Will be called when a Initialize command has to be executed.

					@return New instance of the device specific initialization routine
				*/
				virtual std::shared_ptr<SpecificBase> getSpecificInit() = 0;
				/**
					Has to return a new instance of the device specific setParameters routine. Will be called when a SetParameters command has to be executed.

					@return New instance of the device specific setParameters routine
				*/
				virtual std::shared_ptr<SpecificBase> getSpecificSetParameters() = 0;
				/**
					Has to return a new instance of the device specific reset routine. Will be called when a Reset command has to be executed.

					@return New instance of the device specific reset routine
				*/
				virtual std::shared_ptr<SpecificResetBase> getSpecificReset() = 0;

				/**
					Provides the parameter set to the core

					@return Returns the parameterSet
				*/
				virtual std::shared_ptr<DataSet> getParameterSet() = 0;
				/**
					Provides the diffrent configuration sets to the core

					@param configLevel configLevel of the requested configuration set
					@param password Password to get access to the configuration set. Empty string if no password is requiered.
					@return Return the configuration set if available. Return nullptr no configuration for this configLevel or password wrong
				*/
				virtual std::shared_ptr<DataSet> getConfigurationSet(int configLevel, std::string password = "") = 0;
				/**
					Is called when a configurationSet has been changed. Possible safe-actions can be executed.

					@param configLevel The configuration level of the changed configurationSet
				*/
				virtual void configurationSetHasChanged(int configLevel) = 0;

				/**
					Provides the requiered device identification

					@param uriPathName To set the device path name in the uri (example: URI http://192.168.10.5/Dispenser then it is "Dispenser").
					@param silaDeviceClass The deviceClass
					@param deviceManufacturer Manufacturer of the device
					@param deviceName Human readable name of the device
					@param deviceSerialNumber Serialnumber of the device
					@param deviceFirmwareVersion Firmware-Version of the device if available
					@param includeConverterIdentification If set to true, the converter informations (name, version etc) will be included to the deviceIdentification
				*/
				virtual void getDeviceIdentification(std::string& uriPathName, int& silaDeviceClass, std::string& deviceManufacturer, std::string& deviceName, std::string& deviceSerialNumber, std::string& deviceFirmwareVersion, bool& includeConverterIdentification) = 0;
				/**
					Provides the sila deviceClass to the core;

					@return The deviceClass
				*/
				virtual int getDeviceClass() = 0;
				/**
					Provides access to a map of configuration parameters for the core behaviour. Access by name of the parameter, because parameters can change depending on core version.

					@param parameterName Name of the parameter to get
					@return Value of the parameter. Returns an empty string if parameter doesn't exists
				*/
				virtual std::string getCoreConfigurationParameter(std::string parameterName) = 0;

				/**
					Gets a new instance of the xml parser

					@return XMLParser
				*/
				virtual std::shared_ptr<SPICE::BIG::IXMLParser> getNewXMLParserInstance() = 0;
		};
	}
}
#endif // IRESOURCEPROVIDER_H
