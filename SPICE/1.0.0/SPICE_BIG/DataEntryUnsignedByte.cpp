/**
	SPICE BIG
	DataEntryUnsignedByte.cpp
	Purpose: Class to provide a data entry for type unsigned char / unsigned byte

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryUnsignedByte.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryUnsignedByte::DataEntryUnsignedByte(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryUnsignedByte::DataEntryUnsignedByte(std::string name, unsigned char minValue, unsigned char maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryUnsignedByte::DataEntryUnsignedByte(std::string name, unsigned char value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryUnsignedByte::~DataEntryUnsignedByte()
			{
			}

			bool DataEntryUnsignedByte::setValue(unsigned char value)
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
			unsigned char DataEntryUnsignedByte::getValue()
			{
				return (unsigned char)std::stoul(_value);
			}
			DataEntry::Types DataEntryUnsignedByte::getDataEntryType()
			{
				return DataEntry::Types::unsignedByte;
			}
			std::string DataEntryUnsignedByte::getDataEntryTypeAsString()
			{
				return "unsignedByte";
			}
			std::string DataEntryUnsignedByte::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryUnsignedByte::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryUnsignedByte::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					unsigned long tempValueULong = std::stoul(valueString);
					unsigned char tempValue = 0;
					if(tempValueULong > 255)
					{
						returnValue = false;
					}
					else
					{
						tempValue = (unsigned char)tempValueULong;
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