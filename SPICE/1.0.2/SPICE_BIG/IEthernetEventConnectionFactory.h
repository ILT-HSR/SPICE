/**
	SPICE BIG
	IEthernetEventConnectionFactory.h
	Purpose: Interface which provides the core a factory to create new instances of an IEthernetEventConnection to send events.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IETHERNETEVENTCONNECTIONFACTORY_H
#define IETHERNETEVENTCONNECTIONFACTORY_H

#include <memory>

#include "IEthernetEventConnection.h"

namespace SPICE
{
	namespace BIG
	{
		class IEthernetEventConnectionFactory
		{
			public:
				/**
					Creates a new instance for of an IEthernetEventConnection

                    @return New created IEthernetEventConnection
				*/
				virtual std::shared_ptr<IEthernetEventConnection> getNewEthernetEventConnection() = 0;
		};
	}
}
#endif // IETHERNETEVENTCONNECTIONFACTORY_H
