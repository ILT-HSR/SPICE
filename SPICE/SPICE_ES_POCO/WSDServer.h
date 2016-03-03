/**
	SPICE ES POCO
	WSDServer.h
	Purpose: ServerClass for WebService-Discovery using POCO. Listening for Multicast on Port 3702, checking for ethernet interface changes and trigger corresponding messages.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef WSDSERVER_H
#define WSDSERVER_H

#include <memory>
#include <thread>
#include <atomic>
#include <map>
#include <list>

#include "IWSDiscoveryCallback.h"

#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/MulticastSocket.h"
#include "Poco/Net/IPAddress.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			class WSDServer
			{
				public:
					/**
						Central class of the POCO web-service-discovery implementation for SPICE

						@param ethernetServerPort Port on which the current EthernetServer instance is listening on
						@param uriPath URI-Path of the current core-instance for which this discovery-instance is created
						@param wsDiscoveryCallback Callback to the core to handle and create discovery-messages
					*/
					WSDServer(unsigned int ethernetServerPort, std::string uriPath, std::shared_ptr<SPICE::BIG::IWSDiscoveryCallback> wsDiscoveryCallback);
					virtual ~WSDServer();

					/**
						Starts the listening socket and the thread for checking interface changes.
					*/
					void startServer();

					/**
						Stops the internal cycle which is checking for new interfaces. Sends the bye-message to the present interfaces. Closes the listening socket.
					*/
					void stopServer();

				protected:
				private:

					/**
						Internal cycle to read incomming messages and call the handler
					*/
					void listeningMulticastCycle();
					/**
						Internal cycle to check for changes at the ethernet interfaces and to be able to react on that
					*/
					void checkForEthernetChangesCycle();
					/**
						Sends the multicast message on the given interface

						@param networkInterface Interface to send the multicast message on
						@param message Message to send
					*/
					void sendMulticastMessage(Poco::Net::NetworkInterface& networkInterface, std::string message);
					/**
						Gets the address of the converter as replaceble string for the wsd-message based on the given ipAddress

						@param ipAddress Address on whicht the converter address is generated
						@return The generated replace address
					*/
					std::string getReplaceAddress(std::string ipAddress);


					std::shared_ptr<SPICE::BIG::IWSDiscoveryCallback> _wsDiscoveryCallback;
					std::map<std::string, std::string> _detectedInterfacesAndIPs;
					std::list<std::string> _groupAddedInterfaces;

					unsigned int _ethernetServerPort;
					std::string _uriPath;

					std::thread* _listeningThread;
					std::thread* _internalCheckThread;

					std::atomic<bool> _stopping;

					Poco::Net::SocketAddress _multicastAddress;
					Poco::Net::MulticastSocket _listeningSocket;


			};
		}
	}
}
#endif // WSDSERVER_H
