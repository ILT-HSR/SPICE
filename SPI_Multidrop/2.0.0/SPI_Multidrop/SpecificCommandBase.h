/**
	SPI Multidrop
	SpecificCommandBase.h
	Purpose: Base for all the specific commands. Gives possibility to implement a general used function one time and be available at every command.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef SPECIFICCOMMANDBASE_H
#define SPECIFICCOMMANDBASE_H

#include <memory>

#include "CommandBase.h"
#include "SpecificCore.h"

namespace SPI
{
	namespace Multidrop
	{
		class SpecificCommandBase : public SPICE::BIG::CommandBase
		{
			public:
				SpecificCommandBase(std::shared_ptr<SpecificCore> specificCore);
				virtual ~SpecificCommandBase();

				virtual bool isCommonCommand();

			protected:
				std::shared_ptr<SpecificCore> _specificCore;
			private:
		};
	}
}
#endif // SPECIFICCOMMANDBASE_H
