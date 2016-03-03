/**
	SPICE BIG
	DataSet.h
	Purpose: DataSet provides container-functionalities for instances of DataEntry and XML-functionalities for the core.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef DATASET_H
#define DATASET_H

#include <string>
#include <memory>
#include <vector>

#include "DataEntry.h"
#include "IXMLParser.h"

namespace SPICE
{
	namespace BIG
	{
		class DataSet
		{
			public:
				/**
					DataSet provides container-functionalities for instances of DataEntry and XML-functionalities for the core. 

					@param password Optional password. Doesn't blocks the class-functions. Is just in case to test from the PMS given passwords, as example for configuration levels
				*/
				DataSet(std::string password = "");
				virtual ~DataSet();

				/**
					Adds the given DataEntry to the internal list. 

					@param dataEntry DataEntry to add
				*/
				void addDataEntry(std::shared_ptr<DataEntry> dataEntry);
				/**
					Gets the count of DataEntries 

					@return Count of DataEntries
				*/
				unsigned int getDataEntriesCount();
				/**
					Gets the dataEntry of the list at the given index.

					@param index Index number of the data entry to get.
					@return DataEntry at the index. Returns null if no entry exists
				*/
				std::shared_ptr<DataEntry> getDataEntryByIndex(unsigned int index);
				/**
					Gets the first data entry of the lists which matches to the given name

					@param name Name of the data entry to get. If more then one data entry with that name exists, the first is returned.
					@return DataEntry matches to the name. Returns null if no entry exists.
				*/
				std::shared_ptr<DataEntry> getFirstDataEntryByName(std::string name);

				/**
					Gets the DataEntry as XMLParameterSet.

					@param preTabs Optional prefix tabulators used at each line to be able to get a human readable xml file
					@return DataEntry as XMLParameterSet
				*/
				std::string getXMLParameterSet(std::string preTabs = "");

				/**
					Writes the ParameterSet as XML-file to the given path

					@param path Path on the filesystem to write the file
					@return True if the file could be written successfuly
				*/
				bool writeParameterSetToPath(std::string path);
				/**
					Load the ParameterSet from an XML-file at the given path

					@param path Path on the filesystem to load the file from
					@param xmlParser A new instance of an XML-parser to load the file
					@return True if the ParameterSet could be loaded successfuly
				*/
				bool loadParameterSetFromPath(std::string path, std::shared_ptr<IXMLParser> xmlParser);
				/**
					Parses an XML-file-string to this parameterSet. Only fails if the file could not be parsed or a parameter doesn't fulfill the requirements.
					Does NOT fail if one or more or all parameters could not be found.

					@param xmlFile The XML-file to parse
					@param xmlParser A new instance of an XML-parser
					@return Returns the errorMessage. Empty string in case that no failure occured and no parameter requirements where incorrect.
				*/
				std::string parseParameterSetFromXMLFile(std::string xmlFile, std::shared_ptr<IXMLParser> xmlParser);

				/**
					Gets the current password

					@return Current password
				*/
				std::string getPassword();
				/**
					Set a new password

					@param newPassword New password string
				*/
				void setPassword(std::string newPassword);

			protected:
			private :
				std::vector<std::shared_ptr<DataEntry>> _dataEntryList;
				std::string _password;
		};
	}
}
#endif // DATASET_H
