/**
	SPICE BIG
	SiLASOAPFunctions.h
	Purpose: Provides functionalities to generate SiLASOAPMessages

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef SILASOAPFUNCTIONS_H
#define SILASOAPFUNCTIONS_H

#include <string>

namespace SPICE
{
	namespace BIG
	{
		class SiLASOAPFunctions
		{
		public:
			/**
				General function to create the sync response SOAP message

				@param commandName The commandName for which the response is created
				@param returnCode The return code of the sync response
				@param message The human readable message according to the return code
				@param duration The expected duration to execute the asynchronous command
				@param deviceClass The device class number
				@param otherParameters XML-formated string with other parameters to include in the sync response
				@param silaNamespace The from the service consumer used sila namespace to be able to answer with adapted namespaces
				@param silaNSPrefix Prefix if the silaNamespace should not be included over an global namespace.
				@return The full created sync response xml document as string
			*/
            static std::string createSyncResponseXML(std::string commandName, int returnCode, std::string message, std::string duration, int deviceClass, std::string otherParameters, std::string silaNamespace, std::string silaNSPrefix);
			/**
				General function to create the async response event SOAP message

				@param requestId The requestId of the responsing command
				@param returnCode The return code of the async response
				@param message The human readable message according to the return code
				@param duration The finaly used duration to execute the asynchronous command
				@param deviceClass The device class number
				@param responseData Field to optional include the ResponseData-xmlDocument-content. XML-Document declaration (version and encoding) and the quoting of the file is done inside.
				@param silaNSPrefix Prefix if the silaNamespace should not be included over an global namespace.
				@param silaNamespace The from the service consumer used sila namespace to be able to answer with adapted namespaces
				@return The full created async response event xml document as string
			*/
			static std::string createResponseEvent(int requestId, int returnCode, std::string message, double duration, int deviceClass, std::string responseData, std::string silaNSPrefix, std::string silaNamespace);
			/**
				General function to create the error event SOAP message

				@param requestId The requestId of the error event generating command
				@param returnCode The error code
				@param message The human readable error message
				@param duration Duration between start of the command and the error
				@param deviceClass The device class number
				@param continuationTask String to include the continuationTask-xmlDocument-content. XML-Document declaration (version and encoding), base-element and the quoting of the file is done inside.
				@param silaNSPrefix Prefix if the silaNamespace should not be included over an global namespace.
				@param silaNamespace The from the service consumer used sila namespace to be able to answer with adapted namespaces
				@return The full created error event xml document as string
			*/
			static std::string createErrorEvent(int requestId, int returnCode, std::string message, double duration, int deviceClass, std::string continuationTask, std::string silaNSPrefix, std::string silaNamespace);
			/**
				Function to create a return value

				@param returnCode The return code
				@param message The human readable message according to the return code
				@param duration The duration to add. Meaning is depending on the situation.
				@param deviceClass The device class number
				@param silaNSPrefix Prefix if the silaNamespace should not be included over an global namespace.
				@param elementName The base element name of the generated return value
				@return The generated return value as string
			*/
			static std::string createReturnValue(int returnCode, std::string message, std::string duration, int deviceClass, std::string silaNSPrefix, std::string elementName);

			/**
				Function to create a return value. As base element name "returnValue" is used

				@param returnCode The return code
				@param message The human readable message according to the return code
				@param duration The duration to add. Meaning is depending on the situation.
				@param deviceClass The device class number
				@param silaNSPrefix Prefix if the silaNamespace should not be included over an global namespace.
				@return The generated return value as string
			*/
			static std::string createReturnValue(int returnCode, std::string message, double duration, int deviceClass, std::string silaNSPrefix);
			/**
				General function to enclose a SOAP body content with the needed soap frame

				@param frameContent The referenced string with the frameContent. String is changed and enclosed with the soap frame
			*/
			static void encloseWithSOAPFrame(std::string& frameContent);

			/**
				Function to return a quoted version of a given xmlFile. Replaced signs: < > " '

				@param xmlFile The file to quote
				@return The quoted file
			*/
			static std::string quoteXMLFile(std::string xmlFile);
			/**
				Function to return a dequoted version of a given xmlFile. Replaced signs: < > " '

				@param xmlFile The file to dequote
				@return The dequoted file
			*/
			static std::string dequoteXMLFile(std::string xmlFile);

			/**
				A simple function to replace a searched string with a given replaced string. It's a simple function and is not be able to avoid problems if the searchString and replaceString can be found in each other.

				@param contentString A string given as reference which is changed during the replacing
				@param searchString The string to search for (which should be replaced)
				@param replaceString The string to replace with
			*/
			static void replaceInString(std::string& contentString, std::string searchString, std::string replaceString);
			/**
				Function validates the namespace prefix. If the prefix is invalid the returnValue will be set to an empty string

				@param nsPrefix The namespace prefix which should be validated.
				@return The validated (or maybe emtied) string
			*/
			static std::string validateNSPrefix(std::string nsPrefix);

			/**
				Convertes the given time value to a string formated according ISO8601. Negativ values are returned as 0s-value

				@param seconds The given seconds to convert
				@return The generated string which represents the given seconds as duration
			*/
			static std::string getDurationFromDoubleSeconds(double seconds);
			/**
				Convertes a String formated according ISO8601 to a double seconds value

				@param duration Duration string
				@param doubleSeconds The converted double seconds value
				@return True if Duration string is valid and could be converted
			*/
			static bool getDoubleSecondsFromDuration(std::string duration, double& doubleSeconds);

		private:

			/**
				Function gets the namespace suffix for the given namespace prefix

				@param nsPrefix The namespace prefix which should be used to create a suffix
				@return The namespace suffix
			*/
			static std::string getSuffixFromPrefix(std::string nsPrefix);
		};
	}
}

#endif // SILASOAPFUNCTIONS_H
