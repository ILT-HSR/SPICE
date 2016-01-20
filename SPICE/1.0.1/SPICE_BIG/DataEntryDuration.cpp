/**
	SPICE BIG
	DataEntryDouble.cpp
	Purpose: Class to provide a data entry for type Duration

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "DataEntryDuration.h"

#include "SiLASOAPFunctions.h"

namespace SPICE
{
	namespace BIG
	{
		namespace DataEntryTypes
		{
			DataEntryDuration::DataEntryDuration(std::string name) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = "PT0S";
			}
			DataEntryDuration::DataEntryDuration(std::string name, double minValue, double maxValue) : DataEntry(name),
				_minValue(minValue),
				_maxValue(maxValue),
				_minMaxSetted(true)
			{
				_value = "PT0S";
			}
			DataEntryDuration::DataEntryDuration(std::string name, double value) : DataEntry(name),
				_minValue(0),
				_maxValue(0),
				_minMaxSetted(false)
			{
				_value = SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(value);
				_valueIsSet = true;
			}

			DataEntryDuration::~DataEntryDuration()
			{
			}

			bool DataEntryDuration::setValue(double value)
			{
				if(_minMaxSetted)
				{
					if(value < _minValue || value > _maxValue)
					{
						return false;
					}
				}
				_value = SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(value);
				_valueIsSet = true;
				return true;
			}
			double DataEntryDuration::getValue()
			{
				double returnValue = 0;
				SPICE::BIG::SiLASOAPFunctions::getDoubleSecondsFromDuration(_value, returnValue);
				return returnValue;
			}
			DataEntry::Types DataEntryDuration::getDataEntryType()
			{
				return DataEntry::Types::Duration;
			}
			std::string DataEntryDuration::getDataEntryTypeAsString()
			{
				return "Duration";
			}
			std::string DataEntryDuration::getMinValue()
			{
				if(_minMaxSetted)
				{
					return SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(_minValue);
				}
				return "";
			}
			std::string DataEntryDuration::getMaxValue()
			{
				if(_minMaxSetted)
				{
					return SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(_maxValue);
				}
				return "";
			}
			bool DataEntryDuration::checkStringIsValidValue(std::string& valueString)
			{
				double doubleValue = 0;

				if(!SPICE::BIG::SiLASOAPFunctions::getDoubleSecondsFromDuration(valueString, doubleValue))
				{
					return false;
				}

				if(_minMaxSetted)
				{
					if(doubleValue < _minValue || doubleValue > _maxValue)
					{
						return false;
					}
				}

				valueString = SPICE::BIG::SiLASOAPFunctions::getDurationFromDoubleSeconds(doubleValue);
				return true;
			}

		}
	}
}