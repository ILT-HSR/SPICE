/**
	SPICE BIG
	DataEntryInt.cpp
	Purpose: Class to provide a data entry for integer

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.1 2016_04_01
*/

#include "DataEntryInt.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryInt::DataEntryInt(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryInt::DataEntryInt(std::string name, int32_t minValue, int32_t maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryInt::DataEntryInt(std::string name, int32_t value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryInt::~DataEntryInt()
			{
			}

			bool DataEntryInt::setValue(int32_t value)
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
			int32_t DataEntryInt::getValue()
			{
				return std::stoi(_value);
			}
			DataEntry::Types DataEntryInt::getDataEntryType()
			{
				return DataEntry::Types::Int;
			}
			std::string DataEntryInt::getDataEntryTypeAsString()
			{
				return "Int";
			}
			std::string DataEntryInt::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryInt::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryInt::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					int tempValue = std::stoi(valueString);
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