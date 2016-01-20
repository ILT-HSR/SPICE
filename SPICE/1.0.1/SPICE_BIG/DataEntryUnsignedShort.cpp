/**
	SPICE BIG
	DataEntryUnsignedShort.cpp
	Purpose: Class to provide a data entry for type unsigned short

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryUnsignedShort.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryUnsignedShort::DataEntryUnsignedShort(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryUnsignedShort::DataEntryUnsignedShort(std::string name, unsigned short minValue, unsigned short maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryUnsignedShort::DataEntryUnsignedShort(std::string name, unsigned short value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryUnsignedShort::~DataEntryUnsignedShort()
			{
			}

			bool DataEntryUnsignedShort::setValue(unsigned short value)
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
			unsigned short DataEntryUnsignedShort::getValue()
			{
				return (unsigned short)std::stoul(_value);
			}
			DataEntry::Types DataEntryUnsignedShort::getDataEntryType()
			{
				return DataEntry::Types::unsignedShort;
			}
			std::string DataEntryUnsignedShort::getDataEntryTypeAsString()
			{
				return "unsignedShort";
			}
			std::string DataEntryUnsignedShort::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryUnsignedShort::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryUnsignedShort::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					unsigned long tempValueULong = std::stoul(valueString);
					unsigned short tempValue = 0;
					if(tempValueULong > 65535)
					{
						returnValue = false;
					}
					else
					{
						tempValue = (unsigned short)tempValueULong;
					}
					if(_minMaxSetted && returnValue)
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