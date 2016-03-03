/**
	SPICE BIG
	DataEntryString.cpp
	Purpose: Class to provide a data entry for type string

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryString.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryString::DataEntryString(std::string name, bool emptyStringAllowed) : DataEntry(name)
			{
				_emptyStringAllowed = emptyStringAllowed;
				_value = "";
			}
			DataEntryString::DataEntryString(std::string name, bool emptyStringAllowed, std::string value) : DataEntry(name)
			{
				_emptyStringAllowed = emptyStringAllowed;
				_value = value;
				if(value != "" || _emptyStringAllowed)
				{
					_valueIsSet = true;
				}
			}

			DataEntryString::~DataEntryString()
			{
			}

			DataEntry::Types DataEntryString::getDataEntryType()
			{
				return DataEntry::Types::String;
			}
			std::string DataEntryString::getDataEntryTypeAsString()
			{
				return "String";
			}
			std::string DataEntryString::getMinValue()
			{
				return "";
			}
			std::string DataEntryString::getMaxValue()
			{
				return "";
			}
			bool DataEntryString::checkStringIsValidValue(std::string& valueString)
			{
				return true;
			}
		}
	}
}