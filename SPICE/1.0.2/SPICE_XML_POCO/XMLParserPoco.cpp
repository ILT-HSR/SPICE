/**
	SPICE XML POCO
	XMLParserPoco.cpp
	Purpose: Interface to provide XML functionalities to the core

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "XMLParserPoco.h"

#include "Poco/DOM/NodeList.h"

namespace SPICE
{
	namespace XML
	{
		namespace POCO
		{
			XMLParserPoco::XMLParserPoco() :
				_documentLoaded(false),
				_selectedNode(nullptr)
			{
			}
			XMLParserPoco::~XMLParserPoco()
			{
			}

			bool XMLParserPoco::loadXMLStructure(std::string xmlStructure)
			{
				try
				{
					_document = _parser.parseString(xmlStructure);
					_documentLoaded = true;
				}
				catch (std::exception e)
				{
					_documentLoaded = false;
					return false;
				}
				return true;
			}

			bool XMLParserPoco::getFirstElementAtPathExists(std::string path)
			{
				if(_documentLoaded)
				{
					Poco::XML::Node* node = _document->getNodeByPath(path);
					if(node != nullptr)
					{
						return true;
					}
				}
				return false;
			}
			std::string XMLParserPoco::getFirstElementAtPathInnerText(std::string path)
			{
				if(_documentLoaded)
				{
					Poco::XML::Node* node = _document->getNodeByPath(path);
					if(node != nullptr)
					{
						return node->innerText();
					}
				}
				return "";
			}

			bool XMLParserPoco::selectElementByNameAndNS(std::string name, std::string xmlNamespace)
			{
				if(_documentLoaded)
				{
					Poco::AutoPtr<Poco::XML::NodeList> nodeList = _document->getElementsByTagNameNS(xmlNamespace, name);

					if(nodeList.get()->length() > 0)
					{
						_selectedNode = nodeList.get()->item(0);
						return true;
					}
				}
				_selectedNode = nullptr;
				return false;
			}
			bool XMLParserPoco::selectChildByNameAndNS(std::string name, std::string xmlNamespace)
			{
				if(_documentLoaded && _selectedNode != nullptr)
				{
					Poco::AutoPtr<Poco::XML::NodeList> childNodeList = _selectedNode->childNodes();

					for(unsigned long i = 0; i < childNodeList->length(); i++)
					{
						if(childNodeList->item(i)->localName() == name && childNodeList->item(i)->namespaceURI() == xmlNamespace)
						{
							_selectedNode = childNodeList->item(i);
							return true;
						}
					}
				}
				return false;
			}
			bool XMLParserPoco::selectChildByName(std::string name)
			{
				if(_documentLoaded && _selectedNode != nullptr)
				{
					Poco::AutoPtr<Poco::XML::NodeList> childNodeList = _selectedNode->childNodes();

					for(unsigned long i = 0; i < childNodeList->length(); i++)
					{
						if(childNodeList->item(i)->localName() == name)
						{
							_selectedNode = childNodeList->item(i);
							return true;
						}
					}
				}
				return false;
			}
			bool XMLParserPoco::selectParent()
			{
				if(_documentLoaded && _selectedNode != nullptr)
				{
					_selectedNode = _selectedNode->parentNode();
					return true;
				}
				return false;
			}
			std::string XMLParserPoco::getInnerText()
			{
				if(_documentLoaded && _selectedNode != nullptr)
				{
					return _selectedNode->innerText();
				}
				return "";
			}
			std::string XMLParserPoco::getChildElementInnerText(std::string name)
			{
				if(_documentLoaded && _selectedNode != nullptr)
				{
					if(selectChildByName(name))
					{
						std::string innerText = getInnerText();
						selectParent();
						return innerText;
					}
				}
				return "";
			}
			std::vector<std::string> XMLParserPoco::getChildElementsList()
			{
				std::vector<std::string> childElements;
				if(_documentLoaded && _selectedNode != nullptr)
				{
					Poco::AutoPtr<Poco::XML::NodeList> childNodeList = _selectedNode->childNodes();

					for(unsigned long i = 0; i < childNodeList->length(); i++)
					{
						childElements.push_back(childNodeList->item(i)->localName());
					}
				}
				return childElements;
			}
		}
	}
}