/**
	SPICE ES POCO
	HTTPNotFoundConnection.h
	Purpose: Class to handle not found connections

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef HTTPNOTFOUNDCONNECTION_H
#define HTTPNOTFOUNDCONNECTION_H

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			class HTTPNotFoundConnection : public Poco::Net::HTTPRequestHandler
			{
				public:
					/**
						Class to handle not found connections
					*/
					HTTPNotFoundConnection();
					virtual ~HTTPNotFoundConnection();

			// ------- HTTPRequestHandler ----------
					/**
						Implementation of the by poco needed handleRequest

						@param request Data of the request
						@param response Data to send as response
					*/
					void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

				protected:
				private:

			};
		}
	}
}
#endif // HTTPNOTFOUNDCONNECTION_H
