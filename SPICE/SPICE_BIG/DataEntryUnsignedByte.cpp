/**
	SPICE BIG
	DataEntryUnsignedByte.cpp
	Purpose: Class to provide a data entry for type unsigned char / unsigned byte

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
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
			DataEntryUnsignedByte::DataEntryUnsignedByte(std::string name, uint8_t minValue, uint8_t maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryUnsignedByte::DataEntryUnsignedByte(std::string name, uint8_t value) : DataEntry(name),
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

			bool DataEntryUnsignedByte::setValue(uint8_t value)
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
			uint8_t DataEntryUnsignedByte::getValue()
			{
				return (uint8_t)std::stoul(_value);
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
					uint8_t tempValue = 0;
					if(tempValueULong > 255)
					{
						returnValue = false;
					}
					else
					{
						tempValue = (uint8_t)tempValueULong;
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