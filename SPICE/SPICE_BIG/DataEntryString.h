/**
	SPICE BIG
	DataEntryString.h
	Purpose: Class to provide a data entry for type string

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef DATAENTRYSTRING_H
#define DATAENTRYSTRING_H

#include "DataEntry.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			class DataEntryString : public DataEntry
			{
				// Methoden
			public:
				/**
					DataEntry of type string

					@param name Name of the DataEntry
					@param emptyStringAllowed Set if an empty string is allowed
				*/
				DataEntryString(std::string name, bool emptyStringAllowed);
				/**
					DataEntry of type string

					@param name Name of the DataEntry
					@param emptyStringAllowed Set if an empty string is allowed
					@param value Sets the given value. Flag "IsSet" will be set to TRUE.
				*/
				DataEntryString(std::string name, bool emptyStringAllowed, std::string value);
				virtual ~DataEntryString();

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
			};
		}
	}
}
#endif // DATAENTRYSTRING_H
