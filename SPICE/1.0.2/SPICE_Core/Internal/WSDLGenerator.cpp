/**
	SPICE Core
	WSDLGenerator.cpp
	Purpose: Class to generate a WSDL file

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "WSDLGenerator.h"

#include <sstream>
#include "SiLASOAPFunctions.h"


namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			WSDLGenerator::WSDLGenerator() :
				_wsdlFile(""),
				_isGenerated(false),
				_addressDummy("http://ZZZ.ZZZ.ZZZ.ZZZ:YYY/URIPATHNAME")
			{
			}
			WSDLGenerator::~WSDLGenerator()
			{
			}

			std::string WSDLGenerator::getWSDLFile()
			{
				if(_isGenerated)
				{
					return _wsdlFile;
				}
				return "";
			}
			std::string WSDLGenerator::getAddressDummy()
			{
				return _addressDummy;
			}

			void WSDLGenerator::generateWSDLFile(std::vector<std::shared_ptr<SPICE::BIG::CommandBase>> commands)
			{
				std::string wsdlType = "";
				std::string wsdlMessage = "";
				std::string wsdlPortOperation = "";
				std::string wsdlBindOperation = "";

				for(unsigned int i = 0; i < commands.size(); i++)
				{
					generateWSDLCommand(commands.at(i), wsdlType, wsdlMessage, wsdlPortOperation, wsdlBindOperation);
				}
				addDefaultWSDLParts(wsdlType, wsdlMessage, wsdlPortOperation, wsdlBindOperation);

				std::ostringstream strout;
				strout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl;
				strout << "<wsdl:definitions xmlns:soap=\"http://schemas.xmlsoap.org/wsdl/soap/\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:wsdl=\"http://schemas.xmlsoap.org/wsdl/\" xmlns:tns=\"http://sila-standard.org\" xmlns:s0=\"http://sila-standard.org\" xmlns:wsaw=\"http://www.w3.org/2006/05/addressing/wsdl\" targetNamespace=\"http://sila-standard.org\" name=\"silaLibService\">" << std::endl;
				strout << "<wsdl:types>" << std::endl;
				strout << "<xsd:schema elementFormDefault=\"qualified\" targetNamespace=\"http://sila-standard.org\">" << std::endl;
				strout << wsdlType;
				strout << "</xsd:schema>" << std::endl << "</wsdl:types>" << std::endl;
				strout << wsdlMessage;
				strout << "<wsdl:portType name=\"silaLibPortType\">" << std::endl;
				strout << wsdlPortOperation;
				strout << "</wsdl:portType>" << std::endl;
				strout << "<wsdl:binding name=\"silaLibBinding\" type=\"tns:silaLibPortType\">" << std::endl << "<soap:binding transport=\"http://schemas.xmlsoap.org/soap/http\"/>" << std::endl;
				strout << wsdlBindOperation;
				strout << "</wsdl:binding>" << std::endl;
				strout << "<wsdl:service name=\"silaLibService\">" << std::endl << "<wsdl:documentation>WSDL File</wsdl:documentation>" << std::endl;
				strout << "<wsdl:port name=\"silaLibPort\" binding=\"tns:silaLibBinding\">" << std::endl;
				strout << "<soap:address location=\"" << _addressDummy << "\"/>" << std::endl;
				strout << "</wsdl:port>" << std::endl << "</wsdl:service>" << std::endl << "</wsdl:definitions>";

				_wsdlFile = strout.str();
				_isGenerated = true;
			}

			void WSDLGenerator::generateWSDLCommand(std::shared_ptr<SPICE::BIG::CommandBase> command, std::string& wsdlType, std::string& wsdlMessage, std::string& wsdlPortOperation, std::string& wsdlBindOperation)
			{
				std::ostringstream stroutType;
				std::ostringstream stroutMessage;
				std::ostringstream stroutPortOperation;
				std::ostringstream stroutDescription;
				std::ostringstream stroutBindOperation;

				std::vector<std::shared_ptr<SPICE::BIG::DataEntry>> parameterList = command->createAndGetAdditionalCommandParameters();
				std::string responseDescription = "";
				std::vector<std::shared_ptr<SPICE::BIG::DataSet>> responseList = command->getResponseDataInformation(responseDescription);

				// ------ Type ---------
				stroutType << "<xsd:element name=\"" << command->getCommandName() << "\">" << std::endl << "<xsd:complexType>" << std::endl << "<xsd:sequence>" << std::endl;
				stroutType << "<xsd:element name=\"requestId\" minOccurs=\"1\" maxOccurs=\"1\" type=\"xsd:int\"/>" << std::endl;
				if(command->getCommandName() != "LockDevice")
				{
					stroutType << "<xsd:element name=\"lockId\" minOccurs=\"0\" maxOccurs=\"1\" type=\"xsd:string\"/>" << std::endl;
				}
				else
				{
					stroutType << "<xsd:element name=\"lockId\" minOccurs=\"1\" maxOccurs=\"1\" type=\"xsd:string\"/>" << std::endl;
				}
				for(std::vector<std::shared_ptr<SPICE::BIG::DataEntry>>::iterator i = parameterList.begin(); i != parameterList.end(); i++)
				{
					stroutType << "<xsd:element name=\"" << i->get()->getName();
					if(i->get()->getIsNeeded())
					{
						stroutType << "\" minOccurs=\"1\" maxOccurs=\"1\" type=\"";
					}
					else
					{
						stroutType << "\" minOccurs=\"0\" maxOccurs=\"1\" type=\"";
					}
					stroutType << getXSDFromDataEntryType(i->get()->getDataEntryType()) << "\"/>" << std::endl;
				}
				stroutType << "</xsd:sequence>" << std::endl << "</xsd:complexType>" << std::endl << "</xsd:element>" << std::endl;

				stroutType << "<xsd:element name=\"" << command->getCommandName() << "Response\">" << std::endl;
				stroutType << "<xsd:complexType>" << std::endl << "<xsd:sequence>" << std::endl;
				stroutType << "<xsd:element name=\"" << command->getCommandName() << "Result\" minOccurs=\"0\" maxOccurs=\"1\" type=\"s0:SiLAReturnValue\"/>" << std::endl;
				stroutType << "</xsd:sequence>" << std::endl << "</xsd:complexType>" << std::endl << "</xsd:element>" << std::endl;

				// ------ Message --------
				stroutMessage << "<wsdl:message name=\"" << command->getCommandName() << "Request\">" << std::endl << "<wsdl:part name=\"parameters\" element=\"s0:" << command->getCommandName() << "\"/>" << std::endl << "</wsdl:message>" << std::endl;
				stroutMessage << "<wsdl:message name=\"" << command->getCommandName() << "Response\">" << std::endl << "<wsdl:part name=\"parameters\" element=\"s0:" << command->getCommandName() << "Response\"/>" << std::endl << "</wsdl:message>" << std::endl;

				// ------- SiLACommandDescription --------
				stroutDescription << "<SiLACommandDescription isCommonCommand=\"";
				if(command->isCommonCommand())
				{
					stroutDescription << "true\" estimatedDuration=\"";
				}
				else
				{
					stroutDescription << "false\" estimatedDuration=\"";
				}
				stroutDescription << SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(command->calculateEstimatedDuration()) << "\" ";
				stroutDescription << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" ";
				stroutDescription << "xsi:noNamespaceSchemaLocation=\"http://sila-standard.org/schemata/SoapAnnotation_1.2.xsd\">" << std::endl;
				stroutDescription << "<Summary>" << command->getCommandDescription() << "</Summary>" << std::endl;
				stroutDescription << "<Parameter name=\"requestId\" minValue=\"1\" maxValue=\"2147483647\">This parameter is the unique identification of this command call.</Parameter>" << std::endl;
				stroutDescription << "<Parameter name=\"lockId\">This parameter is the identification of the PMS, which has locked the device.</Parameter>" << std::endl;
				for(std::vector<std::shared_ptr<SPICE::BIG::DataEntry>>::iterator i = parameterList.begin(); i != parameterList.end(); i++)
				{
					stroutDescription << "<Parameter name=\"" << i->get()->getName() << "\"";
					std::string minValue = i->get()->getMinValue();
					std::string maxValue = i->get()->getMaxValue();
					std::string defaultValue = i->get()->getDefaultValue();
					std::string unit = i->get()->getUnit();
					if(minValue != "" && maxValue != "")
					{
						stroutDescription << " minValue=\"" << minValue << "\" maxValue=\"" << maxValue << "\"";
					}
					if(unit != "")
					{
						stroutDescription << " unit=\"" << unit << "\"";
					}
					if(defaultValue != "")
					{
						stroutDescription << " defaultValue=\"" << defaultValue << "\"";
					}
					stroutDescription << ">" << i->get()->getDescription() << "</Parameter>" << std::endl;
				}
				stroutDescription << "<Response xsi:type=\"standardResponse\" parameterSetCount=\"";
				stroutDescription << responseList.size() << "\">" << std::endl;
				stroutDescription << "<Description>" << responseDescription << "</Description>" << std::endl;

				if(responseList.size() > 0)
				{
					std::shared_ptr<SPICE::BIG::DataSet> firstResponse = responseList.at(0);
					unsigned int parameterCount = firstResponse->getDataEntriesCount();
					for(unsigned int i = 0; i < parameterCount; i++)
					{
						std::shared_ptr<SPICE::BIG::DataEntry> dataEntry = firstResponse->getDataEntryByIndex(i);
						stroutDescription << "<Value name=\"" << dataEntry->getName() << "\" type=\"" << dataEntry->getDataEntryTypeAsString() << "\"/>" << std::endl;
					}
				}
				stroutDescription << "</Response>" << std::endl << "</SiLACommandDescription>" << std::endl;

				std::string commandDescription = SPICE::BIG::SiLASOAPFunctions::quoteXMLFile(stroutDescription.str());

				// ------- PortOperation ----------
				stroutPortOperation << "<wsdl:operation name=\"" << command->getCommandName() << "\">" << std::endl << "<wsdl:documentation>" << std::endl;
				stroutPortOperation << commandDescription;
				stroutPortOperation << "</wsdl:documentation>" << std::endl;
				stroutPortOperation << "<wsdl:input message=\"tns:" << command->getCommandName() << "Request\" wsaw:Action=\"http://sila-standard.org/" << command->getCommandName() << "\"/>" << std::endl;
				stroutPortOperation << "<wsdl:output message=\"tns:" << command->getCommandName() << "Response\" wsaw:Action=\"http://sila-standard.org/" << command->getCommandName() << "Response\"/>" << std::endl;
				stroutPortOperation << "</wsdl:operation>" << std::endl;

				// ------- BindOperation ----------
				stroutBindOperation << "<wsdl:operation name=\"" << command->getCommandName() << "\">" << std::endl;
				stroutBindOperation << "<soap:operation soapAction=\"http://sila-standard.org/" << command->getCommandName() << "\" style=\"document\"/>" << std::endl;
				stroutBindOperation << "<wsdl:input><soap:body use=\"literal\"/></wsdl:input>" << std::endl;
				stroutBindOperation << "<wsdl:output><soap:body use=\"literal\"/></wsdl:output>" << std::endl;
				stroutBindOperation << "</wsdl:operation>" << std::endl;


				// -------- return -----------
				wsdlType += stroutType.str();
				wsdlMessage += stroutMessage.str();
				wsdlPortOperation += stroutPortOperation.str();
				wsdlBindOperation += stroutBindOperation.str();
			}
			void WSDLGenerator::addDefaultWSDLParts(std::string& wsdlType, std::string& wsdlMessage, std::string& wsdlPortOperation, std::string& wsdlBindOperation)
			{
				std::ostringstream stroutType;

				// element GetDeviceIdentification
				stroutType << "<xsd:element name=\"GetDeviceIdentification\"><xsd:complexType><xsd:sequence>";
				stroutType << "<xsd:element name=\"requestId\" minOccurs=\"1\" maxOccurs=\"1\" type=\"xsd:int\"/>";
				stroutType << "<xsd:element name=\"lockId\" minOccurs=\"0\" maxOccurs=\"1\" type=\"xsd:string\"/>";
				stroutType << "</xsd:sequence></xsd:complexType></xsd:element>";

				// element GetDeviceIdentificationResponse
				stroutType << "<xsd:element name=\"GetDeviceIdentificationResponse\"><xsd:complexType><xsd:sequence>";
				stroutType << "<xsd:element name=\"GetDeviceIdentificationResult\" minOccurs=\"0\" maxOccurs=\"1\" type=\"s0:SiLAReturnValue\"/>";
				stroutType << "<xsd:element name=\"deviceDescription\" minOccurs=\"1\" maxOccurs=\"1\" type=\"s0:DeviceIdentification\"/>";
				stroutType << "</xsd:sequence></xsd:complexType></xsd:element>";

				// element GetStatus
				stroutType << "<xsd:element name=\"GetStatus\"><xsd:complexType><xsd:sequence>";
				stroutType << "<xsd:element name=\"requestId\" minOccurs=\"1\" maxOccurs=\"1\" type=\"xsd:int\"/>";
				stroutType << "</xsd:sequence></xsd:complexType></xsd:element>";

				// element GetStatusResponse
				stroutType << "<xsd:element name=\"GetStatusResponse\"><xsd:complexType><xsd:sequence>";
				stroutType << "<xsd:element name=\"GetStatusResult\" minOccurs=\"0\" maxOccurs=\"1\" type=\"s0:SiLAReturnValue\"/>";
				stroutType << "<xsd:element name=\"deviceId\" minOccurs=\"1\" maxOccurs=\"1\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element name=\"state\" minOccurs=\"1\" maxOccurs=\"1\" type=\"s0:State\"/>";
				stroutType << "<xsd:element name=\"subStates\" minOccurs=\"0\" maxOccurs=\"unbounded\" type=\"s0:CommandDescription\"/>";
				stroutType << "<xsd:element name=\"locked\" minOccurs=\"1\" maxOccurs=\"1\" type=\"xsd:boolean\"/>";
				stroutType << "<xsd:element name=\"PMSId\" minOccurs=\"0\" maxOccurs=\"1\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element name=\"currentTime\" minOccurs=\"1\" maxOccurs=\"1\" type=\"xsd:dateTime\"/>";
				stroutType << "</xsd:sequence></xsd:complexType></xsd:element>";

				// complex type SiLAReturnValue
				stroutType << "<xsd:complexType name=\"SiLAReturnValue\"><xsd:sequence>";
				stroutType << "<xsd:element minOccurs=\"1\" maxOccurs=\"1\" name=\"returnCode\" type=\"xsd:int\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"message\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"duration\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"1\" maxOccurs=\"1\" name=\"deviceClass\" type=\"xsd:int\"/>";
				stroutType << "</xsd:sequence></xsd:complexType>";

				// complex type CommandDescription
				stroutType << "<xsd:complexType name=\"CommandDescription\"><xsd:sequence>";
				stroutType << "<xsd:element minOccurs=\"1\" maxOccurs=\"1\" name=\"requestId\" type=\"xsd:int\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"commandName\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"1\" maxOccurs=\"1\" name=\"queuePosition\" type=\"xsd:int\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"startedAt\" type=\"xsd:dateTime\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"currentState\" type=\"s0:Substate\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"dataWaiting\" type=\"xsd:int\"/>";
				stroutType << "</xsd:sequence></xsd:complexType>";

				// simple type State
				stroutType << "<xsd:simpleType name=\"State\"><xsd:restriction base=\"xsd:string\">";
				stroutType << "<xsd:enumeration value=\"inError\"/>";
				stroutType << "<xsd:enumeration value=\"errorHandling\"/>";
				stroutType << "<xsd:enumeration value=\"idle\"/>";
				stroutType << "<xsd:enumeration value=\"startup\"/>";
				stroutType << "<xsd:enumeration value=\"standby\"/>";
				stroutType << "<xsd:enumeration value=\"busy\"/>";
				stroutType << "<xsd:enumeration value=\"paused\"/>";
				stroutType << "<xsd:enumeration value=\"initializing\"/>";
				stroutType << "<xsd:enumeration value=\"resetting\"/>";
				stroutType << "</xsd:restriction></xsd:simpleType>";

				// simple type Substate
				stroutType << "<xsd:simpleType name=\"Substate\"><xsd:restriction base=\"xsd:string\">";
				stroutType << "<xsd:enumeration value=\"asynchPaused\"/>";
				stroutType << "<xsd:enumeration value=\"pauseRequested\"/>";
				stroutType << "<xsd:enumeration value=\"processing\"/>";
				stroutType << "<xsd:enumeration value=\"responseWaiting\"/>";
				stroutType << "<xsd:enumeration value=\"none\"/>";
				stroutType << "</xsd:restriction></xsd:simpleType>";

				// complex type CommandDescription
				stroutType << "<xsd:complexType name=\"DeviceIdentification\"><xsd:sequence>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"Wsdl\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"SiLAInterfaceVersion\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"1\" maxOccurs=\"1\" name=\"SiLADeviceClass\" type=\"xsd:int\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"SiLADeviceClassVersion\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"unbounded\" name=\"SiLASubDeviceClass\" type=\"xsd:int\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"DeviceManufacturer\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"DeviceName\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"DeviceSerialNumber\" type=\"xsd:string\"/>";
				stroutType << "<xsd:element minOccurs=\"0\" maxOccurs=\"1\" name=\"DeviceFirmwareVersion\" type=\"xsd:string\"/>";
				stroutType << "</xsd:sequence></xsd:complexType>";

				wsdlType += stroutType.str();

				
				std::ostringstream stroutMsg;

				stroutMsg << "<wsdl:message name=\"GetDeviceIdentificationRequest\"><wsdl:part name=\"parameters\" element=\"s0:GetDeviceIdentification\"/></wsdl:message>";
				stroutMsg << "<wsdl:message name=\"GetDeviceIdentificationResponse\"><wsdl:part name=\"parameters\" element=\"s0:GetDeviceIdentificationResponse\"/></wsdl:message>";
				stroutMsg << "<wsdl:message name=\"GetStatusRequest\"><wsdl:part name=\"parameters\" element=\"s0:GetStatus\"/></wsdl:message>";
				stroutMsg << "<wsdl:message name=\"GetStatusResponse\"><wsdl:part name=\"parameters\" element=\"s0:GetStatusResponse\"/></wsdl:message>";

				wsdlMessage += stroutMsg.str();


				std::ostringstream stroutPortOperation;

				// port operation GetDeviceIdentification
				stroutPortOperation << "<wsdl:operation name=\"GetDeviceIdentification\">";
				stroutPortOperation << "<wsdl:documentation>";
				stroutPortOperation << "&lt;SiLACommandDescription isCommonCommand=&quot;true&quot; estimatedDuration=&quot;PT0S&quot; xmlns:xsi=&quot;http://www.w3.org/2001/XMLSchema-instance&quot; xsi:noNamespaceSchemaLocation=&quot;http://sila-standard.org/schemata/SoapAnnotation_1.2.xsd&quot; &gt;";
				stroutPortOperation << "&lt;Summary&gt;This command reports on details of the device.&lt;/Summary&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;requestId&quot; minValue=&quot;1&quot; maxValue=&quot;2147483647&quot; &gt;This parameter is the unique identification of this command call.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;lockId&quot; &gt;This parameter is the identification of the PMS which has locked the device.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;deviceDescription&quot; &gt;The Device Identification. It is a SOAP complex type.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;/SiLACommandDescription&gt;";
				stroutPortOperation << "</wsdl:documentation>";
				stroutPortOperation << "<wsdl:input message=\"tns:GetDeviceIdentificationRequest\" wsaw:Action=\"http://sila-standard.org/GetDeviceIdentification\"/>";
				stroutPortOperation << "<wsdl:output message=\"tns:GetDeviceIdentificationResponse\" wsaw:Action=\"http://sila-standard.org/GetDeviceIdentificationResponse\"/>";
				stroutPortOperation << "</wsdl:operation>";

				// port operation GetStatus
				stroutPortOperation << "<wsdl:operation name=\"GetStatus\">";
				stroutPortOperation << "<wsdl:documentation>";
				stroutPortOperation << "&lt;SiLACommandDescription isCommonCommand=&quot;true&quot; estimatedDuration=&quot;PT0S&quot; xmlns:xsi=&quot;http://www.w3.org/2001/XMLSchema-instance&quot; xsi:noNamespaceSchemaLocation=&quot;http://sila-standard.org/schemata/SoapAnnotation_1.2.xsd&quot; &gt;";
				stroutPortOperation << "&lt;Summary&gt;This command reports the status of the device.&lt;/Summary&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;requestId&quot; minValue=&quot;1&quot; maxValue=&quot;2147483647&quot; &gt;This parameter is the unique identification of this command call.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;deviceId&quot; &gt;The identification the device returns to identify itself at the PMS.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;state&quot; &gt;Status of the device&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;subStates&quot; &gt;Substate of the above state.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;locked&quot; &gt;Lock state of the device (locked=true, unlocked = false)&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;PMSId&quot; &gt;Identification of the PMS that locked the device.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;Parameter name=&quot;currentTime&quot; &gt;Time of reporting status information.&lt;/Parameter&gt;";
				stroutPortOperation << "&lt;/SiLACommandDescription&gt;";
				stroutPortOperation << "</wsdl:documentation>";
				stroutPortOperation << "<wsdl:input message=\"tns:GetStatusRequest\" wsaw:Action=\"http://sila-standard.org/GetStatus\"/>";
				stroutPortOperation << "<wsdl:output message=\"tns:GetStatusResponse\" wsaw:Action=\"http://sila-standard.org/GetStatusResponse\"/>";
				stroutPortOperation << "</wsdl:operation>";

				wsdlPortOperation += stroutPortOperation.str();


				std::ostringstream stroutBindOperation;

				// bind operation GetDeviceIdentification
				stroutBindOperation << "<wsdl:operation name=\"GetDeviceIdentification\">";
				stroutBindOperation << "<soap:operation soapAction=\"http://sila-standard.org/GetDeviceIdentification\" style=\"document\"/>";
				stroutBindOperation << "<wsdl:input><soap:body use=\"literal\"/></wsdl:input>";
				stroutBindOperation << "<wsdl:output><soap:body use=\"literal\"/></wsdl:output>";
				stroutBindOperation << "</wsdl:operation>";

				// bind operation GetStatus
				stroutBindOperation << "<wsdl:operation name=\"GetStatus\">";
				stroutBindOperation << "<soap:operation soapAction=\"http://sila-standard.org/GetStatus\" style=\"document\"/>";
				stroutBindOperation << "<wsdl:input><soap:body use=\"literal\"/></wsdl:input>";
				stroutBindOperation << "<wsdl:output><soap:body use=\"literal\"/></wsdl:output>";
				stroutBindOperation << "</wsdl:operation>";

				wsdlBindOperation += stroutBindOperation.str();
			}

			std::string WSDLGenerator::getXSDFromDataEntryType(SPICE::BIG::DataEntry::Types dataEntryType)
			{
				switch(dataEntryType)
				{
				case SPICE::BIG::DataEntry::Types::Long :
					return "xsd:long";
					break;
				case SPICE::BIG::DataEntry::Types::Int :
					return "xsd:int";
					break;
				case SPICE::BIG::DataEntry::Types::Short :
					return "xsd:short";
					break;
				case SPICE::BIG::DataEntry::Types::Byte :
					return "xsd:byte";
					break;
				case SPICE::BIG::DataEntry::Types::unsignedLong :
					return "xsd:unsignedLong";
					break;
				case SPICE::BIG::DataEntry::Types::unsignedInt :
					return "xsd:unsignedInt";
					break;
				case SPICE::BIG::DataEntry::Types::unsignedShort :
					return "xsd:unsignedShort";
					break;
				case SPICE::BIG::DataEntry::Types::unsignedByte :
					return "xsd:unsignedByte";
					break;
				case SPICE::BIG::DataEntry::Types::Float :
					return "xsd:float";
					break;
				case SPICE::BIG::DataEntry::Types::Double :
					return "xsd:double";
					break;
				case SPICE::BIG::DataEntry::Types::Boolean :
					return "xsd:boolean";
					break;
				case SPICE::BIG::DataEntry::Types::String :
					return "xsd:string";
					break;
/*				case SPICE::BIG::DataEntry::Types::Enumeration :
					return "xsd:long";
					break;*/
				case SPICE::BIG::DataEntry::Types::dateTime :
					return "xsd:dateTime";
					break;
				case SPICE::BIG::DataEntry::Types::Duration :
					return "xsd:duration";
					break;
//				case SPICE::BIG::DataEntry::Types::array :
//					throw new std::exception("Implement: DataEntry array generate correct xsd-Type");
//					break;
				case SPICE::BIG::DataEntry::Types::xmldocument :
					return "xsd:string";
					break;
//				case SPICE::BIG::DataEntry::Types::ComplexType :
//					throw new std::exception("Implement: DataEntry ComplexType generate correct xsd-Type");
//					break;
				default :
					return "";
					break;
				}
			}
		}
	}
}