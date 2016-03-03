/**
	SPICE BIG
	DataEntry.cpp
	Purpose: Abstract class to provide a data entry independant of his type

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include <sstream>

#include "DataEntry.h"

namespace SPICE
{
	namespace BIG
	{
		DataEntry::DataEntry(std::string name) :
			_valueIsSet(false),
			_isNeeded(false),
			_emptyStringAllowed(false),
			_value(""),
			_name(name),
			_unit(""),
			_defaultValue(""),
			_description("")
		{
		}

		DataEntry::~DataEntry()
		{
		}

		void DataEntry::setAdditionalInformations(std::string description, std::string unit, std::string defaultValue)
		{
			_description = description;
			_unit = unit;
			_defaultValue = defaultValue;
		}
		void DataEntry::setIsNeeded(bool isNeeded)
		{
			_isNeeded = isNeeded;
		}
		bool DataEntry::setValueString(std::string valueString)
		{
			if(valueString == "" && !_emptyStringAllowed)
			{
				return false;
			}
			std::string tempString = valueString;
			if(checkStringIsValidValue(tempString))
			{
				_value = tempString;
				_valueIsSet = true;
				return true;
			}
			return false;
		}
		std::string DataEntry::getValueAsString()
		{
			return _value;
		}
		bool DataEntry::getIsSet()
		{
			return _valueIsSet;
		}
		bool DataEntry::getIsNeeded()
		{
			return _isNeeded;
		}
		std::string DataEntry::getName()
		{
			return _name;
		}
		std::string DataEntry::getUnit()
		{
			return _unit;
		}
		std::string DataEntry::getDefaultValue()
		{
			return _defaultValue;
		}
		std::string DataEntry::getDescription()
		{
			return _description;
		}
		std::string DataEntry::getAsXMLParameter(std::string preTabs)
		{
			std::string dataEntryTypeString = getDataEntryTypeAsString();

			std::ostringstream strout;
			strout << preTabs << "<Parameter parameterType=\"" << dataEntryTypeString << "\" name=\"" << _name << "\">\n";
			strout << preTabs << "\t<" << dataEntryTypeString << ">" << _value << "</" << dataEntryTypeString << ">\n";
			strout << preTabs << "</Parameter>\n";
			return strout.str();
		}


	}
}