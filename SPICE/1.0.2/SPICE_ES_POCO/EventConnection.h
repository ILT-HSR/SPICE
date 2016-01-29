/**
	SPICE ES POCO
	EventConnection.h
	Purpose: Class to provide poco specific outgoing connections (EventConnection)

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef EVENTCONNECTION_H
#define EVENTCONNECTION_H

#include <string>
#include <thread>
#include <atomic>

#include "IEthernetEventConnection.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			class EventConnection : public SPICE::BIG::IEthernetEventConnection
			{
				public:
					/**
						Class to provide a factory for the poco specific outgoing connections (EventConnection)
					*/
					EventConnection();
					virtual ~EventConnection();

			// ---------- IEthernetEventConnection -------------------
					/**
						Sets the needed informations for the connection.

						@param eventReceiverURI The destination event receiver URI
						@param content	The xml soap message to send.
						@param soapAction The SOAP action inclusive the silaNamespace (example: www.sila-standard.org/GetStatus )
					*/
					virtual void setEventInformations(std::string eventReceiverURI, std::string content, std::string soapAction);
					/**
						Start the event connection communication

						@return Returns 1 if connection could be started successful. Other codes according to the specific implementation.
					*/
					virtual int start();
					/**
						Returns if the communication is finished. Doesn't gives information if it was successful.

						@return True if communication is finished. 
					*/
					virtual bool hasFinished();
					/**
						Returns if the communication had an error. Is requested when the connection is marked as finished.

						@return True if there has been an error.
					*/
					virtual bool hadError();
					/**
						Returns an error message if there has been an error.

						@return Human readable error message.
					*/
					virtual std::string getErrorMessage();
					/**
						Returns the content of the response.

						@return Content of the response.
					*/
					virtual std::string getResponseContent();

				protected:
				private:
					/**
						Internal main process to execute the event connection
					*/
					void internalProcess();

					std::string _eventReceiverURI;
					std::string _soapAction;
					std::string _content;

					std::string _errorMessage;
					std::string _responseContent;

					std::thread* _eventConnectionThread;

					std::atomic<bool> _finished;
					std::atomic<bool> _error;

			};
		}
	}
}
#endif // EVENTCONNECTION_H
