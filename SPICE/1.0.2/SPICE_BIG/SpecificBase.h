/**
	SPICE BIG
	SpecificBase.h
	Purpose: Abstract base class for implementation of specific init / setParameters routine

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef SPECIFICBASE_H
#define SPECIFICBASE_H

#include <memory>

#include "ICommandCallback.h"

namespace SPICE
{
	namespace BIG
	{
		class SpecificBase
		{
				// Methoden
			public:
				/**
					Abstract base class for implementation of specific init /setParameters routine
				*/
				SpecificBase();
				virtual ~SpecificBase();

				/**
					Sets the command callback interface which can be used by the specific routine to get access to core functions. DO NOT OVERRIDE (FINAL)
				*/
				void setCommandCallback(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				/**
					Gets the estimated duration of the command

					@return estimated duration in seconds
				*/
				virtual double calculateEstimatedDuration() = 0;
				/**
					Executes the implementation specific actions for a command.

					@return True if command execution has been successful. False, if a not recoverable error occurred.
				*/
				virtual bool processing() = 0;
				/**
					Executes a simulation processing if service provider is in simulation mode. Default is: Wait over the estimated time.
					Can be implemented if an other behaviour is requested (example: give some simulation commands to the specific device core)

					@return True if command execution has been successful. False, if a not recoverable error occurred.
				*/
				bool simulationProcessing();
			protected:
				std::shared_ptr<SPICE::BIG::ICommandCallback> _commandCallback;
			private :
	    };
	}
}
#endif // SPECIFICBASE_H
