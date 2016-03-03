/**
	SPICE ES POCO
	EthernetServer.cpp
	Purpose: Central class of the POCO ethernet server implementation for SPICE

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "EthernetServer.h"

#include <iostream>

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			EthernetServer::EthernetServer() :
				_httpServer(nullptr),
				_port(0)
			{
				_commandConnectionFactory = new CommandConnectionFactory();
				//ctor
			}
			EthernetServer::~EthernetServer()
			{
				//dtor
				delete _commandConnectionFactory;
                if(_httpServer != nullptr)
				{
                    delete _httpServer;
				}
			}

			int EthernetServer::startServer(unsigned int port)
			{
                if(_httpServer == nullptr)
				{
                    try
					{
						_port = port;
						_socket = Poco::Net::ServerSocket(port);
						_httpServer = new Poco::Net::HTTPServer(_commandConnectionFactory, _socket, new Poco::Net::HTTPServerParams());
                        _httpServer->start(); 
						return 0;
					}
                    catch(std::exception e)
					{
						std::cerr << "Exception EthernetServer: " << e.what() << std::endl;
                        return 1;
					}
				}
				else
				{
                    return 2;
				}
			}

			void EthernetServer::stopServer()
			{
                if(_httpServer != nullptr)
				{
					_httpServer->stopAll();
				}
			}

			bool EthernetServer::registerEthernetConnectionCallback(std::string uri, std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback)
			{
				if(_commandConnectionFactory->registerEthernetConnectionCallback(uri, ethernetConnectionCallback))
				{
					std::cout << "Listening on: http://<ip-address>:" << _port << uri << std::endl;
					std::cout << "WSDL-file on: http://<ip-address>:" << _port << uri << "?wsdl" << std::endl;
					return true;
				}
				return false;

			}
			bool EthernetServer::registerEthernetConnectionCallback(std::string uri, std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback, std::shared_ptr<SPICE::BIG::IWSDiscoveryCallback> wsDiscoveryCallback)
			{
				if(_commandConnectionFactory->registerEthernetConnectionCallback(uri, ethernetConnectionCallback))
				{
					std::cout << "Listening on: http://<ip-address>:" << _port << uri << std::endl;
					std::cout << "WSDL-file on: http://<ip-address>:" << _port << uri << "?wsdl" << std::endl;
					std::shared_ptr<WSDServer> newWSDServer(new WSDServer(_port, uri, wsDiscoveryCallback));
					newWSDServer->startServer();
					_wsdServerMap[uri] = newWSDServer;
					return true;
				}
				return false;
			}
			bool EthernetServer::unregisterEthernetConnectionCallback(std::string uriPath)
			{
				if(_commandConnectionFactory->unregisterEthernetConnectionCallback(uriPath))
				{
					std::map<std::string, std::shared_ptr<WSDServer>>::iterator it = _wsdServerMap.find(uriPath);
					if(it != _wsdServerMap.end())
					{
						it->second->stopServer();
						_wsdServerMap.erase(it);
					}
					return true;
				}
				return false;
			}

		}
	}
}