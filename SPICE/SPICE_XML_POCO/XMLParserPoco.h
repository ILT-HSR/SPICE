/**
	SPICE XML POCO
	XMLParserPoco.h
	Purpose: Poco-Implementation of the xml parser interface to provide XML functionalities to the core

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef XMLPARSERPOCO_H
#define XMLPARSERPOCO_H

#include "IXMLParser.h"

#include <string>
#include <vector>

#include "Poco/DOM/Document.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/DOMParser.h"

namespace SPICE
{
	namespace XML
	{
		namespace POCO
		{
			class XMLParserPoco : public SPICE::BIG::IXMLParser
			{
			public:
				/**
					Constructor to generate a new instace of the xml parser
				*/
				XMLParserPoco();
				virtual ~XMLParserPoco();

				/**
					Function is used to load the xml parser with the given xml structure. Normaly this function should be called just one time. If an other xml structure
					should be parsed, a new xml-parser-instance should be used

					@param xmlStructure A valid xml structure as string
					@return True if the xml structure could be loaded successful. False if a parsing error occured
				*/
				virtual bool loadXMLStructure(std::string xmlStructure);

				/**
					Gets a bool if an element exists at the given path (simple XPath with attributes-filter).
					Has no influence to the element-selection of other functions.

					@param path Path to the element
					@return True in case the element exists
				*/
				virtual bool getFirstElementAtPathExists(std::string path);
				/**
					Gets the inner text of the first element which can be found with the given path (simple XPath with attributes-filter).
					Has no influence to the element-selection of other functions.

					@param path Path to the element
					@return The innerPath of the element
				*/
				virtual std::string getFirstElementAtPathInnerText(std::string path);

				/**
					General DOM-function to select an element by name and namespace. If there is more then one element the first is selected.

					@param name Name of the element to select
					@param xmlNamespace The namespace in which the element has to be
					@return Returns true if an element could be selected. If false then there is no element selected anymore.
				*/
				virtual bool selectElementByNameAndNS(std::string name, std::string xmlNamespace);
				/**
					General DOM-function to select a child element of the current selected element by name and namespace. If there is more then one element the first is selected.

					@param name Name of the child element to select
					@param xmlNamespace The namespace in which the child element has to be
					@return Returns true if a child element could be selected. If false then the selected element is not changed.
				*/
				virtual bool selectChildByNameAndNS(std::string name, std::string xmlNamespace);
				/**
					General DOM-function to select a child element of the current selected element by name. If there is more then one element the first is selected.

					@param name Name of the child element to select
					@return Returns true if a child element could be selected. If false then the selected element is not changed.
				*/
				virtual bool selectChildByName(std::string name);
				/**
					General DOM-function to select the parent element of the current selected element.

					@return Returns true if the parent element could be selected. If false then the selecte element is not changed.
				*/
				virtual bool selectParent();
				/**
					Gets the innerText of the current selected element.

					@return The innerText of the current selected element
				*/
				virtual std::string getInnerText();
				/**
					Gets the innerText of the first child element (of the current selected element) with the given name.

					@param name Name of the child element
					@return The innerText of the child element
				*/
				virtual std::string getChildElementInnerText(std::string name);
				/**
					Function to get a vector-list with all child element names of the current selected element

					@return Vector with all child element names.
				*/
				virtual std::vector<std::string> getChildElementsList();

			private:
				bool _documentLoaded;
				Poco::XML::DOMParser _parser;
				Poco::AutoPtr<Poco::XML::Document> _document;
				Poco::XML::Node* _selectedNode;

			};
		}
	}
}
#endif // XMLPARSERPOCO_H
