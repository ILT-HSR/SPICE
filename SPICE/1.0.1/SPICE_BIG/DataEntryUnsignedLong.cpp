/**
	SPICE BIG
	DataEntryUnsignedLong.cpp
	Purpose: Class to provide a data entry for type unsigned long integer

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryUnsignedLong.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryUnsignedLong::DataEntryUnsignedLong(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryUnsignedLong::DataEntryUnsignedLong(std::string name, unsigned long minValue, unsigned long maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryUnsignedLong::DataEntryUnsignedLong(std::string name, unsigned long value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryUnsignedLong::~DataEntryUnsignedLong()
			{
			}

			bool DataEntryUnsignedLong::setValue(unsigned long value)
			{
				if(_minMaxSetted)
				{
					if(value < _minValue || value > _maxValue)
					{
						return false;
					}
				}
				_value = std::to_string(value);
				_valueIsSet = true;
				return true;
			}
			unsigned long DataEntryUnsignedLong::getValue()
			{
				return std::stoul(_value);
			}
			DataEntry::Types DataEntryUnsignedLong::getDataEntryType()
			{
				return DataEntry::Types::unsignedLong;
			}
			std::string DataEntryUnsignedLong::getDataEntryTypeAsString()
			{
				return "unsignedLong";
			}
			std::string DataEntryUnsignedLong::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryUnsignedLong::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryUnsignedLong::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					unsigned long tempValue = std::stoul(valueString);
					if(_minMaxSetted)
					{
						if(tempValue < _minValue || tempValue > _maxValue)
						{
							returnValue = false;
						}
					}
					if(returnValue)
					{
						valueString = std::to_string(tempValue);
					}
				}
				catch (std::exception e)
				{
					returnValue = false;
				}
				return returnValue;
			}
		}
	}
}