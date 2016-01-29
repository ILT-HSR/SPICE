/**
	SPICE BIG
	GeneralFunctions.h
	Purpose: Provides general functionalities. Specially needed to ensure plattform independency. 

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef GENERALFUNCTIONS_H
#define GENERALFUNCTIONS_H

#include <time.h>
#include <mutex>
#include <string>

namespace SPICE
{
	namespace BIG
	{
		class GeneralFunctions
		{
		public:
			/**
				Gets the time information depending on the configured location.

				@param time Given time for which the timeinfo should be generated.
				@param timeinfo	Generated timeinfo
			*/
			static void getLocaltimeInfo(const time_t& time, struct tm& timeinfo);
			/**
				Gets the time information depending on the utc time.

				@param time Given time for which the timeinfo should be generated.
				@param timeinfo Generated timeinfo
			*/
			static void getUTCtimeInfo(const time_t& time, struct tm& timeinfo);

			/**
				Returns a time information as string depending on the utc time.

				@param time Given time for which the string should be generated.
				@return Generated time information as string
			*/
			static std::string convertTimeToUTCString(const time_t& time);

			/**
				Function should be called after writing to the filesystem. Needed for LINUX to sync filesystem. Without, data could be lost (depending on the system).
			*/
			static void wroteFileToDisk();
		private:
			static std::mutex _timeConverterMutex;
		};
	}
}

#endif // GENERALFUNCTIONS_H
