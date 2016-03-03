/**
	SPICE Core
	SPICECore.cpp
	Purpose: Front-Class to implement the core in a specific device

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "SPICECore.h"
#include "Internal/InternalCore.h"

namespace SPICE
{
	namespace Core
	{
		SPICECore::SPICECore(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider)
		{
			_internalCore = new Internal::InternalCore(resourceProvider);
		}
		SPICECore::~SPICECore()
		{
			if(_internalCore != nullptr)
			{
				delete _internalCore;
				_internalCore = nullptr;
			}
		}

		void SPICECore::registerAtEthernetServer(std::shared_ptr<SPICE::BIG::IEthernetServer> ethernetServer)
		{
			_internalCore->registerAtEthernetServer(ethernetServer);
		}
		void SPICECore::unregisterFromEthernetServer(std::shared_ptr<SPICE::BIG::IEthernetServer> ethernetServer)
		{
			_internalCore->unregisterFromEthernetServer(ethernetServer);
		}
	}
}