/**
	SPICE ES POCO
	HTTPNotFoundConnection.cpp
	Purpose: Class to handle not found connections

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "HTTPNotFoundConnection.h"

#include <sstream>

#include "Poco/StreamCopier.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			HTTPNotFoundConnection::HTTPNotFoundConnection() : HTTPRequestHandler()
			{
				//ctor
			}
			HTTPNotFoundConnection::~HTTPNotFoundConnection()
			{
				//dtor
			}

			void HTTPNotFoundConnection::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
			{
				try
				{
					// Daten empfangen
					std::istream& inStream = request.stream();

					std::ostringstream requestStream;
					Poco::StreamCopier::copyStream(inStream, requestStream);

					// Antwort senden
					response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
					response.setContentType("text/xml; charset=utf-8");
					response.setContentLength(0);

					std::ostream& output = response.send();
					output.flush();
				}
				catch (...)
				{
					try
					{
						// Antwort senden
						response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
						response.setContentType("text/xml; charset=utf-8");
						response.setContentLength(0);

						std::ostream& output = response.send();
						output.flush();
					}
					catch (...)
					{
						throw;
					}
				}
			}

		}
	}
}