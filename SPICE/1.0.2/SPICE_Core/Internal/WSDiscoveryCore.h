/**
	SPICE Core
	WSDiscoveryCore.h
	Purpose: Class to provide core specific discovery functionality. Instance created per registred EthernetServer. Currently instances are not deleted until program ends.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef WSDISCOVERYCORE_H
#define WSDISCOVERYCORE_H

#include <random>

#include "IWSDiscoveryCallback.h"
#include "CoreData.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class WSDiscoveryCore : public SPICE::BIG::IWSDiscoveryCallback
			{
				// Methoden
			public:
				/**
					 Class to provide core specific discovery functionality. Instance created per registred EthernetServer. Currently instances are not deleted until program ends.
				*/
				WSDiscoveryCore(std::shared_ptr<CoreData> coreData);
				virtual ~WSDiscoveryCore();


		// -------------------------- Implementation IWDiscoveryCallback ----------------------------------
				/**
					Is called by the wsdiscovery-server in case an ethernet-interface or ip has changed and a new hello message is required

					@param helloMessage Used to give back the created ws-discovery-hello-message
				*/
				virtual void createHelloMessage(std::string& helloMessage);
				/**
					Is called by the wsdiscovery-server in case that the server is stopping and a bye message is required

					@param byeMessage Used to give back the created ws-discovery-bye-message
				*/
				virtual void createByeMessage(std::string& byeMessage);
				/**
					Is called by the wsdiscovery-server in case that a multicast-message is received. The ws-discovery of the core can decide if a unicast answer should be sended.

					@param incomingMessage The received multicast-message which has to be handled
					@param sendUnicastAnswer Used to give back uf a unicast-answer should be send. 
					@param answerMessage Used to give back an answer. Is sended if "sendUnicastAnswer" is true
				*/
				virtual void handleIncomingMulticastMessage(std::string incomingMessage, bool& sendUnicastAnswer, std::string& answerMessage);
				/**
					Gets the string which can be searched within a given message an be replaced by the correct ip-address, port and URI

					@return Template string which can be searched and replaced
				*/
				virtual std::string getAddressTemplate();

				/**
					Generates a valid random UUID with the seeded generator. If generator is not seeded you should seed it.

					@return The generated UUID
				*/
				static std::string generateRandomUUID();
				/**
					Sets a UUID for the device. Is statical and used for every specific implementation within the same application.

					@param uuid The UUID of the device
				*/
				static void setDeviceUUID(std::string uuid);
				/**
					Seeds the generator to be able to generate better random numbers.

					@param seed Value to seed the generator
				*/
				static unsigned long seedGenerator(unsigned long seed);

			protected:
			private:

				/**
					Creates a probe match message to react on incoming messages

					@param probeMatchMessage A given empty string which is changed (probe match message written in)
					@param relatingUUID The message UUID on which the probe match message is relating
					@param replyTo The replyTo address information
				*/
				void createProbeMatchMessage(std::string& probeMatchMessage, std::string relatingUUID, std::string replyTo);
				/**
					Creates a resolve match message to react on incoming messages

					@param resolveMatchMessage A given empty string which is changed (resolve match message written in)
					@param relatingUUID The message UUID on which the resolve match message is relating
					@param replyTo The replyTo address information
				*/
				void createResolveMatchMessage(std::string& resolveMatchMessage, std::string relatingUUID, std::string replyTo);
				/**
					Updates the message id memory. Delete old message ids after 5 minuts. Checks the given message id. If a new ID, true is returned.

					@param messageID Message ID to check if not answered yet
					@return True: Not answered yet, False: already answered, existing in the message id memory
				*/
				bool updateAndCheckMessageIDMemory(std::string messageID);

				static std::default_random_engine _generator;
				static std::uniform_int_distribution<int> _distribution;
				static std::string _deviceUUID;

				std::shared_ptr<CoreData> _coreData;

				int _instanceVersion;
				int _messageNumber;
				int _metadataVersion;

				std::vector<std::string> _messageIDMemory[5];
				std::atomic<int> _currentActiveMessageIDMemory;
				time_t _lastChangedTime;
			};
		}
	}
}
#endif // WSDISCOVERYCORE_H
