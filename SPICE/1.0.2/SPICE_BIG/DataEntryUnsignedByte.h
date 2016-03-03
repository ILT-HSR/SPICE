/**
	SPICE BIG
	DataEntryUnsignedByte.h
	Purpose: Class to provide a data entry for type unsigned char / unsigned byte

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef DATAENTRYUNSIGNEDBYTE_H
#define DATAENTRYUNSIGNEDBYTE_H

#include "DataEntry.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			class DataEntryUnsignedByte : public DataEntry
			{
				// Methoden
			public:
				/**
					DataEntry of type unsigned char / unsigned byte

					@param name Name of the DataEntry
				*/
				DataEntryUnsignedByte(std::string name);
				/**
					DataEntry of type unsigned char / unsigned byte

					@param name Name of the DataEntry
					@param minValue The minimum value for the type
					@param maxValue The maximum value for the type
				*/
				DataEntryUnsignedByte(std::string name, unsigned char minValue, unsigned char maxValue);
				/**
					DataEntry of type unsigned char / unsigned byte

					@param name Name of the DataEntry
					@param value Sets the given value. Flag "IsSet" will be set to TRUE.
				*/
				DataEntryUnsignedByte(std::string name, unsigned char value);
				virtual ~DataEntryUnsignedByte();

				/**
					Sets the value. If setted the min and max limits will be checked too.
					If ok, the value will be set.

					@param value The new value
					@return True if the value could be set. (limits ok)
				*/
				bool setValue(unsigned char value);
				/**
					Gets the value.

					@return Current value. If value is not set the inital value will be returned.
				*/
				unsigned char getValue();

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

				unsigned char _minValue;
				unsigned char _maxValue;
				bool _minMaxSetted;
			};
		}
	}
}
#endif // DATAENTRYUNSIGNEDBYTE_H
