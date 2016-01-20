/**
	SPI Multidrop
	PrepareForInput.h
	Purpose: Implementation of the prepare for input command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef PREPAREFORINPUT_H
#define PREPAREFORINPUT_H

#include "SpecificCommandBase.h"

namespace SPI
{
	namespace Multidrop
	{
		class PrepareForInput : public SpecificCommandBase
		{
			public:
				PrepareForInput(std::shared_ptr<SpecificCore> specificCore);
				virtual ~PrepareForInput();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual double calculateEstimatedDuration();

				virtual bool processing();

			protected:
			private:
		};
	}
}
#endif // PREPAREFORINPUT_H
