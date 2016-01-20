/**
	SPICE BIG
	DataEntryFloat.cpp
	Purpose: Class to provide a data entry for type float

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryFloat.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryFloat::DataEntryFloat(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryFloat::DataEntryFloat(std::string name, float minValue, float maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryFloat::DataEntryFloat(std::string name, float value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryFloat::~DataEntryFloat()
			{
			}

			bool DataEntryFloat::setValue(float value)
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
			float DataEntryFloat::getValue()
			{
				return std::stof(_value);
			}
			DataEntry::Types DataEntryFloat::getDataEntryType()
			{
				return DataEntry::Types::Float;
			}
			std::string DataEntryFloat::getDataEntryTypeAsString()
			{
				return "Float";
			}
			std::string DataEntryFloat::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryFloat::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryFloat::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					float tempValue = std::stof(valueString);
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