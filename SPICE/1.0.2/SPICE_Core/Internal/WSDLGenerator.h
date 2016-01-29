/**
	SPICE Core
	WSDLGenerator.h
	Purpose: Class to generate a WSDL file

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef WSDLGENERATOR_H
#define WSDLGENERATOR_H

#include <string>
#include <memory>
#include <mutex>
#include <atomic>
#include <vector>

#include "CommandBase.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class WSDLGenerator
			{
				public:
					/**
						Class to generate a WSDL file
					*/
					WSDLGenerator();
					virtual ~WSDLGenerator();

					/**
						Gets the generated WSDL-file

						@return Full WSDL-file
					*/
					std::string getWSDLFile();
					/**
						Gets the used address dummy to be replace with the correct address
					*/
					std::string getAddressDummy();

					/**
						Generates the WSDL-file based on the given vector list of commands

						@param commands Vector-list with one instance of each command to generate the WSDL-file
					*/
					void generateWSDLFile(std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> commands);

				protected:
				private:

					/**
						Generate the WSDL-parts for the given command and append them to the diffrent given strings

						@param command Command to add to the WSDL-File
						@param wsdlType Type definition of command is appended to this string
						@param wsdlMessage Message informations of command are appended to this string
						@param wsdlPortOperation Port operations for command are appended to this string
						@param wsdlBindOperation Bind operations for command are appended to this string
					*/
					void generateWSDLCommand(std::shared_ptr<SPICE::BIG::CommandBase> command, std::string& wsdlType, std::string& wsdlMessage, std::string& wsdlPortOperation, std::string& wsdlBindOperation);
					/**
						Adds default WSDL parts to the diffrent given strings (Data-type-definitions and GetStatus and GetDeviceIdentification)

						@param wsdlType Data-type definitions and command definitions are appended to this string
						@param wsdlMessage Message informations of commands are appended to this string
						@param wsdlPortOperation Port operations for commands are appended to this string
						@param wsdlBindOperation Bind operations for commands are appended to this string
					*/
					void addDefaultWSDLParts(std::string& wsdlType, std::string& wsdlMessage, std::string& wsdlPortOperation, std::string& wsdlBindOperation);

					/**
						Gets the XSD-string used for the given DataType

						@param dataEntryType Type of the data entry
						@return XSD-string
					*/
					std::string getXSDFromDataEntryType(SPICE::BIG::DataEntry::Types dataEntryType);

					std::string _wsdlFile;
					std::atomic<bool> _isGenerated;
					std::mutex _wsdlGeneratorMutex;
					std::string _addressDummy;



			};
		}
	}
}
#endif // WSDLGENERATOR_H
