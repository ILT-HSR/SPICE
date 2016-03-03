/**
	SPICE BIG
	DataEntry.h
	Purpose: Abstract class to provide a data entry independant of his type

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef DATAENTRY_H
#define DATAENTRY_H

#include <string>

namespace SPICE
{
	namespace BIG
	{
		class DataEntry
		{
			// Methoden
		public:
			enum Types
			{
				Long,
				Int,
				Short,
				Byte,
				unsignedLong,
				unsignedInt,
				unsignedShort,
				unsignedByte,
				Float,
				Double,
				Boolean,
				String,
//				Enumeration,
				dateTime,
				Duration,
//				array,
				xmldocument,
			};


			/**
				Abstract class to provide a data entry independant of his type 

				@param name Name of the DataEntry
			*/
			DataEntry(std::string name);
			virtual ~DataEntry();

			/**
				Sets additional informations which will be needed to create the WSDL-File. To exclude attributes from the WDSL, just set an empty string.

				@param description Human readable description of the DataEntry.
				@param unit Unit of the DataEntry value
				@param defaultValue A default value, which can be submited to the PMS. Attention: No testing if string is correct for the type.
			*/
			void setAdditionalInformations(std::string description, std::string unit, std::string defaultValue);
			/**
				Sets the isNeeded-Flag of the DataEntry.

				@param isNeeded New value for isNeeded-Flag
			*/
			void setIsNeeded(bool isNeeded);

			/**
				Sets the value as string. There is a testing, if it suitable for the type. And if set the min and max limits will be checked too.
				If ok, the value will be set.

				@param valueString The new value as string
				@return True if the value could be set. (Suitable for type, limits ok)
			*/
			bool setValueString(std::string valueString);
			/**
				Gets the value as string.

				@return Value as string. If value is not set the inital value will be returned.
			*/
			std::string getValueAsString();

			/**
				Gets the information if the value has been set.

				@return True if the value has been set.
			*/
			bool getIsSet();
			/**
				Gets the isNeeded-Flag of the DataEntry.

				@return value of isNeeded-Flag
			*/
			bool getIsNeeded();

			/**
				Gets the isInParameter-Flag of the DataEntry

				@return value of isInParameter-Flag
			*/
			bool getIsInParameter();

			/**
				Gets the isOutParameter-Flag of the DataEntry

				@return value of isOutParameter-Flag
			*/
			bool getIsOutParameter();

			/**
				Gets the name of the DataEntry.

				@return name
			*/
			std::string getName();
			/**
				Gets the type of the DataEntry.

				@return type of the DataEntry
			*/
			virtual Types getDataEntryType() = 0;
			/**
				Gets the type of the DataEntry as string.

				@return type of the DataEntry as string
			*/
			virtual std::string getDataEntryTypeAsString() = 0;
			/**
				Gets the minimum value of the DataEntry. Returns an empty string if no minValue is set.

				@return minValue
			*/
			virtual std::string getMinValue() = 0;
			/**
				Gets the maximum value of the DataEntry. Returns an empty string if no maxValue is set.

				@return maxValue
			*/
			virtual std::string getMaxValue() = 0;
			/**
				Gets the unit of the DataEntry.

				@return unit
			*/
			std::string getUnit();
			/**
				Gets the default value of the DataEntry.

				@return defaultValue
			*/
			std::string getDefaultValue();
			/**
				Gets the description of the DataEntry.

				@return description
			*/
			std::string getDescription();

			/**
				Gets the parameter as soap xml parameter.

				@param preTabs Optional prefix tabulators used at each line to be able to get a human readable xml file
				@return XML parameter
			*/
			std::string getAsXMLParameter(std::string preTabs = "");

		protected:

			/**
				Checks the given string if it is suitable for the type. If set the limits are also checked.

				@param valueString String with value which should be checked. Can be changed from the function for cosmetical reasons (example remove a space)
				@return True if valueString is ok and can be set.
			*/
			virtual bool checkStringIsValidValue(std::string& valueString) = 0;

			bool _valueIsSet;
			bool _isNeeded;
			bool _emptyStringAllowed;
			std::string _value;
			std::string _name;
			std::string _unit;
			std::string _defaultValue;
			std::string _description;


		};
	}
}
#endif // DATAENTRY_H
