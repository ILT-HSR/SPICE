/**
	SPICE ES POCO
	EthernetEventConnectionFactory.h
	Purpose: Class to provide a factory for the poco specific outgoing connections (EventConnection)

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef ETHERNETEVENTCONNECTIONFACTORY_H
#define ETHERNETEVENTCONNECTIONFACTORY_H

#include "IEthernetEventConnectionFactory.h"

#include "EventConnection.h"

namespace SPICE
{
	namespace ES
	{
		namespace POCO
		{
			class EthernetEventConnectionFactory : public SPICE::BIG::IEthernetEventConnectionFactory
			{
				public:
					/**
						Class to provide a factory for the poco specific outgoing connections (EventConnection)
					*/
					EthernetEventConnectionFactory();
					virtual ~EthernetEventConnectionFactory();

			// ---------- IEthernetEventConnectionFactory -------------------
					/**
						Creates a new instance for of an IEthernetEventConnection

						@return New created IEthernetEventConnection
					*/
					virtual std::shared_ptr<SPICE::BIG::IEthernetEventConnection> getNewEthernetEventConnection();

				protected:
				private:

			};
		}
	}
}
#endif // ETHERNETEVENTCONNECTIONFACTORY_H
