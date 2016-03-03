/**
	SPICE ES POCO
	CommandConnection.h
	Purpose: Class to handle an incoming connection

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef COMMANDCONNECTION_H
#define COMMANDCONNECTION_H

#include "IEthernetConnectionCallback.h"

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			class CommandConnection : public Poco::Net::HTTPRequestHandler
			{
				public:
					/**
						Class to handle an incoming connection

						@param ethernetConnectionCallback Callback to the registred listener (normaly the core)
						@param eventConnectionFactory Factory to create eventConnections for commands.
					*/
					CommandConnection(std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback, std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> eventConnectionFactory);
					virtual ~CommandConnection();

			// ------- HTTPRequestHandler ----------
					/**
						Implementation of the by poco needed handleRequest

						@param request Data of the request
						@param response Data to send as response
					*/
					void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

				protected:
				private:
					std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> _ethernetConnectionCallback;
					std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> _eventConnectionFactory;

			};
		}
	}
}
#endif // COMMANDCONNECTION_H
