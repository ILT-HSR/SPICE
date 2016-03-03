/**
	SPI Template
	CommandWithParallelExecution.h
	Purpose: This command shows the recommended implementation of a command with the possibility of parallel execution.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_21
*/

#ifndef COMMANDWITHPARALLELEXECUTION_H
#define COMMANDWITHPARALLELEXECUTION_H

#include "SpecificCommandBase.h"

namespace SPI
{
	namespace Template
	{
		class CommandWithParallelExecution : public SpecificCommandBase
		{
			public:
				CommandWithParallelExecution(std::shared_ptr<SpecificCore> specificCore);
				virtual ~CommandWithParallelExecution();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual bool isCommonCommand();

				virtual double calculateEstimatedDuration();

				virtual bool readyForStart();
				virtual bool processing();

			protected:
			private:
		};
	}
}
#endif // COMMANDWITHPARALLELEXECUTION_H
