/**
	SPICE BIG
	DataEntryUnsignedInt.cpp
	Purpose: Class to provide a data entry for type unsigned integer

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
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
			DataEntryUnsignedInt::DataEntryUnsignedInt(std::string name, unsigned int minValue, unsigned int maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryUnsignedInt::DataEntryUnsignedInt(std::string name, unsigned int value) : DataEntry(name),
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

			bool DataEntryUnsignedInt::setValue(unsigned int value)
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
			unsigned int DataEntryUnsignedInt::getValue()
			{
				return (unsigned int)std::stoul(_value);
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
					unsigned int tempValue = 0;
					if(tempValueULong > 4294967295U)
					{
						returnValue = false;
					}
					else
					{
						tempValue = (unsigned int)tempValueULong;
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