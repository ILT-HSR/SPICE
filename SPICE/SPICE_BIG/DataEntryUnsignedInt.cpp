/**
	SPICE BIG
	DataEntryUnsignedInt.cpp
	Purpose: Class to provide a data entry for type unsigned integer

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
*/

#include "DataEntryUnsignedInt.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryUnsignedInt::DataEntryUnsignedInt(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryUnsignedInt::DataEntryUnsignedInt(std::string name, uint32_t minValue, uint32_t maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryUnsignedInt::DataEntryUnsignedInt(std::string name, uint32_t value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryUnsignedInt::~DataEntryUnsignedInt()
			{
			}

			bool DataEntryUnsignedInt::setValue(uint32_t value)
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
			uint32_t DataEntryUnsignedInt::getValue()
			{
				return (uint32_t)std::stoul(_value);
			}
			DataEntry::Types DataEntryUnsignedInt::getDataEntryType()
			{
				return DataEntry::Types::unsignedInt;
			}
			std::string DataEntryUnsignedInt::getDataEntryTypeAsString()
			{
				return "unsignedInt";
			}
			std::string DataEntryUnsignedInt::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryUnsignedInt::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryUnsignedInt::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					unsigned long tempValueULong = std::stoul(valueString);
					uint32_t tempValue = 0;
					if(tempValueULong > 4294967295U)
					{
						returnValue = false;
					}
					else
					{
						tempValue = (uint32_t)tempValueULong;
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