/**
	SPI Template
	CommandWithSpecificSimulation.h
	Purpose: This command shows the recommended implementation of a command with a specific simulation

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#ifndef COMMANDWITHSPECIFICSIMULATION_H
#define COMMANDWITHSPECIFICSIMULATION_H

#include "SpecificCommandBase.h"

namespace SPI
{
	namespace Template
	{
		class CommandWithSpecificSimulation : public SpecificCommandBase
		{
			public:
				CommandWithSpecificSimulation(std::shared_ptr<SpecificCore> specificCore);
				virtual ~CommandWithSpecificSimulation();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual bool isCommonCommand();

				virtual double calculateEstimatedDuration();

				virtual bool processing();
				virtual bool simulationProcessing();

			protected:
			private:
		};
	}
}
#endif // COMMANDWITHSPECIFICSIMULATION_H
