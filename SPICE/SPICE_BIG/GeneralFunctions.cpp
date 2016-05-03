/**
	SPICE BIG
	GeneralFunctions.h
	Purpose: Provides general functionalities. Specially needed to ensure plattform independency. 

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "GeneralFunctions.h"

#include <sstream>

#ifdef __linux__
#include <stdlib.h>
#endif

namespace SPICE
{
	namespace BIG
	{
		std::mutex GeneralFunctions::_timeConverterMutex;

		void GeneralFunctions::getLocaltimeInfo(const time_t& time, struct tm& timeinfo)
		{
			std::lock_guard<std::mutex> lockGuard(_timeConverterMutex);
#ifdef __linux__
			struct tm* localTime = localtime(&time); // TODO: check if time is correct
			timeinfo = *localTime;
	//		delete localTime;
#else	// Windows
			localtime_s(&timeinfo, &time);
#endif // LINUX
		}
		void GeneralFunctions::getUTCtimeInfo(const time_t& time, struct tm& timeinfo)
		{
			std::lock_guard<std::mutex> lockGuard(_timeConverterMutex);
#ifdef __linux__
			struct tm* utcTime = gmtime(&time); // TODO: check if time is correct
			timeinfo = *utcTime;
	//		delete utcTime;
#else	// Windows
			gmtime_s(&timeinfo, &time);
#endif // LINUX
		}

		std::string GeneralFunctions::convertTimeToUTCString(const time_t& time)
		{
			struct tm timeinfo;
			getUTCtimeInfo(time, timeinfo);
			char buffer[25];
			strftime(buffer, 25, "%Y-%m-%dT%H:%M:%SZ", &timeinfo);

			return std::ostringstream(buffer).str();
		}

		void GeneralFunctions::wroteFileToDisk()
		{
#ifdef __linux__
			std::string command = "sync";
			std::system(&command[0]);
#endif // LINUX
		}


	}
}
