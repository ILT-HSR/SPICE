// SPITemplate.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
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
		// LINUX specific fix to enter the correct working directory
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
	// Create resource provider
	std::shared_ptr<SPI::Template::ResourceProvider> resourceProvider(new SPI::Template::ResourceProvider());

	// Create SpecificCoreContainer
	std::shared_ptr<SPI::Template::SpecificCoreContainer> specificCoreContainer(new SPI::Template::SpecificCoreContainer(resourceProvider));

	// Register SpecificCoreContainer at ResourceProvider
	resourceProvider->setSpecificCoreContainer(specificCoreContainer);

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

