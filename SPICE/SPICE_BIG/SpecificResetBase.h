/**
	SPICE BIG
	SpecificResetBase.h
	Purpose: Abstract base class for implementation specific reset routine

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef SPECIFICRESETBASE_H
#define SPECIFICRESETBASE_H

#include <memory>

#include "ICommandCallback.h"

namespace SPICE
{
	namespace BIG
	{
		class SpecificResetBase
		{
				// Methoden
			public:
				/**
					Abstract base class for implementation specific reset routine
				*/
				SpecificResetBase();
				virtual ~SpecificResetBase();

				/**
					Sets the command callback interface which can be used by the specific routine to get access to core functions. DO NOT OVERRIDE (FINAL)
				*/
				void setCommandCallback(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				/**
					Executes the implementation specific actions for a command.

					@return True if command execution has been successful. False, if a not recoverable error occurred.
				*/
				virtual double calculateEstimatedDuration() = 0;
				/**
					Executes the implementation specific actions for a command.

					@param wasInSimulationMode True if device has been in simulation mode before the reset
					@param willBeInSimulationMode True if device will be in simulation mode after the reset
					@return True if command execution has been successful. False, if a not recoverable error occurred.
				*/
				virtual bool processing(bool wasInSimulationMode, bool willBeInSimulationMode) = 0;

			protected:
				std::shared_ptr<SPICE::BIG::ICommandCallback> _commandCallback;
			private :
		};
	}
}
#endif // SPECIFICRESETBASE_H
