/**
	SPICE BIG
	IEthernetConnectionCallback.h
	Purpose: Interface which is implemented by the core and can be implemented by other services to be able to register them at an IEthernetServer. Has to handle an incoming connection.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IETHERNETCONNECTIONCALLBACK_H
#define IETHERNETCONNECTIONCALLBACK_H

#include <string>
#include <memory>

#include "IEthernetEventConnectionFactory.h"

namespace SPICE
{
	namespace BIG
	{
		class IEthernetConnectionCallback
		{
			public:
				/**
					Handles an incoming connection. Handling must be synchronous.

					@param uriPath The path-part of the URI of the request.
					@param uriParameters The parameters of the URI of the request.
					@param serverAddress The serverAddress on which the request was received.
					@param soapAction The SoapAction if there is one. Otherwise string will be empty.
					@param content A string with the content of the connection.
					@param responseSOAPAction Gives the handler a possibility to set a SOAPAction for the response.
					@param responseContent The responseContent which should be set by the handler.
					@param ethernetEventConnectionFactory The eventConnectionFactory of the connection receiving ethernet server. Will be given to created asynchronous commands.
					@return Returns true if the request could be handled, false if not -> Throw 400 Bad Request
				*/
				virtual bool handleEthernetConnection(std::string uriPath, std::string uriParameters, std::string serverAddress, std::string soapAction,
					std::string content, std::string& responseSOAPAction, std::string& responseContent, std::shared_ptr<IEthernetEventConnectionFactory> ethernetEventConnectionFactory) = 0;
		};
	}
}
#endif // IETHERNETCONNECTIONCALLBACK_H
