/**
	SPICE Core
	Abort.h
	Purpose: Provides the functionality of the abort-command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef ABORT_H
#define ABORT_H

#include "MandatoryCommandBase.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			class Abort : public MandatoryCommandBase
			{
				public:
					Abort();
					virtual ~Abort();

					virtual std::string getCommandName();
					virtual std::string getCommandDescription();

					virtual void additionalCommandCheck(int& returnCode, std::string& message);

					virtual bool processing();

				protected:
				private:
			};
		}
	}
}
#endif // ABORT_H
