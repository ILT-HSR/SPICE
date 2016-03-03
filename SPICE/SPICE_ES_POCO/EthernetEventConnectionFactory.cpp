/**
	SPICE ES POCO
	EthernetEventConnectionFactory.cpp
	Purpose: Class to provide a factory for the poco specific outgoing connections (EventConnection)

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "EthernetEventConnectionFactory.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			EthernetEventConnectionFactory::EthernetEventConnectionFactory()
			{
				//ctor
			}
			EthernetEventConnectionFactory::~EthernetEventConnectionFactory()
			{
				//dtor
			}

			std::shared_ptr<SPICE::BIG::IEthernetEventConnection> EthernetEventConnectionFactory::getNewEthernetEventConnection()
			{
				std::shared_ptr<EventConnection> eventConnection(new EventConnection());
				return eventConnection;
			}

		}
	}
}