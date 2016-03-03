/**
	SPICE BIG
	CommunicationInformations.h
	Purpose: Class containing informations about the communication for successful EventCommunications

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef COMMUNICATIONINFORMATIONS_H
#define COMMUNICATIONINFORMATIONS_H

#include <string>

#include "IEthernetEventConnectionFactory.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class CommunicationInformations
			{
				public:
					/**
						Class containing informations about the communication for successful EventCommunications

						@param eventConnectionFactory Factory to generate event connections
						@param silaNamespace Used namespace for incoming connection
					*/
					CommunicationInformations(std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> eventConnectionFactory, std::string silaNamespace);
					virtual ~CommunicationInformations();

					/**
						Gets the event connection factory

						@return EventConnectionFactory to generate event connections
					*/
					std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> getEventConnectionFactory();
					/**
						Gets the used silaNamespace

						@return SiLANamespace to use for events
					*/
					std::string getSiLANamespace();

				protected:
				private:
					std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> _eventConnectionFactory;
					std::string _silaNamespace;

			};
		}
	}
}
#endif // COMMUNICATIONINFORMATIONS_H
