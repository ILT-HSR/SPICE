/**
	SPICE Core
	MandatoryCommandBase.cpp
	Purpose: Abstract base class for implementation of mandatory commands

	@author Lukas Mueller (ilt.hsr.ch)
	@version 1.0 2015_10_21
*/

#include "MandatoryCommandBase.h"

namespace SPICE
{
	namespace Core
	{
		namespace Internal
		{
			MandatoryCommandBase::MandatoryCommandBase() : CommandBase(),
				_mandatoryCommandCallback(nullptr)
			{
			}
			MandatoryCommandBase::~MandatoryCommandBase()
			{
			}

			void MandatoryCommandBase::setMandatoryCommandCallback(std::shared_ptr<IMandatoryCommandCallback> mandatoryCommandCallback)
			{
				_mandatoryCommandCallback = mandatoryCommandCallback;
			}

			double MandatoryCommandBase::calculateEstimatedDuration()
			{
				return 0;
			}
            bool MandatoryCommandBase::isCommonCommand()
			{
                return true;
			}
			bool MandatoryCommandBase::readyForStart()
			{
				return true;
			}
			bool MandatoryCommandBase::simulationProcessing()
			{
				return processing();
			}
		}
	}
}