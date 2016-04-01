/**
	SPICE BIG
	DataEntryByte.cpp
	Purpose: Class to provide a data entry for type char / byte

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
*/

#include "DataEntryByte.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryByte::DataEntryByte(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryByte::DataEntryByte(std::string name, int8_t minValue, int8_t maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryByte::DataEntryByte(std::string name, int8_t value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryByte::~DataEntryByte()
			{
			}

			bool DataEntryByte::setValue(int8_t value)
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
			int8_t DataEntryByte::getValue()
			{
				return (int8_t)std::stoi(_value);
			}
			DataEntry::Types DataEntryByte::getDataEntryType()
			{
				return DataEntry::Types::Byte;
			}
			std::string DataEntryByte::getDataEntryTypeAsString()
			{
				return "Byte";
			}
			std::string DataEntryByte::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryByte::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryByte::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					int tempValueInt = std::stoi(valueString);
					int8_t tempValue = 0;
					if(tempValueInt < -128 || tempValueInt > 127)
					{
						returnValue = false;
					}
					else
					{
						tempValue = (int8_t)tempValueInt;
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