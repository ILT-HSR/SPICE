/**
	SPICE BIG
	SiLASOAPFunctions.cpp
	Purpose: Provides functionalities to generate SiLASOAPMessages

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "SiLASOAPFunctions.h"

#include <sstream>
#include <cmath>

namespace SPICE
{
	namespace BIG
	{
        std::string SiLASOAPFunctions::createSyncResponseXML(std::string commandName, int returnCode, std::string message, std::string duration, int deviceClass, std::string otherParameters, std::string silaNamespace, std::string silaNSPrefix)
		{
			silaNSPrefix = validateNSPrefix(silaNSPrefix);
			std::string silaNSSuffix = getSuffixFromPrefix(silaNSPrefix);
            std::ostringstream strout;

			strout << "<" << silaNSPrefix << commandName << "Response xmlns" << silaNSSuffix << "=\"" + silaNamespace + "\">";
			strout << SiLASOAPFunctions::createReturnValue(returnCode, message, duration, deviceClass, silaNSPrefix, commandName + "Result");
            strout << otherParameters;
			strout << "</" << silaNSPrefix << commandName << "Response>";
			std::string returnValue = strout.str();

            SiLASOAPFunctions::encloseWithSOAPFrame(returnValue);
            return returnValue;
		}
		std::string SiLASOAPFunctions::createResponseEvent(int requestId, int returnCode, std::string message, double duration, int deviceClass, std::string responseData, std::string silaNSPrefix, std::string silaNamespace)
		{
			std::ostringstream responseDataXML;
			responseDataXML << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
			if(responseData != "")
			{
				responseDataXML << responseData;
			}
			else
			{
				responseDataXML << "<ResponseData xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"ResponseType_1.2.xsd\"/>\n";
			}

			silaNSPrefix = validateNSPrefix(silaNSPrefix);
			std::string silaNSSuffix = getSuffixFromPrefix(silaNSPrefix);
			std::ostringstream strout;
			strout << "<" << silaNSPrefix << "ResponseEvent xmlns" << silaNSSuffix << "=\"" << silaNamespace << "\">";
			strout << "<" << silaNSPrefix << "requestId>" << requestId << "</" << silaNSPrefix << "requestId>";
			strout << SiLASOAPFunctions::createReturnValue(returnCode, message,duration, deviceClass, silaNSPrefix);
			strout << "<" << silaNSPrefix << "responseData>" << quoteXMLFile(responseDataXML.str()) << "</" << silaNSPrefix << "responseData>";
			strout << "</" << silaNSPrefix << "ResponseEvent>";

			std::string returnString = strout.str();
			encloseWithSOAPFrame(returnString);
			return returnString;
		}
		std::string SiLASOAPFunctions::createErrorEvent(int requestId, int returnCode, std::string message, double duration, int deviceClass, std::string continuationTask, std::string silaNSPrefix, std::string silaNamespace)
		{
			std::ostringstream continuationTaskXML;
			continuationTaskXML << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			continuationTaskXML << "<ContinuationTask xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"ContinuationTask.xsd\">";
			continuationTaskXML << continuationTask;
			continuationTaskXML << "</ContinuationTask>";

			silaNSPrefix = validateNSPrefix(silaNSPrefix);
			std::string silaNSSuffix = getSuffixFromPrefix(silaNSPrefix);
			std::ostringstream strout;
			strout << "<" << silaNSPrefix << "ErrorEvent xmlns" << silaNSSuffix << "=\"" << silaNamespace << "\">";
			strout << "<" << silaNSPrefix << "requestId>" << requestId << "</" << silaNSPrefix << "requestId>";
			strout << SiLASOAPFunctions::createReturnValue(returnCode, message,duration, deviceClass, silaNSPrefix);
			strout << "<" << silaNSPrefix << "continuationTask>" << quoteXMLFile(continuationTaskXML.str()) << "</" << silaNSPrefix << "continuationTask>";
			strout << "</" << silaNSPrefix << "ErrorEvent>";

			std::string returnString = strout.str();
			encloseWithSOAPFrame(returnString);
			return returnString;
		}
		std::string SiLASOAPFunctions::createReturnValue(int returnCode, std::string message, std::string duration, int deviceClass, std::string silaNSPrefix, std::string elementName)
		{
			std::ostringstream strout;
			strout << "<" << silaNSPrefix << elementName << ">";
			strout << "<" << silaNSPrefix << "returnCode>" << returnCode << "</" << silaNSPrefix << "returnCode><" << silaNSPrefix << "message>" <<
				message << "</" << silaNSPrefix << "message><" << silaNSPrefix << "duration>" << duration << "</" << silaNSPrefix << "duration><" << silaNSPrefix << "deviceClass>" <<
				deviceClass << "</" << silaNSPrefix << "deviceClass>";
			strout << "</" << silaNSPrefix << elementName << ">";

			return strout.str();
		}
		std::string SiLASOAPFunctions::createReturnValue(int returnCode, std::string message, double duration, int deviceClass, std::string silaNSPrefix)
		{
            return createReturnValue(returnCode, message, getDurationFromDoubleSeconds(duration), deviceClass, silaNSPrefix, "returnValue");
		}
		void SiLASOAPFunctions::encloseWithSOAPFrame(std::string& frameContent)
		{
			std::ostringstream strout;

			strout << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			strout << "<soap:Envelope xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">";
			strout << "<soap:Body>";
			strout << frameContent;
			strout << "</soap:Body>";
			strout << "</soap:Envelope>";

			frameContent = strout.str();
		}

		std::string SiLASOAPFunctions::quoteXMLFile(std::string xmlFile)
		{
			std::string returnString = xmlFile;
			replaceInString(returnString, "<", "&lt;");
			replaceInString(returnString, ">", "&gt;");
			replaceInString(returnString, "\"", "&quot;");
			replaceInString(returnString, "\'", "&apos;");
			return returnString;
		}
		std::string SiLASOAPFunctions::dequoteXMLFile(std::string xmlFile)
		{
			std::string returnString = xmlFile;
			replaceInString(returnString, "&lt;", "<");
			replaceInString(returnString, "&gt;", ">");
			replaceInString(returnString, "&quot;", "\"");
			replaceInString(returnString, "&apos;", "\'");
			return returnString;
		}

		void SiLASOAPFunctions::replaceInString(std::string& contentString, std::string searchString, std::string replaceString)
		{
			size_t searchStringLength = searchString.length();
			if(searchStringLength > 0)
			{
				size_t pos = 0;

				while((pos = contentString.find(searchString, pos)) != std::string::npos)
				{
					contentString.replace(pos, searchStringLength, replaceString);
				}
			}
		}
		std::string SiLASOAPFunctions::validateNSPrefix(std::string nsPrefix)
		{
			if(nsPrefix.find(":") == nsPrefix.length() - 1)
			{
				return nsPrefix;
			}
			return "";
		}

		std::string SiLASOAPFunctions::getDurationFromDoubleSeconds(double seconds)
		{
			if(seconds < 0)
			{
				return "PT0S";
			}
			else
			{
				double onlySeconds = 0;
				double minutes = 0;
				double hours = 0;
				double days = 0;

				onlySeconds = modf((seconds/60), &minutes);
				onlySeconds = onlySeconds*60;

				if(minutes > 0)
				{
					minutes = modf((minutes/60), &hours);
					minutes = minutes * 60;
				
					if(hours > 0)
					{
						hours = modf((minutes/24), &days);
						hours = minutes * 24;
					}
				}

				std::ostringstream strout;
				strout << "P";
				bool anyValueWritten = false;
				if(days > 0)
				{
					strout << days << "D";
					anyValueWritten = true;
				}
				if(!anyValueWritten || hours != 0 || minutes != 0 || onlySeconds != 0)
				{
					strout << "T";
				}
				if(hours > 0)
				{
					strout << hours << "H";
					anyValueWritten = true;
				}
				if(minutes > 0)
				{
					strout << minutes << "M";
					anyValueWritten = true;
				}
				if(!anyValueWritten || onlySeconds != 0)
				{
					strout << onlySeconds << "S";
				}
			
				return strout.str();
			}
		}
		bool SiLASOAPFunctions::getDoubleSecondsFromDuration(std::string duration, double& doubleSeconds)
		{
			double days = 0;
			double hours = 0;
			double minutes = 0;
			double seconds = 0;

			double currentNr = 0;
			int divisionCounter = 0;

			int mode = 0; // 0 - wait on P, 1 - Y, 2 - M, 3 - W, 4 - D, 5 - T, 6 - H, 7 - M, 8 - ., 9 - S

			for(unsigned int i = 0; i < duration.length(); i++)
			{
				unsigned char currentChar = duration[i];

				if(mode == 0)
				{
					if(currentChar == 'P')
					{
						mode++;
					}
					else
					{
						return false;
					}
				}
				else
				{
					if(currentChar >= 48 && currentChar <= 57)
					{
						currentNr = currentNr * 10;
						currentNr += (currentChar - 48);
						if(mode == 8)
						{
							divisionCounter += 1;
						}
					}
					else if(currentChar == 'Y')
					{
						if(mode < 1)
						{
							days += currentNr * 360;
							currentNr = 0;
							mode = 1;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == 'M' && mode < 5)
					{
						if(mode < 2)
						{
							days += currentNr * 30;
							currentNr = 0;
							mode = 2;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == 'W')
					{
						if(mode < 3)
						{
							days += currentNr * 7;
							currentNr = 0;
							mode = 3;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == 'D')
					{
						if(mode < 4)
						{
							days += currentNr;
							currentNr = 0;
							mode = 4;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == 'T')
					{
						if(mode < 5)
						{
							mode = 5;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == 'H')
					{
						if(mode == 5)
						{
							hours = currentNr;
							currentNr = 0;
							mode = 6;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == 'M')
					{
						if(mode >= 5 && mode < 7)
						{
							minutes = currentNr;
							currentNr = 0;
							mode = 7;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == '.')
					{
						if(mode >= 5 && mode < 8)
						{
							seconds = currentNr;
							currentNr = 0;
							mode = 8;
						}
						else
						{
							return false;
						}
					}
					else if(currentChar == 'S')
					{
						if(mode >= 5 && mode < 9)
						{
							if(mode == 8)
							{
								for(int i = 0; i < divisionCounter; i++)
								{
									currentNr = currentNr / 10;
								}
								seconds = seconds + currentNr;
							}
							else
							{
								seconds = currentNr;
							}
							currentNr = 0;
							mode = 9;
						}
						else
						{
							return false;
						}
					}
				}
			}
			doubleSeconds = (((((days * 24) + hours) * 60) + minutes) * 60) + seconds;
			return true;
		}

		std::string SiLASOAPFunctions::getSuffixFromPrefix(std::string nsPrefix)
		{
			std::string localPrefix = validateNSPrefix(nsPrefix);

			if(localPrefix == "")
			{
				return "";
			}
			else
			{
				return ":" + nsPrefix.substr(0, nsPrefix.length() - 1);
			}
		}


	}
}