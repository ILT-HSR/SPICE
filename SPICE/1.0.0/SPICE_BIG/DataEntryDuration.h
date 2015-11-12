/**
	SPICE BIG
	DataEntryDuration.h
	Purpose: Class to provide a data entry for type Duration

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef DATAENTRYDURATION_H
#define DATAENTRYDURATION_H

#include "DataEntry.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			class DataEntryDuration : public DataEntry
			{
				// Methoden
			public:
				/**
					DataEntry of type Duration

					@param name Name of the DataEntry
				*/
				DataEntryDuration(std::string name);
				/**
					DataEntry of type Duration

					@param name Name of the DataEntry
					@param minValue The minimum value for the type (seconds)
					@param maxValue The maximum value for the type (seconds)
				*/
				DataEntryDuration(std::string name, double minValue, double maxValue);
				/**
					DataEntry of type Duration

					@param name Name of the DataEntry
					@param value Sets the given value as seconds. Flag "IsSet" will be set to TRUE.
				*/
				DataEntryDuration(std::string name, double value);
				virtual ~DataEntryDuration();

				/**
					Sets the value. If setted the min and max limits will be checked too.
					If ok, the value will be set.

					@param value The new value as seconds
					@return True if the value could be set. (limits ok)
				*/
				bool setValue(double value);
				/**
					Gets the value as seconds.

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
#endif // DATAENTRYDURATION_H
