/**
	SPI Template
	CommandMinimum.h
	Purpose: This command shows the minimum required implementation of a command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2016_01_20
*/

#ifndef COMMANDMINIMUM_H
#define COMMANDMINIMUM_H

#include "SpecificCommandBase.h"

namespace SPI
{
	namespace Template
	{
		class CommandMinimum : public SpecificCommandBase
		{
			public:
				CommandMinimum(std::shared_ptr<SpecificCore> specificCore);
				virtual ~CommandMinimum();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual bool isCommonCommand();

				virtual double calculateEstimatedDuration();

				virtual bool processing();

			protected:
			private:
		};
	}
}
#endif // COMMANDMINIMUM_H
