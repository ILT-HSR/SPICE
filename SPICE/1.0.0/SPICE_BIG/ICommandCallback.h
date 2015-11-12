/**
	SPICE BIG
	ICommandCallback.h
	Purpose: Interface for a core internal command. Provides access for a implementation specific command to the needed core functions.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#ifndef ICOMMANDCALLBACK_H
#define ICOMMANDCALLBACK_H

#include <string>
#include <map>
#include <memory>

#include "CommandError.h"
#include "DataSet.h"
#include "IXMLParser.h"

namespace SPICE
{
	namespace BIG
	{
		class ICommandCallback
		{
			public:
				/**
					For polling whether the command has to pause

					@return True when it has to enter the pause
				*/
				virtual bool hasToPause() = 0;
				/**
					To call after command is ready to enter pause. StateMachine gets the information that the pause state is reached. Returns if pause-state is left or reset is called.
				*/
				virtual void enterPause() = 0;
				/**
					For polling whether the command has to abort. 

					@return True when it has to abort.
				*/
				virtual bool hasToAbort() = 0;
				/**
					For polling whether the command has to reset. 

					@return True when it has to reset.
				*/
				virtual bool hasToReset() = 0;
				/**
					For polling whether the command has to abort or reset. 

					@return True when it has to abort or reset.
				*/
				virtual bool hasToAbortOrReset() = 0;

				/**
					To throw a generated error and maybe trigger the sending of an error event. 

					@param commandError Generated error which is to throw
					@return The selected continuation task name
				*/
				virtual std::string throwError(std::shared_ptr<CommandError> commandError) = 0;

				/**
					Gets the count of waiting commands.

					@return count of waiting commands
				*/
				virtual unsigned int getWaitingCommandsCount() = 0;
				/**
					Gets the count of active commands (currently executing)

					@return count of active commands
				*/
				virtual unsigned int getActiveCommandsCount() = 0;
				/**
					Gets a map with the requestIds and commandNames of all active commands.

					@return Map with requestIds and commandNames of all active commands.
				*/
				virtual std::map<int, std::string> getActiveCommandsList() = 0;

				/**
					Provides the possibility to set content to the responseData in the responseEvent.

					@param responseEventData xml-formated string with content for the responseData
				*/
				virtual void setResponseEventData(std::string responseEventData) = 0;

				/**
					Gets the parameter of the state machine as DataSet.

					@return The state machine parameter as DataSet
				*/
				virtual std::shared_ptr<DataSet> getParameterSet() = 0;
				/**
					Gets the configuration of a configuration level as DataSet.

					@param configLevel the configuration level which is requested
					@param password Password to get access to the configLevel. Default is an empty string.
					@return The requested configuration as DataSet. Returns null if configLevel not exists or a wrong password is given.
				*/
				virtual std::shared_ptr<DataSet> getConfigurationSet(int configLevel, std::string password = "") = 0;
				/**
					Is called when a configurationSet has been changed. Possible safe-actions can be executed.

					@param configLevel The configuration level of the changed configurationSet
				*/
				virtual void configurationSetHasChanged(int configLevel) = 0;

				/**
					Gets a new instance of the xml parser

					@return XMLParser
				*/
				virtual std::shared_ptr<SPICE::BIG::IXMLParser> getNewXMLParserInstance() = 0;

		};
	}
}
#endif // ICOMMANDCALLBACK_H
