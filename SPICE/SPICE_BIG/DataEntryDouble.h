/**
	SPICE BIG
	DataEntryDouble.h
	Purpose: Class to provide a data entry for type double

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef DATAENTRYDOUBLE_H
#define DATAENTRYDOUBLE_H

#include "DataEntry.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			class DataEntryDouble : public DataEntry
			{
				// Methoden
			public:
				/**
					DataEntry of type double

					@param name Name of the DataEntry
				*/
				DataEntryDouble(std::string name);
				/**
					DataEntry of type double

					@param name Name of the DataEntry
					@param minValue The minimum value for the type
					@param maxValue The maximum value for the type
				*/
				DataEntryDouble(std::string name, double minValue, double maxValue);
				/**
					DataEntry of type double

					@param name Name of the DataEntry
					@param value Sets the given value. Flag "IsSet" will be set to TRUE.
				*/
				DataEntryDouble(std::string name, double value);
				virtual ~DataEntryDouble();

				/**
					Sets the value. If setted the min and max limits will be checked too.
					If ok, the value will be set.

					@param value The new value
					@return True if the value could be set. (limits ok)
				*/
				bool setValue(double value);
				/**
					Gets the value.

					@return Current value. If value is not set the inital value will be returned.
				*/
				double getValue();

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

				double _minValue;
				double _maxValue;
				bool _minMaxSetted;
			};
		}
	}
}
#endif // DATAENTRYDOUBLE_H
