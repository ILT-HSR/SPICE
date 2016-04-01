/**
	SPICE BIG
	DataEntryUnsignedShort.h
	Purpose: Class to provide a data entry for type unsigned short

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
*/

#ifndef DATAENTRYUNSIGNEDSHORT_H
#define DATAENTRYUNSIGNEDSHORT_H

#include <stdint.h>

#include "DataEntry.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			class DataEntryUnsignedShort : public DataEntry
			{
				// Methoden
			public:
				/**
					DataEntry of type uint16_t

					@param name Name of the DataEntry
				*/
				DataEntryUnsignedShort(std::string name);
				/**
					DataEntry of type uint16_t

					@param name Name of the DataEntry
					@param minValue The minimum value for the type
					@param maxValue The maximum value for the type
				*/
				DataEntryUnsignedShort(std::string name, uint16_t minValue, uint16_t maxValue);
				/**
					DataEntry of type uint16_t

					@param name Name of the DataEntry
					@param value Sets the given value. Flag "IsSet" will be set to TRUE.
				*/
				DataEntryUnsignedShort(std::string name, uint16_t value);
				virtual ~DataEntryUnsignedShort();

				/**
					Sets the value. If setted the min and max limits will be checked too.
					If ok, the value will be set.

					@param value The new value
					@return True if the value could be set. (limits ok)
				*/
				bool setValue(uint16_t value);
				/**
					Gets the value.

					@return Current value. If value is not set the inital value will be returned.
				*/
				uint16_t getValue();

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

				uint16_t _minValue;
				uint16_t _maxValue;
				bool _minMaxSetted;
			};
		}
	}
}
#endif // DATAENTRYUNSIGNEDSHORT_H
