/**
	SPICE BIG
	IXMLParser.h
	Purpose: Interface to provide XML functionalities to the core

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef IXMLPARSER_H
#define IXMLPARSER_H

#include <string>
#include <vector>

namespace SPICE
{
	namespace BIG
	{
		class IXMLParser
		{
			public:
				/**
					Function is used to load the xml parser with the given xml structure. Normaly this function should be called just one time. If an other xml structure
					should be parsed, a new xml-parser-instance should be used

					@param xmlStructure A valid xml structure as string
					@return True if the xml structure could be loaded successful. False if a parsing error occured
				*/
				virtual bool loadXMLStructure(std::string xmlStructure) = 0;

				/**
					Gets a bool if an element exists at the given path (simple XPath with attributes-filter).
					Has no influence to the element-selection of other functions.

					@param path Path to the element
					@return True in case the element exists
				*/
				virtual bool getFirstElementAtPathExists(std::string path) = 0;
				/**
					Gets the inner text of the first element which can be found with the given path (simple XPath with attributes-filter).
					Has no influence to the element-selection of other functions.

					@param path Path to the element
					@return The innerPath of the element
				*/
				virtual std::string getFirstElementAtPathInnerText(std::string path) = 0;

				/**
					General DOM-function to select an element by name and namespace. If there is more then one element the first is selected.

					@param name Name of the element to select
					@param xmlNamespace The namespace in which the element has to be
					@return Returns true if an element could be selected. If false then there is no element selected anymore.
				*/
				virtual bool selectElementByNameAndNS(std::string name, std::string xmlNamespace) = 0;
				/**
					General DOM-function to select a child element of the current selected element by name and namespace. If there is more then one element the first is selected.

					@param name Name of the child element to select
					@param xmlNamespace The namespace in which the child element has to be
					@return Returns true if a child element could be selected. If false then the selected element is not changed.
				*/
				virtual bool selectChildByNameAndNS(std::string name, std::string xmlNamespace) = 0;
				/**
					General DOM-function to select a child element of the current selected element by name. If there is more then one element the first is selected.

					@param name Name of the child element to select
					@return Returns true if a child element could be selected. If false then the selected element is not changed.
				*/
				virtual bool selectChildByName(std::string name) = 0;
				/**
					General DOM-function to select the parent element of the current selected element.

					@return Returns true if the parent element could be selected. If false then the selecte element is not changed.
				*/
				virtual bool selectParent() = 0;
				/**
					Gets the innerText of the current selected element.

					@return The innerText of the current selected element
				*/
				virtual std::string getInnerText() = 0;
				/**
					Gets the innerText of the first child element (of the current selected element) with the given name.

					@param name Name of the child element
					@return The innerText of the child element
				*/
				virtual std::string getChildElementInnerText(std::string name) = 0;
				/**
					Function to get a vector-list with all child element names of the current selected element

					@return Vector with all child element names.
				*/
				virtual std::vector<std::string> getChildElementsList() = 0;

		};
	}
}
#endif // IXMLPARSER_H
