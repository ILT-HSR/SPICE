/**
	SPICE ES POCO
	CommandConnection.cpp
	Purpose: Class to handle an incoming connection

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "CommandConnection.h"

#include <sstream>
#include <string>

#include "Poco/StreamCopier.h"
#include "Poco/URI.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			CommandConnection::CommandConnection(std::shared_ptr<SPICE::BIG::IEthernetConnectionCallback> ethernetConnectionCallback, std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> eventConnectionFactory) : HTTPRequestHandler(),
				_ethernetConnectionCallback(ethernetConnectionCallback),
				_eventConnectionFactory(eventConnectionFactory)
			{
				//ctor
			}
			CommandConnection::~CommandConnection()
			{
				//dtor
			}

			void CommandConnection::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
			{
				try
				{
					// Daten empfangen
					std::istream& inStream = request.stream();
					std::string soapAction = "";
					if(request.has("SOAPAction"))
					{
						soapAction = request.get("SOAPAction");
						while(soapAction.find("\"") != std::string::npos)
						{
							soapAction.erase(soapAction.find("\""), 1);
						}
					}

					Poco::URI uri(request.getURI());

					std::string requestContent = "";
					std::ostringstream requestStream;
					Poco::StreamCopier::copyStream(inStream, requestStream);
					requestContent = requestStream.str();

					// Daten verarbeit lassen
					std::string responseSOAPAction = "";
					std::string responseContent = "";
					std::ostringstream strout;
					strout << request.serverAddress().toString();
					if(_ethernetConnectionCallback->handleEthernetConnection(uri.getPath(), uri.getQuery(), strout.str(), soapAction, requestContent, responseSOAPAction, responseContent, _eventConnectionFactory))
					{
						// Antwort senden
						response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
						response.setContentType("text/xml; charset=utf-8");
						response.setContentLength(responseContent.length());
						if(responseSOAPAction != "")
						{
							response.set("SOAPAction", "\"" + responseSOAPAction + "\"");
						}

						std::ostream& output = response.send();
						output << responseContent;
						output.flush();
					}
					else // could not be handled -> bad request
					{
						// Antwort senden
						response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
						response.setContentType("text/xml; charset=utf-8");
						response.setContentLength(0);

						std::ostream& output = response.send();
						output.flush();
					}

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