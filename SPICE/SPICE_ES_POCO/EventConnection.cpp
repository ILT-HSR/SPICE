/**
	SPICE ES POCO
	EventConnection.cpp
	Purpose: Class to provide poco specific outgoing connections (EventConnection)

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "EventConnection.h"

#include <sstream>
#include <system_error>

#include "Poco/URI.h"
#include "Poco/Timespan.h"
#include "Poco/StreamCopier.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			EventConnection::EventConnection() :
				_eventReceiverURI(""),
				_soapAction(""),
				_content(""),
				_errorMessage(""),
				_responseContent(""),
				_eventConnectionThread(nullptr),
				_finished(false),
				_error(false)
			{
				//ctor
			}
			EventConnection::~EventConnection()
			{
				//dtor
                if(_eventConnectionThread != nullptr)
				{
					if(_eventConnectionThread->joinable())
					{
						_eventConnectionThread->join();
					}
                    delete _eventConnectionThread;
					_eventConnectionThread = nullptr;
				}
			}

			void EventConnection::setEventInformations(std::string eventReceiverURI, std::string content, std::string soapAction)
			{
				_eventReceiverURI = eventReceiverURI;
				_content = content;
				_soapAction = soapAction;
			}
			int EventConnection::start()
			{
                if(_eventConnectionThread == nullptr)
				{
                    try
					{
                        _eventConnectionThread = new std::thread(&EventConnection::internalProcess, this);
					}
                    catch(std::system_error e)
					{
                        return 1;
					}
                    return 0;
				}
				else
				{
                    return 2;
				}

			}
			bool EventConnection::hasFinished()
			{
				return _finished;
			}
			bool EventConnection::hadError()
			{
				return _error;
			}
			std::string EventConnection::getErrorMessage()
			{
				return _errorMessage;
			}
			std::string EventConnection::getResponseContent()
			{
				return _responseContent;
			}
			void EventConnection::internalProcess()
			{
				try
				{
					Poco::URI uri(_eventReceiverURI);

                    // prepare session
					Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
        		    session.setKeepAlive(false);

					if(_soapAction.find("ErrorEvent") != std::string::npos)
					{
						session.setTimeout(Poco::Timespan(10,0,0,0,0));
					}
                
                    // prepare request
                    std::string path (uri.getPathAndQuery());
				    if(path.empty())
				    {
                        path = "/";
				    }
					Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPMessage::HTTP_1_1);
				    request.setContentLength(_content.length());
					request.setContentType("text/xml; charset=utf-8");
					request.setKeepAlive(false);

                    request.set("SOAPAction","\"" + _soapAction + "\"");

                    // send request
                    std::ostream& outStream = session.sendRequest(request);
                    outStream << _content;

                    // get response
                    Poco::Net::HTTPResponse response;
                    std::istream& inStream = session.receiveResponse(response);

					std::ostringstream responseContentStream;
					Poco::StreamCopier::copyStream(inStream, responseContentStream);
					_responseContent = responseContentStream.str();

					if(response.getStatus() != 200)
					{
						std::ostringstream strout;
						strout << "EventConnection HTTP-Error: " << response.getStatus() << " - " << response.getReason();
						_errorMessage = strout.str();
						_error = true;
					}

					while(session.connected())
					{
						session.abort();
					}

					_finished = true;
				}
                catch (std::exception e)
				{
					std::ostringstream strout;
					strout << "EventConnection Exception: " << e.what();
					_errorMessage = strout.str();
					_error = true;
					_finished = true;
				}


			}

		}
	}
}
