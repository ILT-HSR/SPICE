// SPIMultidrop.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#ifdef LINUX
#include <unistd.h>
#else
#include "stdafx.h"
#define POCO_NO_UNWINDOWS
#endif // LINUX

#include <iostream>

#include "SPICECore.h"
#include "EthernetServer.h"
#include "ResourceProvider.h"

#ifdef LINUX
int main()
{
    if(true)
    {
        char buff[255];
        ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff)-1);
        if(len != -1)
        {
            buff[len] = '\0';
            std::string exePath = std::string(buff);
            exePath = exePath.erase(exePath.find_last_of("/")) + "/";

            std::cout << exePath << std::endl;
            chdir(exePath.c_str());
        }
    }
#else
int _tmain()
{
#endif
	// SpecificCoreContainer
	std::cout << "Search for connected Multidrop... " << std::flush;
	std::shared_ptr<SPI::Multidrop::SpecificCoreContainer> specificCoreContainer(new SPI::Multidrop::SpecificCoreContainer());
	for(int i = 0; i < 3; i++)
	{
		if(specificCoreContainer->getSpecificCore()->startupFindDevice())
		{
			i = 3;
			std::cout << "found" << std::endl;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			if(i == 2)
			{
				std::cout << "not found" << std::endl;
			}
		}
	}

	// Create resource provider
	std::shared_ptr<SPI::Multidrop::ResourceProvider> resourceProvider(new SPI::Multidrop::ResourceProvider(specificCoreContainer));

	// Create and start an EthernetServer
	std::cout << "Starting EthernetServer... " << std::flush;
	std::shared_ptr<SPICE::ES::POCO::EthernetServer> ethernetServer(new SPICE::ES::POCO::EthernetServer());
	if(ethernetServer->startServer() != 0)
	{
		std::cout << "fail" << std::endl;
		std::cerr << "EthernetServer could not be started successfully" << std::endl;
		ethernetServer->stopServer();
		std::cout << "Application has to stop (10s)" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		return 1;
	}
	else
	{
		std::cout << "Done" << std::endl;
	}

	// Create core and add it at an ethernet server
	std::shared_ptr<SPICE::Core::SPICECore> core(new SPICE::Core::SPICECore(resourceProvider));
	core->registerAtEthernetServer(ethernetServer);

	std::cout << std::endl;

	while(true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return 0;
}

