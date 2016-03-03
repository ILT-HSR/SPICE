/**
	SPICE Core
	CoreData.cpp
	Purpose: Class containing the diffrent data container.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "CoreData.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			CoreData::CoreData() :
				_sessionData(std::shared_ptr<SessionData>(new SessionData)),
				_resourceProvider(nullptr),
				_serialNumber(0)
			{
			}
			CoreData::~CoreData()
			{
			}

			std::shared_ptr<SessionData> CoreData::getSessionData()
			{
				return _sessionData;
			}
			std::shared_ptr<SPICE::BIG::IResourceProvider> CoreData::getResourceProvider()
			{
				return _resourceProvider;
			}
			void CoreData::setResourceProvider(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider)
			{
				_resourceProvider = resourceProvider;
			}

			unsigned long CoreData::getSerialNumber()
			{
				return _serialNumber;
			}

			void CoreData::setSerialNumber(unsigned long serialNumber)
			{
				if(_serialNumber == 0)
				{
					_serialNumber = serialNumber;
				}
			}
		}
	}
}