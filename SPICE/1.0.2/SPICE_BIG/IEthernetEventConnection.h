/**
	SPICE BIG
	IEthernetEventConnection.h
	Purpose: Interface which provides the core a factory to create new instances of an IEthernetEventConnection to send events.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IETHERNETEVENTCONNECTION_H
#define IETHERNETEVENTCONNECTION_H

#include <string>

namespace SPICE
{
	namespace BIG
	{
		class IEthernetEventConnection
		{
			public:
				/**
					Sets the needed informations for the connection.

					@param eventReceiverURI The destination event receiver URI
					@param content	The xml soap message to send.
					@param soapAction The SOAP action inclusive the silaNamespace (example: www.sila-standard.org/GetStatus )
				*/
				virtual void setEventInformations(std::string eventReceiverURI, std::string content, std::string soapAction) = 0;
				/**
					Start the event connection communication

					@return Returns 1 if connection could be started successful. Other codes according to the specific implementation.
				*/
				virtual int start() = 0;
				/**
					Returns if the communication is finished. Doesn't gives information if it was successful.

					@return True if communication is finished. 
				*/
				virtual bool hasFinished() = 0;
				/**
					Returns if the communication had an error. Is requested when the connection is marked as finished.

					@return True if there has been an error.
				*/
				virtual bool hadError() = 0;
				/**
					Returns an error message if there has been an error.

					@return Human readable error message.
				*/
				virtual std::string getErrorMessage() = 0;
				/**
					Returns the content of the response.

					@return Content of the response.
				*/
				virtual std::string getResponseContent() = 0;
		};
	}
}
#endif // IETHERNETEVENTCONNECTION_H
