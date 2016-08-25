// SPITemplate.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#ifdef __linux__
#include <unistd.h>
#else
#include "stdafx.h"
#define POCO_NO_UNWINDOWS
#endif // LINUX

#include <iostream>

#include "SPICECore.h"
#include "EthernetServer.h"
#include "ResourceProvider.h"

// Flag used to force cleanup of connected device on exit.
bool cleanup = false;

// Setup the port we should listen on... default to 8080.
int port = 8080;

#ifdef __linux__
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

BOOL ctrl_handler(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT) {
		cleanup = true;
		while (cleanup)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		return TRUE;
	}
	return FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// Hook shutdown event
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);

	// Grab command line arguments, we only use them to set the TCP Port
	// First arg is the path, second should be the port.
	if (argc == 2)
	{
		// Only override the default if the value of 
		// the second command line arg evaluates to something other than zero
		int var = _ttoi(argv[1]);
		if (var > 0)
		{
			port = var;
		}
	}

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
	if(ethernetServer->startServer(port) != 0)
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
		// Check to see if we are exiting...
		if (cleanup)
		{
			// Perform any cleanup/disconnect here...
			// ...
			// ...

			// Clear the cleanup flag to continue
			cleanup = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return 0;
}

