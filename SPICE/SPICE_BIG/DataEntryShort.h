/**
	SPICE BIG
	DataEntryShort.h
	Purpose: Class to provide a data entry for type short

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
*/

#ifndef DATAENTRYSHORT_H
#define DATAENTRYSHORT_H

#include <stdint.h>

#include "DataEntry.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			class DataEntryShort : public DataEntry
			{
				// Methoden
			public:
				/**
					DataEntry of type int16_t

					@param name Name of the DataEntry
				*/
				DataEntryShort(std::string name);
				/**
					DataEntry of type int16_t

					@param name Name of the DataEntry
					@param minValue The minimum value for the type
					@param maxValue The maximum value for the type
				*/
				DataEntryShort(std::string name, int16_t minValue, int16_t maxValue);
				/**
					DataEntry of type int16_t

					@param name Name of the DataEntry
					@param value Sets the given value. Flag "IsSet" will be set to TRUE.
				*/
				DataEntryShort(std::string name, int16_t value);
				virtual ~DataEntryShort();

				/**
					Sets the value. If setted the min and max limits will be checked too.
					If ok, the value will be set.

					@param value The new value
					@return True if the value could be set. (limits ok)
				*/
				bool setValue(int16_t value);
				/**
					Gets the value.

					@return Current value. If value is not set the inital value will be returned.
				*/
				int16_t getValue();

				/**
					Gets the type of the DataEntry.

					@return type of the DataEntry
				*/
				virtual Types getDataEntryType();
				/**
					Gets the type of the DataEntry as string.

					@return type of the DataEntry as string
				*/
				virtual std::string getDataEntryTypeAsString();
				/**
					Gets the minimum value of the DataEntry. Returns an empty string if no minValue is set.

					@return minValue
				*/
				virtual std::string getMinValue();
				/**
					Gets the maximum value of the DataEntry. Returns an empty string if no maxValue is set.

					@return maxValue
				*/
				virtual std::string getMaxValue();

			private:
				/**
					Checks the given string if it is suitable for the type. If set the limits are also checked.

					@param valueString String with value which should be checked. Can be changed from the function for cosmetical reasons (example remove a space)
					@return True if valueString is ok and can be set.
				*/
				virtual bool checkStringIsValidValue(std::string& valueString);

				int16_t _minValue;
				int16_t _maxValue;
				bool _minMaxSetted;
			};
		}
	}
}
#endif // DATAENTRYSHORT_H
