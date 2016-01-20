/**
	SPICE Core
	ConnectionHandler.h
	Purpose: Class to handle incoming connections

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <memory>
#include <string>

#include "IEthernetConnectionCallback.h"
#include "CommandHandler.h"
#include "WSDLGenerator.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class ConnectionHandler : public SPICE::BIG::IEthernetConnectionCallback
			{
				public:
					/**
						Class to handle incoming connections

						@param commandHandler The command handler to submit new commands
					*/
					ConnectionHandler(std::shared_ptr<CommandHandler> commandHandler);
					virtual ~ConnectionHandler();

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
						std::string content, std::string& responseSOAPAction, std::string& responseContent, std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> ethernetEventConnectionFactory);

					/**
						Prepares a vector with all commands and starts the generation of the WSDL file. Should be executed as preparation to be able to handle WSDL Connections.
					*/
					void prepareWSDLFile();
				protected:
				private:

					/**
						Handles an incomming command connection. Executes the needed checks and creates the according async command if possible.

						@param serverAddress The serverAddress on which the request was received.
						@param silaCommand The name of the command delivered over the SOAP-Action
						@param silaNamespace The namespace of the sila soap elements, given with the SOAPAction.
						@param content A string with the content of the connection.
						@param responseSOAPAction Gives the handler a possibility to set a SOAPAction for the response.
						@param responseContent The responseContent which should be set by the handler.
						@param ethernetEventConnectionFactory The eventConnectionFactory of the connection receiving ethernet server. Will be given to created asynchronous commands.
					*/
					void handleCommandConnection(std::string serverAddress, std::string silaCommand, std::string silaNamespace,
						std::string content, std::string& responseSOAPAction, std::string& responseContent, std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> ethernetEventConnectionFactory);

					/**
						Handles an incomming command connection. Executes the needed checks and creates the according async command if possible.

						@param serverAddress The serverAddress on which the request was received.
						@param responseContent The responseContent which should be set by the handler. In this case the WSDL.xml
					*/
					void handleWSDLConnection(std::string serverAddress, std::string& responseContent); 

					/**
						Checks if the state machine is locked and if yes wheter the lockId matches

						@param lockId The lockId to check
						@return True if ok
					*/
					bool checkLockIdOk(std::string& lockId);
					/**
						Checks the request id

						@param requestIdString requestId as string
						@param requestId requestId as int
						@return True if ok
					*/
					bool checkRequestIdOk(std::string& requestIdString, int& requestId);

					std::shared_ptr<CommandHandler> _commandHandler;
					std::shared_ptr<WSDLGenerator> _wsdlGenerator;
			};
		}
	}
}
#endif // CONNECTIONHANDLER_H
