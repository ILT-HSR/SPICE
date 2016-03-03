/**
	SPICE Core
	CommunicationInformations.cpp
	Purpose: Class containing informations about the communication for successful EventCommunications

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "CommunicationInformations.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			CommunicationInformations::CommunicationInformations(std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> eventConnectionFactory, std::string silaNamespace) :
				_eventConnectionFactory(eventConnectionFactory),
				_silaNamespace(silaNamespace)
			{
			}
			CommunicationInformations::~CommunicationInformations()
			{
			}

			std::shared_ptr<SPICE::BIG::IEthernetEventConnectionFactory> CommunicationInformations::getEventConnectionFactory()
			{
				return _eventConnectionFactory;
			}
			std::string CommunicationInformations::getSiLANamespace()
			{
				return _silaNamespace;
			}
		}
	}
}