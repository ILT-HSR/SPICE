/**
	SPICE ES POCO
	CommandConnection.h
	Purpose: Class to provide a factory to create instances of connection handlers (CommandConnections)

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef COMMANDCONNECTIONFACTORY_H
#define COMMANDCONNECTIONFACTORY_H

#include <memory>
#include <string>
#include <map>

#include "EthernetEventConnectionFactory.h"

#include "IEthernetConnectionCallback.h"

#include "Poco/Net/HTTPServerConnectionFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			class CommandConnectionFactory : public Poco::Net::HTTPRequestHandlerFactory
			{
				public:
					/**
						Class to provide a factory to create instances of connection handlers (CommandConnections)
					*/
					CommandConnectionFactory();
					virtual ~CommandConnectionFactory();

					/**
						Provides a registration for a service on a uri with a given ethernetConnectionCallback if a request for the uri is received.

						@param uriPath URI to listen on
						@param ethernetConnectionCallback Callback class for an incoming connection to this URI
						@return True if the uri could be registred. False if callback is null or the uri is already in use.
					*/
					virtual bool registerEthernetConnectionCallback(std::string uriPath, std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback);
					/**
						To unregister already registred URIs.

						@param uriPath URI to unregister
						@return True if there was an URI and could be unregistred
					*/
					virtual bool unregisterEthernetConnectionCallback(std::string uriPath);

			// ------- HTTPRequestHandlerFactory ----------
					/**
						Implementation of the by poco needed createRequestHandler

						@param request Reference to the request
						@return Pointer to the generated request handler
					*/
                    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request);

				protected:
				private:
					std::map<std::string, std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback>> _ethernetConnectionCallbackMap;
					std::shared_ptr<EthernetEventConnectionFactory> _eventConnectionFactory;
			};
		}
	}
}
#endif // COMMANDCONNECTIONFACTORY_H
