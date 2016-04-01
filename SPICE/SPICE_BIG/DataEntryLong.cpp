/**
	SPICE BIG
	DataEntryLong.cpp
	Purpose: Class to provide a data entry for type long integer

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
*/

#include "DataEntryLong.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryLong::DataEntryLong(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryLong::DataEntryLong(std::string name, int64_t minValue, int64_t maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryLong::DataEntryLong(std::string name, int64_t value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryLong::~DataEntryLong()
			{
			}

			bool DataEntryLong::setValue(int64_t value)
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
			int64_t DataEntryLong::getValue()
			{
				return std::stoll(_value);
			}
			DataEntry::Types DataEntryLong::getDataEntryType()
			{
				return DataEntry::Types::Long;
			}
			std::string DataEntryLong::getDataEntryTypeAsString()
			{
				return "Long";
			}
			std::string DataEntryLong::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryLong::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryLong::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					int64_t tempValue = std::stoll(valueString);
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