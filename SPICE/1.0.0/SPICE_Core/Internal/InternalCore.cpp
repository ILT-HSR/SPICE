/**
	SPICE Core
	InternalCore.cpp
	Purpose: Class internal as container for the most important instances of the core

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "InternalCore.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "GeneralFunctions.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			InternalCore::InternalCore(std::shared_ptr<SPICE::BIG::IResourceProvider> resourceProvider) :
				_commandHandler(std::shared_ptr<CommandHandler>(new CommandHandler())),
				_connectionHandler(std::shared_ptr<ConnectionHandler>(new ConnectionHandler(_commandHandler)))
			{
				_commandHandler->getCoreData()->setResourceProvider(resourceProvider);
				_connectionHandler->prepareWSDLFile();
				_commandHandler->startCommandHandler();

				std::string filenameSN = "SerialNumber.txt";
#ifdef SILACONVERTER_BUILD
				filenameSN = "../SerialNumber/" + filenameSN;
#endif
				std::string serialNumberStr = "";

				// Read SerialNumber-File
				{
					std::ifstream readStream;
					try
					{
						readStream.open(filenameSN, std::ios::in);
					
						if(readStream.is_open() && !readStream.eof())
						{
							std::string line;
							std::getline(readStream, line);
							serialNumberStr = line;
						}
						readStream.close();
					}
					catch (std::exception e)
					{
						readStream.close();
					}
				}

				if(serialNumberStr == "")
				{
					std::cerr << "No serial number found!" << std::endl;
					throw std::runtime_error("No serial number found");
				}
				else
				{
					if(resourceProvider->getCoreConfigurationParameter("SERIAL_NUMBER_EXTENSION") == "")
					{
						std::cout << "Serial number: " << serialNumberStr << std::endl;
					}
					else
					{
						serialNumberStr += resourceProvider->getCoreConfigurationParameter("SERIAL_NUMBER_EXTENSION");
						std::cout << "Serial number (with extension): " << serialNumberStr << std::endl;
					}
				}
				unsigned long serialNumber = std::stoul(serialNumberStr);

				if(resourceProvider->getCoreConfigurationParameter("USE_WSDISCOVERY") == "true")
				{
					std::cout << "WSDiscovery: enabled" << std::endl;
					std::string filenameConfig = resourceProvider->getCoreConfigurationParameter("URI_PATHNAME") + ".wsdiscovery";
					std::string uuid = "";
					unsigned long randomNumber = 0;
					std::ifstream readStream;
					try
					{
						readStream.open(filenameConfig, std::ios::in);
					
						while(readStream.is_open() && !readStream.eof())
						{
							std::string line;
							std::getline(readStream, line);
							if(line.length() >= 3)
							{
								std::string value = line.substr(0,3);
								if(value == "ID:")
								{
									uuid = line.substr(3);
								}
								else if(value == "RN:")
								{
									randomNumber = std::stoul(line.substr(3));
								}
							}
						}
						readStream.close();
					}
					catch (std::exception e)
					{
						readStream.close();
					}

					if(randomNumber == 0)
					{
						randomNumber = serialNumber;
					}
					randomNumber = WSDiscoveryCore::seedGenerator(randomNumber);

					if(uuid == "")
					{
						uuid = WSDiscoveryCore::generateRandomUUID();
					}
					WSDiscoveryCore::setDeviceUUID(uuid);

					std::ofstream writeFile;
					writeFile.open(filenameConfig, std::ios::trunc);
					writeFile << "ID:" << uuid << std::endl;
					writeFile << "RN:" << randomNumber;
					writeFile.flush();
					writeFile.close();
					SPICE::BIG::GeneralFunctions::wroteFileToDisk();
				}
				else
				{
					std::cout << "WSDiscovery: disabled" << std::endl;
				}

				_commandHandler->getCoreData()->setSerialNumber(serialNumber);
			}
			InternalCore::~InternalCore()
			{
			}

			void InternalCore::registerAtEthernetServer(std::shared_ptr<SPICE::BIG::IEthernetServer> ethernetServer)
			{
				std::string uriPathName = "/" + _commandHandler->getCoreData()->getResourceProvider()->getCoreConfigurationParameter("URI_PATHNAME");
				if(_commandHandler->getCoreData()->getResourceProvider()->getCoreConfigurationParameter("USE_WSDISCOVERY") == "true")
				{
					std::shared_ptr<WSDiscoveryCore> newWSDiscoveryCore(new WSDiscoveryCore(_commandHandler->getCoreData()));
					_wsDiscoveryCoreList.push_back(newWSDiscoveryCore);
					ethernetServer->registerEthernetConnectionCallback(uriPathName, _connectionHandler, newWSDiscoveryCore);
				}
				else
				{
					ethernetServer->registerEthernetConnectionCallback(uriPathName, _connectionHandler);
				}
			}
			void InternalCore::unregisterFromEthernetServer(std::shared_ptr<SPICE::BIG::IEthernetServer> ethernetServer)
			{
				std::string uriPathName = "/" + _commandHandler->getCoreData()->getResourceProvider()->getCoreConfigurationParameter("URI_PATHNAME");

				ethernetServer->unregisterEthernetConnectionCallback(uriPathName);
			}
		}
	}
}