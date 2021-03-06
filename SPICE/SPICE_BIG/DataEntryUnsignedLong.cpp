/**
	SPICE BIG
	DataEntryUnsignedLong.cpp
	Purpose: Class to provide a data entry for type unsigned long integer

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
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
			DataEntryUnsignedLong::DataEntryUnsignedLong(std::string name, uint64_t minValue, uint64_t maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryUnsignedLong::DataEntryUnsignedLong(std::string name, uint64_t value) : DataEntry(name),
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

			bool DataEntryUnsignedLong::setValue(uint64_t value)
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
			uint64_t DataEntryUnsignedLong::getValue()
			{
				return std::stoull(_value);
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
					uint64_t tempValue = std::stoull(valueString);
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