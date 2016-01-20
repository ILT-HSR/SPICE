/**
	SPICE BIG
	IWSDiscoveryCallback.h
	Purpose: Interface which is implemented by the core to be able to register WS-Discovery-Service at an IEthernetServer. Has to handle the callbacks of the WSDiscovery.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IWSDISCOVERYCALLBACK_H
#define IWSDISCOVERYCALLBACK_H

#include <string>

namespace SPICE
{
	namespace BIG
	{
		class IWSDiscoveryCallback
		{
		public:

			/**
				Is called by the wsdiscovery-server in case an ethernet-interface or ip has changed and a new hello message is required

				@param helloMessage Used to give back the created ws-discovery-hello-message
			*/
			virtual void createHelloMessage(std::string& helloMessage) = 0;

			/**
				Is called by the wsdiscovery-server in case that the server is stopping and a bye message is required

				@param byeMessage Used to give back the created ws-discovery-bye-message
			*/
			virtual void createByeMessage(std::string& byeMessage) = 0;

			/**
				Is called by the wsdiscovery-server in case that a multicast-message is received. The ws-discovery of the core can decide if a unicast answer should be sended.

				@param incomingMessage The received multicast-message which has to be handled
				@param sendUnicastAnswer Used to give back uf a unicast-answer should be send. 
				@param answerMessage Used to give back an answer. Is sended if "sendUnicastAnswer" is true
			*/
			virtual void handleIncomingMulticastMessage(std::string incomingMessage, bool& sendUnicastAnswer, std::string& answerMessage) = 0;

			/**
				Gets the string which can be searched within a given message an be replaced by the correct ip-address, port and URI

				@return Template string which can be searched and replaced
			*/
			virtual std::string getAddressTemplate() = 0;


		};

	}
}
#endif // IWSDISCOVERYCALLBACK_H
