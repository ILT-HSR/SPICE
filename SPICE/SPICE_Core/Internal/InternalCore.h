/**
	SPICE Core
	InternalCore.h
	Purpose: Class internal as container for the most important instances of the core

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef INTERNALCORE_H
#define INTERNALCORE_H

#include <memory>

#include "IResourceProvider.h"
#include "IEthernetServer.h"
#include "CommandHandler.h"
#include "ConnectionHandler.h"
#include "WSDiscoveryCore.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class InternalCore
			{
				// Methoden
			public:
				/**
					Class internal as container for the most important instances of the core

					@param resourceProvider ResourceProvider to provide specific implementations to the core.
				*/
				InternalCore(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider);
				virtual ~InternalCore();

				/**
					Register the core at the given ethernet server. It is allowed to register the same core to
					more then one ethernet server. (Usefull as example to provide as http & https service)

					@param ethernetServer Server where the core should register
				*/
				void registerAtEthernetServer(std::shared_ptr<SPICE::BIG::IEthernetServer> ethernetServer);
				/**
					Unregister the core from the given ethernet server.

					@param ethernetServer Server where the core should unregister
				*/
				void unregisterFromEthernetServer(std::shared_ptr<SPICE::BIG::IEthernetServer> ethernetServer);

			protected:
			private:
				std::shared_ptr<CommandHandler> _commandHandler;
				std::shared_ptr<ConnectionHandler> _connectionHandler;
				std::vector<std::shared_ptr<WSDiscoveryCore>> _wsDiscoveryCoreList;
			};
		}
	}
}
#endif // INTERNALCORE_H
