/**
	SPICE BIG
	DataEntryDouble.cpp
	Purpose: Class to provide a data entry for type double

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryDouble.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryDouble::DataEntryDouble(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "0";
			}
			DataEntryDouble::DataEntryDouble(std::string name, double minValue, double maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "0";
			}
			DataEntryDouble::DataEntryDouble(std::string name, double value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = std::to_string(value);
				_valueIsSet = true;
			}

			DataEntryDouble::~DataEntryDouble()
			{
			}

			bool DataEntryDouble::setValue(double value)
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
			double DataEntryDouble::getValue()
			{
				return std::stod(_value);
			}
			DataEntry::Types DataEntryDouble::getDataEntryType()
			{
				return DataEntry::Types::Double;
			}
			std::string DataEntryDouble::getDataEntryTypeAsString()
			{
				return "Double";
			}
			std::string DataEntryDouble::getMinValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_minValue);
				}
				return "";
			}
			std::string DataEntryDouble::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return std::to_string(_maxValue);
				}
				return "";
			}
			bool DataEntryDouble::checkStringIsValidValue(std::string& valueString)
			{
				bool returnValue = true;
				try
				{
					double tempValue = std::stod(valueString);
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