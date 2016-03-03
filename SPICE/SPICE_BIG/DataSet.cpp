/**
	SPICE BIG
	DataSet.cpp
	Purpose: DataSet provides container-functionalities for instances of DataEntry and XML-functionalities for the core.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include <sstream>
#include <fstream>

#include "DataSet.h"

#include "GeneralFunctions.h"
#include "SiLASOAPFunctions.h"

namespace SPICE
{
	namespace BIG
	{
		DataSet::DataSet(std::string password) :
			_password(password)
		{
		}

		DataSet::~DataSet()
		{
		}

		void DataSet::addDataEntry(std::shared_ptr<DataEntry> dataEntry)
		{
			_dataEntryList.push_back(dataEntry);
		}
		unsigned int DataSet::getDataEntriesCount()
		{
			return _dataEntryList.size();
		}
		std::shared_ptr<DataEntry> DataSet::getDataEntryByIndex(unsigned int index)
		{
			if(index >= 0 && index < _dataEntryList.size())
			{
				return _dataEntryList[index];
			}
			return nullptr;
		}
		std::shared_ptr<DataEntry> DataSet::getFirstDataEntryByName(std::string name)
		{
			for(std::vector<std::shared_ptr<DataEntry>>::iterator i = _dataEntryList.begin(); i != _dataEntryList.end(); ++i)
			{
				if((*i)->getName() == name)
				{
					return (*i);
				}
			}
			return nullptr;
		}

		std::string DataSet::getXMLParameterSet(std::string preTabs)
		{
			if(getDataEntriesCount() > 0)
			{
				std::ostringstream strout;
				strout << preTabs << "<ParameterSet>\n";
				for(std::vector<std::shared_ptr<DataEntry>>::iterator i = _dataEntryList.begin(); i != _dataEntryList.end(); ++i)
				{
					strout << (*i)->getAsXMLParameter(preTabs + "\t");
				}
				strout << preTabs << "</ParameterSet>\n";
				return strout.str();
			}
			else
			{
				return "";
			}
		}

		bool DataSet::writeParameterSetToPath(std::string path)
		{
			std::string file = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" + getXMLParameterSet();

			std::ofstream writeFile;
			try
			{
				writeFile.open(path, std::ios::trunc);
				if(writeFile.is_open())
				{
					writeFile << file;
					writeFile.flush();
					writeFile.close();
				}
				else
				{
					writeFile.close();
					return false;
				}
			}
			catch (std::exception e)
			{
				writeFile.close();
				return false;
			}
			SPICE::BIG::GeneralFunctions::wroteFileToDisk();
			return true;
		}
		bool DataSet::loadParameterSetFromPath(std::string path, std::shared_ptr<IXMLParser> xmlParser)
		{
			std::ifstream readStream;
			std::string file = "";
			try
			{
				readStream.open(path);

				while(readStream.is_open() && !readStream.eof())
				{
					std::string line;
					std::getline(readStream, line);
					file += line;
				}
				readStream.close();
			}
			catch (std::exception e)
			{
				readStream.close();
				return false;
			}

			std::string message = "";
			return parseParameterSetFromXMLFile(file, xmlParser) == "";
		}
		std::string DataSet::parseParameterSetFromXMLFile(std::string xmlFile, std::shared_ptr<IXMLParser> xmlParser)
		{
			std::string errorMessage = "";
			xmlFile = SPICE::BIG::SiLASOAPFunctions::dequoteXMLFile(xmlFile);

            if(xmlParser->loadXMLStructure(xmlFile))
			{
                unsigned int count = getDataEntriesCount();

                for(unsigned int i = 0; i < count; i++)
				{
                    std::shared_ptr<SPICE::BIG::DataEntry> parameter = getDataEntryByIndex(i);

                    std::string path = "/ParameterSet/Parameter[@name='" + parameter->getName() + "']/" + parameter->getDataEntryTypeAsString();

                    if(xmlParser->getFirstElementAtPathExists(path))
					{
	                    std::string value = xmlParser->getFirstElementAtPathInnerText(path);
                        if(!parameter->setValueString(value))
						{
							if(errorMessage != "")
							{
								errorMessage += " - ";
							}
							std::string message = "Parameter \"" + parameter->getName() + "\" has invalid value. ";
							if(parameter->getMinValue() != "" || parameter->getMaxValue() != "")
							{
								message += "Range is " + parameter->getMinValue() + ".." + parameter->getMaxValue() + ". ";
							}
							if(parameter->getUnit() != "")
							{
								message += "Unit: " + parameter->getUnit() + ". ";
							}
							message += parameter->getDescription();
							errorMessage += message;
						}
					}
				}
			}
			else
			{
				errorMessage = "XML-format-error. No parameter changed.";
			}
			return errorMessage;
		}

		std::string DataSet::getPassword()
		{
			return _password;
		}
		void DataSet::setPassword(std::string newPassword)
		{
			_password = newPassword;
		}
	}
}