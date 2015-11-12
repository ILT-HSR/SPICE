/**
	SPI Multidrop
	PrepareForOutput.h
	Purpose: Implementation of the prepare for output command

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef PREPAREFOROUTPUT_H
#define PREPAREFOROUTPUT_H

#include "SpecificCommandBase.h"

namespace SPI
{
	namespace Multidrop
	{
		class PrepareForOutput : public SpecificCommandBase
		{
			public:
				PrepareForOutput(std::shared_ptr<SpecificCore> specificCore);
				virtual ~PrepareForOutput();

				virtual std::string getCommandName();
				virtual std::string getCommandDescription();
				virtual double calculateEstimatedDuration();

				virtual bool processing();

			protected:
			private:
		};
	}
}
#endif // PREPAREFOROUTPUT_H
