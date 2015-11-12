/**
	SPICE BIG
	IEthernetServer.h
	Purpose: An interface for an ethernet server. Diffrent services (as example the core) can be registred at the server (with IEthernetConnectionCallback).
	Gives the possibility to change the used libraries for a server.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IETHERNETSERVER_H
#define IETHERNETSERVER_H

#include <string>
#include <memory>

#include "IEthernetConnectionCallback.h"
#include "IWSDiscoveryCallback.h"

namespace SPICE
{
	namespace BIG
	{
		class IEthernetServer
		{
			public:
				/**
					Provides a registration for a service on a uri with a given ethernetConnectionCallback if a request for the uri is received.

					@param uri URI to listen on
					@param ethernetConnectionCallback Callback class for an incoming connection to this URI
					@return True if the uri could be registred. False if callback is null or the uri is already in use.
				*/
				virtual bool registerEthernetConnectionCallback(std::string uri, std::shared_ptr<IEthernetConnectionCallback> ethernetConnectionCallback) = 0;
				/**
					Provides a registration for a service on a uri with a given ethernetConnectionCallback if a request for the uri is received. Additionally a WSDiscovery-Server is started using the given wsDiscoveryCallback.

					@param uri URI to listen on
					@param ethernetConnectionCallback Callback class for an incoming connection to this URI
					@param wsDiscoveryCallback Callback for a created ws-discovery-server to be able to call the core depending discovery messages.
					@return True if the uri could be registred. False if callback is null or the uri is already in use.
				*/
				virtual bool registerEthernetConnectionCallback(std::string uri, std::shared_ptr<IEthernetConnectionCallback> ethernetConnectionCallback, std::shared_ptr<IWSDiscoveryCallback> wsDiscoveryCallback) = 0;
				/**
					To unregister already registred URIs.

					@param uri URI to unregister
					@return True if there was an URI and could be unregistred
				*/
				virtual bool unregisterEthernetConnectionCallback(std::string uri) = 0;
		};
	}
}
#endif // IETHERNETSERVER_H
