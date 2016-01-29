/**
	SPICE Core
	CoreData.h
	Purpose: Class containing the diffrent data container.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef COREDATA_H
#define COREDATA_H

#include <memory>

#include "SessionData.h"
#include "IResourceProvider.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class CoreData
			{
				public:
					/**
						Class containing the diffrent data container.
					*/
					CoreData();
					virtual ~CoreData();

					/**
						Gets the sessionData containing data according to the current session

						@return SessionData
					*/
					std::shared_ptr<SessionData> getSessionData();

					/**
						Gets the registred resource provider

						@return Registred resource provider
					*/
					std::shared_ptr<SPICE::BIG::IResourceProvider> getResourceProvider();
					/**
						Sets / registers the resource provider

						@param resourceProvider The new resource provider
					*/
					void setResourceProvider(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider);
					
					/**
						Gets the serial number of the core

						@return serial number of the core
					*/
					unsigned long getSerialNumber();

					/**
						Sets the serial number of the core

						@param serialNumber New serial number
					*/
					void setSerialNumber(unsigned long serialNumber);

				protected:
				private:

					std::shared_ptr<SessionData> _sessionData;
					std::shared_ptr<SPICE::BIG::IResourceProvider> _resourceProvider;
					unsigned long _serialNumber;
			};
		}
	}
}
#endif // COREDATA_H
