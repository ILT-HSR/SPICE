/**
	SPICE BIG
	DataEntryBool.cpp
	Purpose: Class to provide a data entry for type bool

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryBool.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryBool::DataEntryBool(std::string name) : DataEntry(name)
			{
				_value = "false";
			}
			DataEntryBool::DataEntryBool(std::string name, bool value) : DataEntry(name)
			{
				if(value)
				{
					_value = "true";
				}
				else
				{
					_value = "false";
				}
				_valueIsSet = true;
			}

			DataEntryBool::~DataEntryBool()
			{
			}

			bool DataEntryBool::setValue(bool value)
			{
				if(value)
				{
					_value = "true";
				}
				else
				{
					_value = "false";
				}
				_valueIsSet = true;
				return true;
			}
			bool DataEntryBool::getValue()
			{
				return _value == "true";
			}
			DataEntry::Types DataEntryBool::getDataEntryType()
			{
				return DataEntry::Types::Boolean;
			}
			std::string DataEntryBool::getDataEntryTypeAsString()
			{
				return "Boolean";
			}
			std::string DataEntryBool::getMinValue()
			{
				return "";
			}
			std::string DataEntryBool::getMaxValue()
			{
				return "";
			}
			bool DataEntryBool::checkStringIsValidValue(std::string& valueString)
			{
				if(valueString == "1" || valueString == "true" || valueString == "True" || valueString == "TRUE")
				{
					valueString = "true";
					return true;
				}
				if(valueString == "0" || valueString == "false" || valueString == "False" || valueString == "FALSE")
				{
					valueString = "false";
					return true;
				}
				return false;
			}
		}
	}
}