/**
	SPICE Core
	SPICECore.h
	Purpose: Front-Class to implement the core in a specific device

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef SPICECORE_H
#define SPICECORE_H

#include "IResourceProvider.h"
#include "IEthernetServer.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class InternalCore;
		}
		class SPICECore
		{
			// Methoden
		public:
			/**
				Front-Class to implement the core in a specific device

				@param resourceProvider ResourceProvider to provide specific implementations to the core.
			*/
			SPICECore(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider);
			virtual ~SPICECore();

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

		private:
			Internal::InternalCore* _internalCore;
		};
	}
}
#endif // SPICECORE_H
