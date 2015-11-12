/**
	SPICE ES POCO
	EthernetServer.h
	Purpose: Central class of the POCO ethernet server implementation for SPICE

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef ETHERNETSERVER_H
#define ETHERNETSERVER_H

#include "CommandConnectionFactory.h"
#include "WSDServer.h"

#include "IEthernetServer.h"

#include "Poco/Net/HTTPServer.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			class EthernetServer : public SPICE::BIG::IEthernetServer
			{
				public:
					/**
						Central class of the POCO ethernet server implementation for SPICE
					*/
					EthernetServer();
					virtual ~EthernetServer();

					/**
						Starts the server for the given port.

						@param port Port to listen on for the server
						@return Returns 0 if server could start successful
					*/
					int startServer(unsigned int port = 80);

					/**
						Stops the server
					*/
					void stopServer();

			// ---------- IEthernetServer -------------------
					/**
						Provides a registration for a service on a uri with a given ethernetConnectionCallback if a request for the uri is received.

						@param uri URI to listen on
						@param ethernetConnectionCallback Callback class for an incoming connection to this URI
						@return True if the uri could be registred. False if callback is null or the uri is already in use.
					*/
					virtual bool registerEthernetConnectionCallback(std::string uri, std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback);
					/**
						Provides a registration for a service on a uri with a given ethernetConnectionCallback if a request for the uri is received. Additionally a WSDiscovery-Server is started using the given wsDiscoveryCallback.

						@param uri URI to listen on
						@param ethernetConnectionCallback Callback class for an incoming connection to this URI
						@param wsDiscoveryCallback Callback for a created ws-discovery-server to be able to call the core depending discovery messages.
						@return True if the uri could be registred. False if callback is null or the uri is already in use.
					*/
					virtual bool registerEthernetConnectionCallback(std::string uri, std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback, std::shared_ptr<SPICE::BIG::IWSDiscoveryCallback> wsDiscoveryCallback);
					/**
						To unregister already registred URIs.

						@param uriPath URI to unregister
						@return True if there was an URI and could be unregistred
					*/
					virtual bool unregisterEthernetConnectionCallback(std::string uriPath);

				protected:
				private:

					CommandConnectionFactory* _commandConnectionFactory;
					Poco::Net::HTTPServer* _httpServer;
					Poco::Net::ServerSocket _socket;
					std::map<std::string, std::shared_ptr<WSDServer>> _wsdServerMap;
					unsigned int _port;
			};
		}
	}
}
#endif // ETHERNETSERVER_H
