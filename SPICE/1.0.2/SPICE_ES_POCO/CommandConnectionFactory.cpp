/**
	SPICE ES POCO
	CommandConnectionFactory.cpp
	Purpose: Class to provide a factory to create instances of connection handlers (CommandConnections)

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "CommandConnectionFactory.h"

#include "Poco/URI.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "CommandConnection.h"
#include "HTTPNotFoundConnection.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			CommandConnectionFactory::CommandConnectionFactory() : HTTPRequestHandlerFactory(),
				_eventConnectionFactory(std::shared_ptr<EthernetEventConnectionFactory>(new EthernetEventConnectionFactory()))
			{
				//ctor
			}
			CommandConnectionFactory::~CommandConnectionFactory()
			{
				//dtor
			}

			bool CommandConnectionFactory::registerEthernetConnectionCallback(std::string uriPath, std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback)
			{
				if(_ethernetConnectionCallbackMap.find(uriPath) == _ethernetConnectionCallbackMap.end()) // TODO: Test if this is correct working
				{
					_ethernetConnectionCallbackMap[uriPath] = ethernetConnectionCallback;
					return true;
				}
				else
				{
					return false;
				}
			}
			bool CommandConnectionFactory::unregisterEthernetConnectionCallback(std::string uriPath)
			{
				if(_ethernetConnectionCallbackMap.find(uriPath) != _ethernetConnectionCallbackMap.end()) // TODO: Test if this is correct working
				{
					_ethernetConnectionCallbackMap.erase(_ethernetConnectionCallbackMap.find(uriPath));
					return true;
				}
				else
				{
					return false;
				}
			}

            Poco::Net::HTTPRequestHandler* CommandConnectionFactory::createRequestHandler(const Poco::Net::HTTPServerRequest& request)
			{
				Poco::URI uri(request.getURI());

				std::string path = uri.getPath();
				std::string query = uri.getQuery();
				std::string pathAndQuery = uri.getPathAndQuery();

				if(_ethernetConnectionCallbackMap.find(path) != _ethernetConnectionCallbackMap.end())
				{
					return new CommandConnection(_ethernetConnectionCallbackMap.at(path), _eventConnectionFactory);
				}
				else
				{
					return new HTTPNotFoundConnection();

					// TODO: implement service overview
				}

			}


		}
	}
}