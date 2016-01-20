/**
	SPICE Core
	WSDiscoveryCore.cpp
	Purpose: Class to provide core specific discovery functionality. Instance created per registred EthernetServer. Currently instances are not deleted until program ends.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "WSDiscoveryCore.h"

#include <iostream>
#include <sstream>

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{

			std::string WSDiscoveryCore::_deviceUUID = "";
			std::uniform_int_distribution<int> WSDiscoveryCore::_distribution(0, 15);
			std::default_random_engine WSDiscoveryCore::_generator;

			WSDiscoveryCore::WSDiscoveryCore(std::shared_ptr<CoreData> coreData) :
				_coreData(coreData),
				_instanceVersion(1),
				_messageNumber(1),
				_metadataVersion(1),
				_currentActiveMessageIDMemory(0),
				_lastChangedTime(time(nullptr))
			{
				
			}
			WSDiscoveryCore::~WSDiscoveryCore()
			{
			}

			void WSDiscoveryCore::createHelloMessage(std::string& helloMessage)
			{
#ifdef WSDISCOVERY_DEBUG
				std::cout << "WSDiscovery: Create HelloMessage" << std::endl;
#endif
				std::ostringstream strout;
				strout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
//				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:sila=\"http://sila-standard.org/WSDiscovery\">";
				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wsd=\"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01\">";
				strout << "<soap:Header>";
//				strout << "<wsa:To>urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>";
				strout << "<wsa:To>urn:docs-oasis-open-org:ws-dd:ns:discovery:2009:01</wsa:To>";
//				strout << "<wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello</wsa:Action>";
				strout << "<wsa:Action>http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Hello</wsa:Action>";
				strout << "<wsa:MessageID>" << generateRandomUUID() << "</wsa:MessageID>";
				strout << "<wsd:AppSequence InstanceId=\"" << _instanceVersion << "\" MessageNumber=\"" << _messageNumber << "\"></wsd:AppSequence>";
				strout << "</soap:Header>";
				strout << "<soap:Body>";
				strout << "<wsd:Hello>";
				strout << "<wsa:EndpointReference>";
				strout << "<wsa:Address>" << _deviceUUID << "</wsa:Address>";
				strout << "</wsa:EndpointReference>";
				strout << "<wsd:Types>SiLADevice</wsd:Types>";
//				strout << "<wsd:Scopes></wsd:Scopes>";
				strout << "<wsd:Scopes/>";
				strout << "<wsd:XAddrs>" << getAddressTemplate() << "</wsd:XAddrs>";
				strout << "<wsd:MetadataVersion>" << _metadataVersion << "</wsd:MetadataVersion>";
				strout << "</wsd:Hello>";
				strout << "</soap:Body>";
				strout << "</soap:Envelope>";

				_messageNumber = _messageNumber + 1;
				helloMessage = strout.str();
			}
			void WSDiscoveryCore::createByeMessage(std::string& byeMessage)
			{
#ifdef WSDISCOVERY_DEBUG
				std::cout << "WSDiscovery: Create ByeMessage" << std::endl;
#endif
				std::ostringstream strout;
				strout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
//				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">";
				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wsd=\"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01\">";
				strout << "<soap:Header>";
//				strout << "<wsa:To>urn:schemas-xmlsoap-org:ws:2005:04:discovery</wsa:To>";
				strout << "<wsa:To>urn:docs-oasis-open-org:ws-dd:ns:discovery:2009:01</wsa:To>";
//				strout << "<wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/Bye</wsa:Action>";
				strout << "<wsa:Action>http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Bye</wsa:Action>";
				strout << "<wsa:MessageID>" << generateRandomUUID() << "</wsa:MessageID>";
				strout << "<wsd:AppSequence InstanceId=\"" << _instanceVersion << "\" MessageNumber=\"" << _messageNumber << "\"></wsd:AppSequence>";
				strout << "</soap:Header>";
				strout << "<soap:Body>";
				strout << "<wsd:Bye>";
				strout << "<wsa:EndpointReference>";
				strout << "<wsa:Address>" << _deviceUUID << "</wsa:Address>";
				strout << "</wsa:EndpointReference>";
				strout << "</wsd:Bye>";
				strout << "</soap:Body>";
				strout << "</soap:Envelope>";

				_messageNumber = _messageNumber + 1;
				byeMessage = strout.str();
			}
			void WSDiscoveryCore::handleIncomingMulticastMessage(std::string incomingMessage, bool& sendUnicastAnswer, std::string& answerMessage)
			{
#ifdef WSDISCOVERY_DEBUG
				std::cout << "WSDiscovery: Handle incoming message" << std::endl;
#endif
				std::shared_ptr<SPICE::BIG::IXMLParser> xmlParser = _coreData->getResourceProvider()->getNewXMLParserInstance();

				sendUnicastAnswer = false;

				// check document
				if(!xmlParser->loadXMLStructure(incomingMessage))
				{
					return;
				}

				if(!xmlParser->selectElementByNameAndNS("Header", "http://www.w3.org/2003/05/soap-envelope"))
				{
					return;
				}
				std::vector<std::string> headerList = xmlParser->getChildElementsList();
				

				// check headerTo
				std::string headerTo = xmlParser->getChildElementInnerText("To");
				if(headerTo != "urn:docs-oasis-open-org:ws-dd:ns:discovery:2009:01")
				{
					return;
				}

				// check headerAction
				std::string headerAction = xmlParser->getChildElementInnerText("Action");
				bool isProbe = headerAction == "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Probe";
				bool isResolve = headerAction == "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Resolve";
				if(!isProbe && !isResolve)
				{
					return;
				}

				// getMessageID
				std::string headerMessageId = xmlParser->getChildElementInnerText("MessageID");
				if(headerMessageId == "")
				{
					return;
				}

				// checkMessageID
				if(!updateAndCheckMessageIDMemory(headerMessageId))
				{
					return;
				}

				// getReplyTo
				std::string headerReplyTo = xmlParser->getChildElementInnerText("ReplyTo");
				if(headerReplyTo == "")
				{
					headerReplyTo = "http://www.w3.org/2005/08/addressing/anonymous";
				}

				if(!xmlParser->selectElementByNameAndNS("Body", "http://www.w3.org/2003/05/soap-envelope"))
				{
					return;
				}

				if(isProbe)
				{
#ifdef WSDISCOVERY_DEBUG
				std::cout << "WSDiscovery: is Probe" << std::endl;
#endif
					if(!xmlParser->selectChildByName("Probe"))
					{
						return;
					}
					std::string bodyTypes = xmlParser->getChildElementInnerText("Types");
					std::string bodyScopes = xmlParser->getChildElementInnerText("Scopes");
					if(bodyScopes != "" || (bodyTypes.find("SiLADevice") == std::string::npos && bodyTypes != ""))
					{
						return;
					}
					sendUnicastAnswer = true;
					createProbeMatchMessage(answerMessage, headerMessageId, headerReplyTo);
				}
				else // isResolve
				{
#ifdef WSDISCOVERY_DEBUG
				std::cout << "WSDiscovery: is Resolve" << std::endl;
#endif
					std::string bodyResolveEndpointReference = xmlParser->getFirstElementAtPathInnerText("//Envelope/Body/Resolve/EndpointReference/Address");
					if(bodyResolveEndpointReference != _deviceUUID)
					{
						return;
					}
					sendUnicastAnswer = true;
					createResolveMatchMessage(answerMessage, headerMessageId, headerReplyTo);
				}
			}
			std::string WSDiscoveryCore::getAddressTemplate()
			{
				return "http://ZZZ.ZZZ.ZZZ.ZZZ:YYY/URIPATHNAME";
			}

			std::string WSDiscoveryCore::generateRandomUUID()
			{
				std::ostringstream strout;

				strout << "urn:uuid:";
				strout << std::hex;

				for(int i = 0; i < 8; i++)
				{
					strout << _distribution(_generator);
				}
				strout << "-";
				for(int i = 0; i < 4; i++)
				{
					strout << _distribution(_generator);
				}
				strout << "-4";
				for(int i = 0; i < 3; i++)
				{
					strout << _distribution(_generator);
				}
				strout << "-";
				strout << (_distribution(_generator)%4 + 8);
				for(int i = 0; i < 3; i++)
				{
					strout << _distribution(_generator);
				}
				strout << "-";
				for(int i = 0; i < 12; i++)
				{
					strout << _distribution(_generator);
				}
				std::string debugUUID = strout.str();
				return debugUUID;
			}
			void WSDiscoveryCore::setDeviceUUID(std::string uuid)
			{
				_deviceUUID = uuid;
			}
			unsigned long WSDiscoveryCore::seedGenerator(unsigned long seed)
			{
				std::uniform_int_distribution<unsigned long> ulongDist(1, 4000000000UL);
				_generator.seed(seed);
				return ulongDist(_generator);
			}

			void WSDiscoveryCore::createProbeMatchMessage(std::string& probeMatchMessage, std::string relatingUUID, std::string replyTo)
			{
				std::ostringstream strout;
				strout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
//				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:sila=\"http://sila-standard.org/WSDiscovery\">";
				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wsd=\"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01\">";
				strout << "<soap:Header>";
//				strout << "<wsa:To>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>";
				strout << "<wsa:To>" << replyTo << "</wsa:To>";
//				strout << "<wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches</wsa:Action>";
				strout << "<wsa:Action>http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ProbeMatches</wsa:Action>";
				strout << "<wsa:MessageID>" << generateRandomUUID() << "</wsa:MessageID>";
				strout << "<wsa:RelatesTo>" << relatingUUID << "</wsa:RelatesTo>";
				strout << "<wsd:AppSequence InstanceId=\"" << _instanceVersion << "\" MessageNumber=\"" << _messageNumber << "\"></wsd:AppSequence>";
				strout << "</soap:Header>";
				strout << "<soap:Body>";
				strout << "<wsd:ProbeMatches>";
				strout << "<wsd:ProbeMatch>";
				strout << "<wsa:EndpointReference>";
				strout << "<wsa:Address>" << _deviceUUID << "</wsa:Address>";
				strout << "</wsa:EndpointReference>";
				strout << "<wsd:Types>SiLADevice</wsd:Types>";
//				strout << "<wsd:Scopes></wsd:Scopes>";
				strout << "<wsd:XAddrs>" << getAddressTemplate() << "</wsd:XAddrs>";
				strout << "<wsd:Scopes/>";
				strout << "<wsd:MetadataVersion>" << _metadataVersion << "</wsd:MetadataVersion>";
				strout << "</wsd:ProbeMatch>";
				strout << "</wsd:ProbeMatches>";
				strout << "</soap:Body>";
				strout << "</soap:Envelope>";

				_messageNumber = _messageNumber + 1;
				probeMatchMessage = strout.str();
			}
			void WSDiscoveryCore::createResolveMatchMessage(std::string& resolveMatchMessage, std::string relatingUUID, std::string replyTo)
			{
				std::ostringstream strout;
				strout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
//				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\" xmlns:wsd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\" xmlns:sila=\"http://sila-standard.org/WSDiscovery\">";
				strout << "<soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:wsa=\"http://www.w3.org/2005/08/addressing\" xmlns:wsd=\"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01\">";
				strout << "<soap:Header>";
//				strout << "<wsa:To>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</wsa:To>";
				strout << "<wsa:To>" << replyTo << "</wsa:To>";
//				strout << "<wsa:Action>http://schemas.xmlsoap.org/ws/2005/04/discovery/ResolveMatches</wsa:Action>";
				strout << "<wsa:Action>http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ResolveMatches</wsa:Action>";
				strout << "<wsa:MessageID>" << generateRandomUUID() << "</wsa:MessageID>";
				strout << "<wsa:RelatesTo>" << relatingUUID << "</wsa:RelatesTo>";
				strout << "<wsd:AppSequence InstanceId=\"" << _instanceVersion << "\" MessageNumber=\"" << _messageNumber << "\"></wsd:AppSequence>";
				strout << "</soap:Header>";
				strout << "<soap:Body>";
				strout << "<wsd:ResolveMatches>";
				strout << "<wsd:ResolveMatch>";
				strout << "<wsa:EndpointReference>";
				strout << "<wsa:Address>" << _deviceUUID << "</wsa:Address>";
				strout << "</wsa:EndpointReference>";
				strout << "<wsd:Types>sila:SiLADevice</wsd:Types>";
//				strout << "<wsd:Scopes></wsd:Scopes>";
				strout << "<wsd:XAddrs>" << getAddressTemplate() << "</wsd:XAddrs>";
				strout << "<wsd:MetadataVersion>" << _metadataVersion << "</wsd:MetadataVersion>";
				strout << "</wsd:ResolveMatch>";
				strout << "</wsd:ResolveMatches>";
				strout << "</soap:Body>";
				strout << "</soap:Envelope>";
			}
			bool WSDiscoveryCore::updateAndCheckMessageIDMemory(std::string messageID)
			{
				time_t now = time(nullptr);
				double timediff =  difftime(now, _lastChangedTime);
				timediff = timediff / 60;
				if(timediff > 5)
				{
					timediff = 5;
				}
				int intTimeDiff = (int)timediff;

				if(intTimeDiff >= 1)
				{
					_lastChangedTime = now;
				}

				for(int i = 0; i < intTimeDiff; i++)
				{
					_currentActiveMessageIDMemory += 1;
					if(_currentActiveMessageIDMemory >= 5)
					{
						_currentActiveMessageIDMemory = 0;
					}
					_messageIDMemory[_currentActiveMessageIDMemory].clear();
				}

				for(int i = 0; i < 5; i++)
				{
					for(std::vector<std::string>::iterator j = _messageIDMemory[i].begin(); j != _messageIDMemory[i].end(); j++)
					{
						if((*j) == messageID)
						{
							return false;
						}
					}
				}
				_messageIDMemory[_currentActiveMessageIDMemory].push_back(messageID);
				return true;
			}

		}
	}
}