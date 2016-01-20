/**
	SPICE ES POCO
	WSDServer.cpp
	Purpose: ServerClass for WebService-Discovery using POCO. Listening for Multicast on Port 3702, checking for ethernet interface changes and trigger corresponding messages.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "WSDServer.h"

#include <sstream>
#include <iostream>

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			WSDServer::WSDServer(unsigned int ethernetServerPort, std::string uriPath, std::shared_ptr<SPICE::BIG::IWSDiscoveryCallback> wsDiscoveryCallback) :
				_wsDiscoveryCallback(wsDiscoveryCallback),
				_ethernetServerPort(ethernetServerPort),
				_uriPath(uriPath),
				_listeningThread(nullptr),
				_internalCheckThread(nullptr),
				_stopping(false),
				_multicastAddress("239.255.255.250", 3702),
				_listeningSocket()
			{
				//ctor
			}
			WSDServer::~WSDServer()
			{
				//dtor
				if(_listeningThread != nullptr || _internalCheckThread != nullptr)
				{
					stopServer();
				}
			}

			void WSDServer::startServer()
			{
				if(_listeningThread == nullptr)
				{
					try
					{
						_listeningSocket = Poco::Net::MulticastSocket(Poco::Net::SocketAddress(Poco::Net::IPAddress(), _multicastAddress.port()), true);
						_stopping = false;
						_listeningThread = new std::thread(&WSDServer::listeningMulticastCycle, this);
						_internalCheckThread = new std::thread(&WSDServer::checkForEthernetChangesCycle, this);
					}
					catch(std::exception e)
					{
						stopServer();
					}
				}
			}

			void WSDServer::stopServer()
			{
				_stopping = true;
				if(_listeningThread != nullptr)
				{
					_listeningSocket.close();
					if(_listeningThread->joinable())
					{
						_listeningThread->join();
					}
					delete _listeningThread;
					_listeningThread = nullptr;
				}
				if(_internalCheckThread != nullptr)
				{
					if(_internalCheckThread->joinable())
					{
						_internalCheckThread->join();
					}
					delete _internalCheckThread;
					_internalCheckThread = nullptr;
				}
				

				std::string byeMessage = "";
				_wsDiscoveryCallback->createByeMessage(byeMessage);
				Poco::Net::NetworkInterface::NetworkInterfaceList netList = Poco::Net::NetworkInterface::list();
				for(std::map<std::string, std::string>::iterator i = _detectedInterfacesAndIPs.begin(); i != _detectedInterfacesAndIPs.end(); i++)
				{
					for(Poco::Net::NetworkInterface::NetworkInterfaceList::iterator j = netList.begin(); j != netList.end(); j++)
					{
						if(j->name().compare(i->first) == 0)
						{
							sendMulticastMessage(*j, byeMessage);
							j = netList.end();
						}
					}
				}
			}

			void WSDServer::listeningMulticastCycle()
			{
				Poco::Net::SocketAddress sender;
				char buffer[8192];
				int receivedCount = 0;

				while(true)
				{
					try
					{
						receivedCount = _listeningSocket.receiveFrom(buffer, sizeof(buffer), sender);

						std::string incomingMessage = "";
						incomingMessage.append(buffer, receivedCount);
						bool sendAnswer = false;
						std::string answerMessage = "";
#ifdef WSDISCOVERY_DEBUG
									std::cout << "WSDiscovery: Handle incoming message" << std::endl;
#endif
						_wsDiscoveryCallback->handleIncomingMulticastMessage(incomingMessage, sendAnswer, answerMessage);

						if(sendAnswer)
						{
							std::string replaceAddress = "";
							Poco::Net::NetworkInterface::NetworkInterfaceList netList = Poco::Net::NetworkInterface::list();
							for(Poco::Net::NetworkInterface::NetworkInterfaceList::iterator i = netList.begin(); i != netList.end(); i++)
							{
								if(i->supportsIPv4())
								{
									Poco::Net::IPAddress adapterNetID(i->address());
									adapterNetID.mask(i->subnetMask());
									Poco::Net::IPAddress senderNetID(sender.host());
									senderNetID.mask(i->subnetMask());
									if(adapterNetID.toString() == senderNetID.toString())
									{
										replaceAddress = getReplaceAddress(i->address().toString());
									}
								}
							}
							if(replaceAddress != "")
							{
								std::string addressTemplate = _wsDiscoveryCallback->getAddressTemplate();
								while(answerMessage.find(addressTemplate) != std::string::npos)
								{
									answerMessage.replace(answerMessage.find(addressTemplate), addressTemplate.length(), replaceAddress);
								}
#ifdef WSDISCOVERY_DEBUG
								std::cout << "WSDiscovery: Send answer" << std::endl;
#endif
								_listeningSocket.sendTo(&answerMessage[0], answerMessage.length(), sender);
							}
						}
					}
					catch (std::exception e)
					{
						if(_stopping)
						{
							return;
						}
					}
				}
			}

			void WSDServer::checkForEthernetChangesCycle()
			{
				while(true)
				{
					// waiting 10s
					for(int i = 0; i < 50; i++)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(200));
						if(_stopping)
						{
							return;
						}
					}

					// searching for new interfaces and ip changes
					std::string helloMessage = "";
					Poco::Net::NetworkInterface::NetworkInterfaceList netList = Poco::Net::NetworkInterface::list();
					std::list<std::string> currentInterfaces;
					for(Poco::Net::NetworkInterface::NetworkInterfaceList::iterator i = netList.begin(); i != netList.end(); i++)
					{
						if(i->supportsIPv4())
						{
							currentInterfaces.push_back(i->name());
							std::map<std::string, std::string>::iterator existingInterface = _detectedInterfacesAndIPs.find(i->name());
							if(existingInterface != _detectedInterfacesAndIPs.end())
							{
								if(i->address().toString() != existingInterface->second)
								{
#ifdef WSDISCOVERY_DEBUG
									std::cout << "WSDiscovery: IP changed on interface: " << existingInterface->first << " to: " << i->address().toString() << std::endl;
#endif
									// ip changed
									existingInterface->second = i->address().toString();
									if(helloMessage == "")
									{
										_wsDiscoveryCallback->createHelloMessage(helloMessage);
									}
									sendMulticastMessage(*i, helloMessage);
								}
							}
							else
							{
#ifdef WSDISCOVERY_DEBUG
								std::cout << "WSDiscovery: New interface: " << i->name() << std::endl;
#endif
								// new network interfaces
								_detectedInterfacesAndIPs[i->name()] = i->address().toString();
								if(helloMessage == "")
								{
									_wsDiscoveryCallback->createHelloMessage(helloMessage);
								}
								if(std::find(_groupAddedInterfaces.begin(), _groupAddedInterfaces.end(), i->name()) == _groupAddedInterfaces.end())
								{
									_listeningSocket.joinGroup(_multicastAddress.host(), *i);
									_groupAddedInterfaces.push_back(i->name());
								}
								sendMulticastMessage(*i, helloMessage);
							}
						}
					}
					// remove lost interfaces
					while(_detectedInterfacesAndIPs.size() > currentInterfaces.size())
					{
						for(std::map<std::string, std::string>::iterator i = _detectedInterfacesAndIPs.begin(); i != _detectedInterfacesAndIPs.end(); i++)
						{
							if(std::find(currentInterfaces.begin(), currentInterfaces.end(), i->first) == currentInterfaces.end())
							{
#ifdef WSDISCOVERY_DEBUG
								std::cout << "WSDiscovery: Lost interface: " << i->first << std::endl;
#endif
								_detectedInterfacesAndIPs.erase(i);
								i = _detectedInterfacesAndIPs.end();
							}
						}
					}

				}
			}

			void WSDServer::sendMulticastMessage(Poco::Net::NetworkInterface& networkInterface, std::string message)
			{
				// prepare message
				std::string replaceAddress = getReplaceAddress(networkInterface.address().toString());
				std::string addressTemplate = _wsDiscoveryCallback->getAddressTemplate();
				while(message.find(addressTemplate) != std::string::npos)
				{
					message.replace(message.find(addressTemplate), addressTemplate.length(), replaceAddress);
				}

				// prepare socket
				Poco::Net::SocketAddress destAddress("239.255.255.250", 3702);
				Poco::Net::MulticastSocket ms;
				ms.setInterface(networkInterface);

				// send
				ms.sendTo(&message[0], message.length(), destAddress);
#ifdef WSDISCOVERY_DEBUG
				std::cout << "WSDiscovery: Send message to interface: " << networkInterface.name() << std::endl;
#endif

				// close
				ms.close();
			}

			std::string WSDServer::getReplaceAddress(std::string ipAddress)
			{
				std::ostringstream strout;
				strout << "http://" << ipAddress + ":" << _ethernetServerPort << _uriPath << "?wsdl";
				return strout.str();
			}

		}
	}
}