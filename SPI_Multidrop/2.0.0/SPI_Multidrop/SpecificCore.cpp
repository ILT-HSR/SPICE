/**
	SPI Multidrop
	SpecificCore.cpp
	Purpose: Central class of a specific implemention. Usually used to communicate with the device. An instance is just deleted and new generated with the reset command. Every command can use it.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#include "SpecificCore.h"

#include <iostream>

namespace SPI
{
	namespace Multidrop
	{
		SpecificCore::SpecificCore(bool simulationMode) :
			_deviceType(DeviceType::MultidropCombi),
			_settedCassetteType(0),
			_settedPlateTypeCombi(0),
			_settedSpeed(100),
			_settedPlateTypeMicroDW384(0),
			_simulationMode(simulationMode),
			_isConnected(false),
			_isConnecting(false),
			_isClosing(false),
			_sendingStates(SendingStates::SENDING_NOT_ACTIVE),
			_connectionError(false),
			_multidropError(false),
			_connectionErrorMessage(""),
			_multidropErrorNumber(-1),
			_sendingData(""),
			_receivedLinesCount(0),
			_nextReceivingLine(""),
			_multidropVersionInformation("-"),
			_multidropFirmwareVersion("-"),
			_multidropSerialNo("-"),
			_io_service(nullptr),
			_serial_port(nullptr),
			_portOffset(0u)
		{
			loadLastMultidropVersion();
		}
		SpecificCore::~SpecificCore()
		{
			closeConnection();
		}


		bool SpecificCore::startupFindDevice()
		{
			if(_simulationMode)
			{
				return true;
			}
			unsigned int portOffset = 0u;
			while(true)
			{
				int openedPort = openConnection(portOffset);
				if(openedPort == -1)
				{
					return false;
				}
				else
				{
					if(getMultidropVersion())
					{
						if(_deviceType == DeviceType::MultidropCombi || _deviceType == DeviceType::MultidropCombiNL)
						{
							modernCommandCommunication("QIT", "", 15);
						}
						closeConnection();
						_portOffset = (unsigned int)(openedPort);
						return true;
					}
					else
					{
						closeConnection();
						portOffset = (unsigned int)(openedPort + 1);
					}
				}
			}
		}
		bool SpecificCore::openConnection()
		{
			if(_simulationMode)
			{
				return true;
			}
			return openConnection(_portOffset) != -1;
		}
		bool SpecificCore::closeConnection()
		{
			if(_simulationMode)
			{
				return true;
			}
			if(!_isConnected || _isClosing)
			{
				return false;
			}
			_isClosing = true;

			_serial_port->close();
			while(_serial_port->is_open())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			delete _serial_port;
			_serial_port = nullptr;

			_io_service_keep_running = false;
			if(_io_service != nullptr)
			{
				_io_service->stop();
				while(!_io_service->stopped())
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
				if(_io_service_thread != nullptr)
				{
					if(_io_service_thread->joinable())
					{
						_io_service_thread->join();
					}
					delete _io_service_thread;
					_io_service_thread = nullptr;
				}
				delete _io_service;
				_io_service = nullptr;
			}

			_isConnected = false;
			_isClosing = false;
			return true;
		}

		bool SpecificCore::isReadyToReset()
		{
			if(_simulationMode)
			{
				return true;
			}
			return (_io_service_thread == nullptr && _serial_port == nullptr && _io_service == nullptr);
		}

		SpecificCore::DeviceType SpecificCore::getDeviceType()
		{
			return _deviceType;
		}
		std::string SpecificCore::getMultidropDeviceTypeName()
		{
			return _multidropDeviceTypeName;
		}
		std::string SpecificCore::getMultidropFirmwareVersion()
		{
			return _multidropFirmwareVersion;
		}
		std::string SpecificCore::getMultidropSerialNo()
		{
			return _multidropSerialNo;
		}

		int SpecificCore::calculateTimeDispense(unsigned int volume)
		{
			if(_deviceType == DeviceType::MultidropCombi)
			{
				double cassetteFactor = 12;
				if(_settedPlateTypeCombi >= 3)
				{
					cassetteFactor *= 4;
				}
				if(_settedPlateTypeCombi >= 8)
				{
					cassetteFactor *= 4;
				}
				if(_settedCassetteType == 0)
				{
					double duration = _settedSpeed * 0.01 * 14;
					duration = (67 - duration) * volume * cassetteFactor / 10000;
					return ((int)(duration)) + 10;
				}
				else
				{
					double duration = _settedSpeed * 0.01 * 98;
					duration = (266 - duration) * volume * cassetteFactor / 10000;
					return ((int)(duration)) + 10;
				}
			}
			else if(_deviceType == DeviceType::MultidropMicro)
			{
				if(_settedPlateTypeMicroDW384 == 0)
				{
					double duration = 5.0 + (10.0 * volume / 50);
					return ((int)(duration)) + 10;
				}
				else
				{
					double duration = 15.0 + (40.0 * volume / 50);
					return ((int)(duration)) + 10;
				}
			}
			else if(_deviceType == DeviceType::Multidrop384)
			{
				if(_settedPlateTypeMicroDW384 == 0)
				{
					double duration = 5.0 + (75.0 * volume / 1000);
					return ((int)(duration)) + 10;
				}
				else
				{
					double duration = 15.0 + (50.0 * volume / 140);
					return ((int)(duration)) + 10;
				}
			}
			else
			{
				return 1800;
			}
		}
		int SpecificCore::calculateTimeDispenseByColumn(unsigned int volume, unsigned int startColumn, unsigned int stopColumn)
		{
			if(_deviceType == DeviceType::MultidropCombi)
			{
				unsigned int columnCount = stopColumn - startColumn + 1;
				double cassetteFactor = 0;
				if(_settedPlateTypeCombi >= 0 && _settedPlateTypeCombi < 3)
				{
					if(columnCount > 12)
					{
						columnCount = 12;
					}
					cassetteFactor = columnCount;
				}
				if(_settedPlateTypeCombi >= 3 && _settedPlateTypeCombi < 8)
				{
					if(columnCount > 24)
					{
						columnCount = 24;
					}
					cassetteFactor = columnCount * 2;
				}
				if(_settedPlateTypeCombi >= 8)
				{
					cassetteFactor = columnCount * 4;
				}
				if(_settedCassetteType == 0)
				{
					double duration = _settedSpeed * 0.01 * 14;
					duration = (67 - duration) * volume * cassetteFactor / 10000;
					return ((int)(duration)) + 10;
				}
				else
				{
					double duration = _settedSpeed * 0.01 * 98;
					duration = (266 - duration) * volume * cassetteFactor / 10000;
					return ((int)(duration)) + 10;
				}
			}
			else if(_deviceType == DeviceType::MultidropMicro)
			{
				unsigned int columnCount = stopColumn - startColumn + 1;
				if(_settedPlateTypeMicroDW384 == 0)
				{
					double duration = (5.0 + (10.0 * volume / 50)) * columnCount / 12;
					return ((int)(duration)) + 10;
				}
				else
				{
					double duration = (15.0 + (40.0 * volume / 50)) * columnCount / 24;
					return ((int)(duration)) + 10;
				}
			}
			else if(_deviceType == DeviceType::Multidrop384)
			{
				unsigned int columnCount = stopColumn - startColumn + 1;
				if(_settedPlateTypeMicroDW384 == 0)
				{
					double duration = (5.0 + (75.0 * volume / 1000)) * columnCount / 12;
					return ((int)(duration)) + 10;
				}
				else
				{
					double duration = (15.0 + (50.0 * volume / 140)) * columnCount / 24;
					return ((int)(duration)) + 10;
				}
			}
			else
			{
				return 1800;
			}
		}
		int SpecificCore::calculateTimeEmptyTubesAndPrime(unsigned long volume)
		{
			if(_deviceType == DeviceType::MultidropCombi)
			{
				if(_settedCassetteType == 0)
				{
					double duration = _settedSpeed * 0.01 * 14;
					duration = (67 - duration) * volume / 10000;
					return ((int)(duration)) + 10;
				}
				else
				{
					double duration = _settedSpeed * 0.01 * 98;
					duration = (266 - duration) * volume / 10000;
					return ((int)(duration)) + 10;
				}
			}
			else if(_deviceType == DeviceType::MultidropMicro)
			{
				double duration = 20.0 * volume / 999;
				return ((int)(duration)) + 10;
			}
			else if(_deviceType == DeviceType::Multidrop384)
			{
				return 15;
			}
			else
			{
				return 1800;
			}
		}


		bool SpecificCore::init(unsigned int plateType, bool usePlateTypeConfig, unsigned int predispenseVolume, unsigned int dispensingHeight, unsigned int dispensingOrder, int dispenseOffsetX, int dispenseOffsetY, unsigned int cassetteType, unsigned int pumpSpeed, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::MultidropCombi)
			{
				if(!validateMultidropVersion(commandCallback))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!setParameters(plateType, usePlateTypeConfig, predispenseVolume, dispensingHeight, dispensingOrder, dispenseOffsetX, dispenseOffsetY, cassetteType, pumpSpeed, commandCallback))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::init(unsigned int plateType, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW || _deviceType == DeviceType::MultidropMicro)
			{
				if(!validateMultidropVersion(commandCallback))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!oldCommandExecution("O", commandCallback, 20))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!setParameters(plateType, commandCallback))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::setParameters(unsigned int plateType, bool usePlateTypeConfig, unsigned int predispenseVolume, unsigned int dispensingHeight, unsigned int dispensingOrder, int dispenseOffsetX, int dispenseOffsetY, unsigned int cassetteType, unsigned int pumpSpeed, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::MultidropCombi)
			{
				if(usePlateTypeConfig)
				{
					predispenseVolume = 20 - (cassetteType * 18);
					dispensingOrder = 0;
					dispenseOffsetX = 0;
					dispenseOffsetY = 0;

					switch(plateType)
					{
					case 0 :
					case 5 :
						dispensingHeight = 1600;
						break;
					case 1 :
					case 6 :
						dispensingHeight = 2300;
						break;
					case 2 :
					case 7 :
						dispensingHeight = 4500;
						break;
					case 3 :
						dispensingHeight = 850;
						break;
					case 4 :
						dispensingHeight = 1100;
						break;
					case 8 :
						dispensingHeight = 600;
						break;
					case 9 :
						dispensingHeight = 1150;
						break;
					default :
						dispensingHeight = 1600;
						break;
					}
				}
				if(!modernCommandExecution("SCT", std::to_string(cassetteType), commandCallback, 5))
				{
					return false;
				}
				_settedCassetteType = cassetteType;
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SPL", std::to_string(plateType), commandCallback, 5))
				{
					return false;
				}
				_settedPlateTypeCombi = plateType;
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SPS", std::to_string(pumpSpeed), commandCallback, 5))
				{
					return false;
				}
				_settedSpeed = pumpSpeed;
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SPV", std::to_string(predispenseVolume * 10), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SDO", std::to_string(dispensingOrder), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SOF", std::to_string(dispenseOffsetX) + " " + std::to_string(dispenseOffsetY), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SDH", std::to_string(dispensingHeight), commandCallback, 5))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::setParameters(unsigned int plateType, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW || _deviceType == DeviceType::MultidropMicro)
			{
				if(!oldCommandExecution("T" + std::to_string(plateType), commandCallback, 5))
				{
					return false;
				}
				_settedPlateTypeMicroDW384 = plateType;
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::reset(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_simulationMode)
			{
				return true;
			}
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW || _deviceType == DeviceType::MultidropMicro)
			{
				if(_isConnected && !_connectionError)
				{
					if(!oldCommandExecution("O", commandCallback, 20))
					{
						return false;
					}
				}
				return true;
			}
			else if(_deviceType == DeviceType::MultidropCombi)
			{
				if(_isConnected && !_connectionError)
				{
					if(!modernCommandExecution("QIT", "", commandCallback, 20))
					{
						return false;
					}
				}
			}
			else if(_deviceType == DeviceType::MultidropCombiNL)
			{
				if(_isConnected && !_connectionError)
				{
					if(!modernCommandExecution("POF", "", commandCallback, 20))
					{
						return false;
					}
					if(!checkForPauseAndReturn(commandCallback))
					{
						return true;
					}
					if(!modernCommandExecution("QIT", "", commandCallback, 20))
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::dispense(unsigned int volume, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW || _deviceType == DeviceType::MultidropMicro)
			{
				if(!oldCommandExecution("V" + std::to_string(volume), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!oldCommandExecution("D", commandCallback, calculateTimeDispense(volume)))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombi)
			{
				if(!modernCommandExecution("SCV", "0 " + std::to_string(volume * 10), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("DIS", "", commandCallback, calculateTimeDispense(volume)))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombiNL)
			{
				if(!modernCommandExecution("PON", "1", commandCallback, 60))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SAV", "0 0 0 0 " + std::to_string(volume), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("DIS", "", commandCallback, calculateTimeDispense(volume)))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("POF", "", commandCallback, 20))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::dispenseByColumn(unsigned int volume, unsigned int startColumn, unsigned int stopColumn, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW || _deviceType == DeviceType::MultidropMicro)
			{
				if(!oldCommandExecution("S" + std::to_string(startColumn), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!oldCommandExecution("V" + std::to_string(volume), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!oldCommandExecution("M" + std::to_string(stopColumn - startColumn + 1), commandCallback, calculateTimeDispenseByColumn(volume, startColumn, stopColumn)))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!oldCommandExecution("O", commandCallback, 20))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombi)
			{
				if(!modernCommandExecution("SCV", "0 0", commandCallback, 5))
				{
					return false;
				}
				for(unsigned int i = startColumn; i <= stopColumn; i++)
				{
					if(!checkForPauseAndReturn(commandCallback))
					{
						return true;
					}
					if(!modernCommandExecution("SCV", std::to_string(i) + " " + std::to_string(volume * 10), commandCallback, 5))
					{
						return false;
					}
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("DIS", "", commandCallback, calculateTimeDispenseByColumn(volume, startColumn, stopColumn)))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombiNL)
			{
				if(!modernCommandExecution("PON", "1", commandCallback, 60))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SAV", "0 0 0 0 0", commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("SAV", std::to_string(startColumn) + " 0 " + std::to_string(stopColumn) + " 0 " + std::to_string(volume), commandCallback, 5))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("DIS", "", commandCallback, calculateTimeDispenseByColumn(volume, startColumn, stopColumn)))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("POF", "", commandCallback, 10))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::emptyTubesVolume(unsigned long volume, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::MultidropMicro)
			{
				if(!oldCommandExecution("E" + std::to_string(volume), commandCallback, calculateTimeEmptyTubesAndPrime(volume)))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombi)
			{
				if(!modernCommandExecution("EMP", std::to_string(volume * 10), commandCallback, calculateTimeEmptyTubesAndPrime(volume)))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::emptyTubesTime(unsigned long time, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::MultidropCombiNL)
			{
				if(!modernCommandExecution("EMP", std::to_string(time * 100), commandCallback, time + 10))
				{
					return false;
				}
				return true;
			}
			return false;
		}
		bool SpecificCore::emptyTubes(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW)
			{
				if(!oldCommandExecution("E", commandCallback, 60))
				{
					return false;
				}
				return true;
			}
			return false;
		}
		bool SpecificCore::prime(unsigned long volume, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW || _deviceType == DeviceType::MultidropMicro)
			{
				if(!oldCommandExecution("P" + std::to_string(volume), commandCallback, calculateTimeEmptyTubesAndPrime(volume)))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombi)
			{
				if(!modernCommandExecution("PRI", std::to_string(volume * 10), commandCallback, calculateTimeEmptyTubesAndPrime(volume)))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombiNL)
			{
				if(!modernCommandExecution("PON", "1", commandCallback, 60))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("PRI", std::to_string(volume), commandCallback, calculateTimeEmptyTubesAndPrime(volume)))
				{
					return false;
				}
				if(!checkForPauseAndReturn(commandCallback))
				{
					return true;
				}
				if(!modernCommandExecution("POF", "", commandCallback, 10))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::shake(unsigned int time, unsigned int distance, unsigned int speed, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::MultidropCombi || _deviceType == DeviceType::MultidropCombiNL)
			{
				if(!modernCommandExecution("SHA", std::to_string(time * 100) + " " + std::to_string(distance) + " " + std::to_string(speed), commandCallback, time + 10))
				{
					return false;
				}
				return true;
			}
			return false;
		}
		bool SpecificCore::shake(unsigned int time, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropMicro)
			{
				if(!oldCommandExecution("Z" + std::to_string(time), commandCallback, time + 10))
				{
					return false;
				}
				return true;
			}
			return false;
		}
		bool SpecificCore::prepareForInOutput(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_deviceType == DeviceType::Multidrop384 || _deviceType == DeviceType::MultidropDW || _deviceType == DeviceType::MultidropMicro)
			{
				if(!oldCommandExecution("O", commandCallback, 20))
				{
					return false;
				}
			}
			else if(_deviceType == DeviceType::MultidropCombi || _deviceType == DeviceType::MultidropCombiNL)
			{
				if(!modernCommandExecution("POU", "", commandCallback, 20))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
			return true;
		}
		bool SpecificCore::checkForPauseAndReturn(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(commandCallback->hasToAbortOrReset())
			{
				return false;
			}
			else if(commandCallback->hasToPause())
			{
				commandCallback->enterPause();
			}
			return true;
		}

		void SpecificCore::loadLastMultidropVersion()
		{
			std::ifstream readStream;
			try
			{
				readStream.open("multidrop.version");
				std::string versionInformation = "";
				std::string deviceType = "";
				std::string firmware = "";
				std::string serialNo = "";

				if(readStream.eof())
				{
					return;
				}
				std::getline(readStream, versionInformation);
				if(readStream.eof())
				{
					return;
				}
				std::getline(readStream, deviceType);
				if(readStream.eof())
				{
					return;
				}
				std::getline(readStream, firmware);
				if(readStream.eof())
				{
					return;
				}
				std::getline(readStream, serialNo);

				readStream.close();
				if(deviceType == "MultidropCombi")
				{
					_deviceType = DeviceType::MultidropCombi;
				}
				else if(deviceType == "MultidropCombiNL")
				{
					_deviceType = DeviceType::MultidropCombiNL;
				}
				else if(deviceType == "Multidrop384")
				{
					_deviceType = DeviceType::Multidrop384;
				}
				else if(deviceType == "MultidropDW")
				{
					_deviceType = DeviceType::MultidropDW;
				}
				else if(deviceType == "MultidropMicro")
				{
					_deviceType = DeviceType::MultidropMicro;
				}
				else
				{
					return;
				}
				_multidropDeviceTypeName = deviceType;
				_multidropVersionInformation = versionInformation;
				_multidropFirmwareVersion = firmware;
				_multidropSerialNo = serialNo;

			}
			catch (std::exception e)
			{
				readStream.close();
			}
		}
		void SpecificCore::setNewMultidropVersion(DeviceType newDeviceType, std::string multidropVersionInformation)
		{
			std::string firmware = "";
			std::string serialNo = "";
			std::string deviceType = "";

			switch(newDeviceType)
			{
			case DeviceType::MultidropCombi :
				firmware = multidropVersionInformation.substr(15);
				serialNo = firmware.substr(firmware.find_first_of(" ") + 1);
				firmware = firmware.substr(0, firmware.find_first_of(" "));
				deviceType = "MultidropCombi";
				break;
			case DeviceType::MultidropCombiNL :
				firmware = multidropVersionInformation.substr(9);
				serialNo = firmware.substr(firmware.find_first_of(" ") + 1);
				firmware = firmware.substr(0, firmware.find_first_of(" "));
				deviceType = "MultidropCombiNL";
				break;
			case DeviceType::Multidrop384 :
				firmware = multidropVersionInformation.substr(9);
				serialNo = firmware;
				deviceType = "Multidrop384";
				break;
			case DeviceType::MultidropDW :
				firmware = multidropVersionInformation.substr(8);
				serialNo = firmware;
				deviceType = "MultidropDW";
				break;
			case DeviceType::MultidropMicro :
				firmware = multidropVersionInformation.substr(8);
				serialNo = firmware;
				deviceType = "MultidropMicro";
				break;
			default :
				return;
				break;
			}

			if(multidropVersionInformation != _multidropVersionInformation)
			{
				std::ofstream writeFile;
				try
				{
					writeFile.open("multidrop.version", std::ios::trunc);
					writeFile << multidropVersionInformation << std::endl;
					writeFile << deviceType << std::endl;
					writeFile << firmware << std::endl;
					writeFile << serialNo << std::endl;
					writeFile.flush();
					writeFile.close();
				}
				catch (std::exception e)
				{
					writeFile.close();
				}
				SPICE::BIG::GeneralFunctions::wroteFileToDisk();
			}
			_multidropVersionInformation = multidropVersionInformation;
			_multidropFirmwareVersion = firmware;
			_multidropSerialNo = serialNo;
			_multidropDeviceTypeName = deviceType;
			_deviceType = newDeviceType;
		}

		bool SpecificCore::getMultidropVersion()
		{
			bool found = false;
			DeviceType deviceType = DeviceType::MultidropCombi;
			std::string multidropVersionInformation = "";

			if(!readMultidropVersion(found, deviceType, multidropVersionInformation))
			{
				return false;
			}
			if(!found)
			{
				return false;
			}

			setNewMultidropVersion(deviceType, multidropVersionInformation);
			return true;
		}
		bool SpecificCore::validateMultidropVersion(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			bool found = false;
			DeviceType deviceType = DeviceType::MultidropCombi;
			std::string multidropVersionInformation = "";

			if(!readMultidropVersion(found, deviceType, multidropVersionInformation))
			{
				std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("RS232-Connection-Error: " + _connectionErrorMessage, "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
				commandCallback->throwError(error);
				return false;
			}

			if(!found)
			{
				std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Version-Error: Could not find version.", "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
				commandCallback->throwError(error);
				return false;
			}
			if(deviceType != _deviceType)
			{
				std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Version-Error: Multidrop-Version not as expected. Maybe it is not supported or multidrop was connected to late. Check that Multidrop is connected and switched on at the startup of the SiLAConverter.", "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
				commandCallback->throwError(error);
				return false;
			}
			return true;
		}
		bool SpecificCore::readMultidropVersion(bool& found, DeviceType& deviceType, std::string& multidropVersionInformation)
		{
			found = false;
			if(_simulationMode)
			{
				found = true;
				deviceType = _deviceType;
				_multidropVersionInformation = multidropVersionInformation;
				return true;
			}
			if(_isConnected && !_isConnecting && !_isClosing)
			{
				clearReceivedLinesQueue();

				// send
				std::string line = "VER";
				if(!sendCommand(line))
				{
					return false;
				}

				// wait on answer
				bool commandDone = false;
				long timeoutCounter =  150;
				std::string unexpectedLine = "";
				while(!commandDone)
				{
					if(_connectionError || _multidropError || _isClosing)
					{
						return false;
					}
					while(_receivedLinesCount > 0)
					{
						std::string currentLine = getReceivedLine();

						if(currentLine.length() >= 8)
						{
							std::string lineSubstring = currentLine.substr(0,8);
							if(lineSubstring == "VER Mult")
							{
								if(currentLine.find("MultidropCombi") == 4)
								{
									multidropVersionInformation = currentLine.substr(4);
									deviceType = DeviceType::MultidropCombi;
								}
							}
							/*else if(lineSubstring == "VER Comb")
							{
								if(currentLine.find("Combi&nl") == 4)
								{
									multidropVersionInformation = currentLine.substr(4);
									newDeviceType = DeviceType::MultidropCombiNL;
								}
							}*/ // prohibit Multidrop CombiNL, cause diffrent behaviour and no possibility to test
							else if(lineSubstring == "MdropDW ")
							{
								multidropVersionInformation = currentLine;
								deviceType = DeviceType::MultidropDW;
								commandDone = true;
							}
							else if(lineSubstring == "MDMicro ")
							{
								multidropVersionInformation = currentLine;
								deviceType = DeviceType::MultidropMicro;
								commandDone = true;
							}
							else if(lineSubstring == "Mdrop384")
							{
								multidropVersionInformation = currentLine;
								deviceType = DeviceType::Multidrop384;
								commandDone = true;
							}
							else if(lineSubstring == "VER END ")
							{
								int returnValue = 0;
								try
								{
									returnValue = std::stoi(currentLine.substr(8));
								}
								catch (std::exception e)
								{
									returnValue = -1;
								}
								if(returnValue == 0)
								{
									commandDone = true;
								}
								else if (returnValue > 0)
								{
									_multidropError = true;
									_multidropErrorNumber = returnValue;
								}
							}
							else
							{
								unexpectedLine = currentLine;
							}
						}
					}
					if(timeoutCounter <= 0 && !commandDone && !_multidropError)
					{
						if(unexpectedLine == "")
						{
							std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
							multidropVersionInformation = "";
							_connectionErrorMessage = "Timeout during command execution - No version answer within the estimated time.";
							_connectionError = true;
							closeConnection();
						}
						else
						{
							std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
							multidropVersionInformation = "";
							_connectionErrorMessage = "Error during validating Multidrop version - Multidrop version not as expected / is not supported. (" + unexpectedLine + ")";
							_connectionError = true;
							closeConnection();
						}
					}
					else
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
						timeoutCounter -= 1;
					}
				}
				if(multidropVersionInformation != "")
				{
					found = true;
					return true;
				}
			}
			else
			{
				if(!_isClosing)
				{
					_connectionErrorMessage = "Programming-fault (should not occure): executeModernCommand was not ready";
					_connectionError = true;
					closeConnection();
				}
			}
			return false;
		}

		bool SpecificCore::modernCommandExecution(std::string command, std::string parameter,  std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback, int maximumExecutionSeconds)
		{
			bool retry = true;
			bool returnValue = true;
			while(retry)
			{
				retry = false;
				if(!modernCommandCommunication(command, parameter, maximumExecutionSeconds, commandCallback))
				{
					if(_connectionError)
					{
						std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("RS232-Connection-Error: " + _connectionErrorMessage + " - A RS232-Connection-Error is serious problem. Check that the Multidrop is connected and the power is switched on.", "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
						commandCallback->throwError(error);
						returnValue = false;
					}
					else if(_multidropError && command == "EAK")
					{
						std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: Could not acknowledge the error" , "Retry", "Retry to acknowledge the error", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
						error->addContinuationTask("AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
						std::string continuationTask = commandCallback->throwError(error);
						if(continuationTask == "Retry")
						{
							retry = true;
						}
						else
						{
							returnValue = false;
						}
					}
					else if(_multidropError)
					{
						std::string message = "";
						std::string action = "";
						modernCommandErrorAndAction(_multidropErrorNumber, message, action);

						if(action == "AA")
						{
							std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: " + message , "AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands));
							commandCallback->throwError(error);
							returnValue = false;
						}
						else if(action == "R")
						{
							std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: " + message , "Retry", "Retry to execute the command", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
							error->addContinuationTask("AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
							std::string continuationTask = commandCallback->throwError(error);
							if(continuationTask == "Retry")
							{
								retry = true;
							}
							else
							{
								returnValue = false;
							}
						}
						else if(action == "PRIR")
						{
							std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: " + message , "PrimeAndRetry", "Prime the pump with 100ul and retry to execute the command", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
							error->addContinuationTask("AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
							std::string continuationTask = commandCallback->throwError(error);
							if(continuationTask == "PrimeAndRetry")
							{
								retry = true;
								_multidropError = false;
								_multidropErrorNumber = -1;
								if(!modernCommandExecution("EAK","", commandCallback, 10))
								{
									retry = false;
									returnValue = false;
								}
								else if(!modernCommandExecution("PRI", "10000", commandCallback, 60))
								{
									returnValue = false;
									retry = false;
								}
							}
							else
							{
								returnValue = false;
							}
						}
						else if(action == "PONR")
						{
							std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: " + message , "PressureOnAndRetry", "Switch pressure on and retry to execute the command", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
							error->addContinuationTask("AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
							std::string continuationTask = commandCallback->throwError(error);
							if(continuationTask == "PressureOnAndRetry")
							{
								retry = true;
								_multidropError = false;
								_multidropErrorNumber = -1;
								if(!modernCommandExecution("EAK","", commandCallback, 10))
								{
									retry = false;
									returnValue = false;
								}
								else if(!modernCommandExecution("PON", "1", commandCallback, 60))
								{
									returnValue = false;
									retry = false;
								}
							}
							else
							{
								returnValue = false;
							}
						}
						else
						{
							std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: " + message, "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
							commandCallback->throwError(error);
							returnValue = false;
						}
						_multidropError = false;
						_multidropErrorNumber = -1;
						if(!modernCommandExecution("EAK","", commandCallback, 10))
						{
							retry = false;
							returnValue = false;
						}
					}
					else
					{
						returnValue =  false;
					}
					
				}
			}
			return returnValue;
		}
		void SpecificCore::modernCommandErrorAndAction(int errorNumber, std::string& message, std::string& action)
		{
			switch(errorNumber)
			{
			case 1:
				message = "1 - Internal firmware error. Contact service.";
				action = "E"; // EnterInError
				break;
			case 2:
				message = "2 - The instrument did not recognize the command it received. Contact the SiLAConverter vendor.";
				action = "E"; // EnterInError
				break;
			case 3:
				message = "3 - The arguments of the received command are not valid. Contact the SiLAConverter vendor.";
				action = "E"; // EnterInError
				break;
			case 4:
				message = "4 - Pump position error. Reduce dispensing speed or, if using the small tube cassette, use standard tube cassette instead. If the problem persists even with normal viscosity fluids, contact service.";
				action = "AA"; // AbortAll
				break;
			case 5:
				message = "5 - Plate X position error. If this error is reported repeatedly, contact service.";
				action = "AA"; // AbortAll
				break;
			case 6:
				message = "6 - Plate Y position error. If this error is reported repeatedly, contact service.";
				action = "AA"; // AbortAll
				break;
			case 7:
				message = "7 - Z position error. If this error is reported repeatedly, contact service.";
				action = "AA"; // AbortAll
				break;
			case 8:
				message = "8 - Selector position error. If this error is reported repeatedly, contact service.";
				action = "AA"; // AbortAll
				break;
			case 9:
				message = "9 - Attempt to reset the serial number. Do not try to reprogram the serial number.";
				action = "AA"; // AbortAll
				break;
			case 10:
				message = "10 - Nonvolatile parameters lost. Contact service.";
				action = "E"; // EnterInError
				break;
			case 11:
				message = "11 - No more memory for storing user data. Release some user data no longer needed.";
				action = "AA"; // AbortAll
				break;
			case 12:
				message = "12 - A previous command is executing in the background. Do not use commands interfering with the ongoing operation without first stopping the operation.";
				action = "AA"; // AbortAll
				break;
			case 13:
				message = "13 - X and Z positions conflict. Contact service.";
				action = "E"; // EnterInError
				break;
			case 14:
				message = "14 - Cannot dispense when pump not primed. Prime the pump before dispensing.";
				action = "PRIR"; // PrimeRetry
				break;
			case 15:
				message = "15 - Missing prime vessel. Insert the priming vessel.";
				action = "R"; // Retry
				break;
			case 16:
				message = "16 - Rotor shield not in place. Slide the rotor cover in place.";
				action = "R"; // Retry
				break;
			case 17:
				message = "17 - Dispense volume for all wells is 0. Select a nonezero volume for at least one plate well.";
				action = "AA"; // AbortAll
				break;
			case 18:
				message = "18 - Cannot dispense because the plate type is invalid (bad plate index). Use a valid plate index.";
				action = "AA"; // AbortAll
				break;
			case 19:
				message = "19 - Cannot dispense because the plate has not beend defined. Define the plate.";
				action = "E"; // EnterInError
				break;
			case 20:
				message = "20 - Invalid rows in plate definition. Check the WPL or PLA command.";
				action = "E"; // EnterInError
				break;
			case 21:
				message = "21 - Invalid columns in plate definition. Check the WPL or PLA command.";
				action = "E"; // EnterInError
				break;
			case 22:
				message = "22 - Plate height is invalid. Check the WPL or PLA command.";
				action = "E"; // EnterInError
				break;
			case 23:
				message = "23 - Plate well volume is invalid (too small or too big). Check the WPL or PLA command.";
				action = "E"; // EnterInError
				break;
			case 24:
				message = "24 - The cassette type is invalid (bad cassette index). Use a valid cassette index.";
				action = "AA"; // AbortAll
				break;
			case 25:
				message = "25 - Cassette not defined. Define the cassette.";
				action = "E"; // EnterInError
				break;
			case 26:
				message = "26 - Volume increment defined for cassette is invalid.";
				action = "E"; // EnterInError
				break;
			case 27:
				message = "27 - Maximum volume defined for the cassette is invalid.";
				action = "E"; // EnterInError
				break;
			case 28:
				message = "28 - Minimum volume defined for the cassette is invalid.";
				action = "E"; // EnterInError
				break;
			case 29:
				message = "29 - Minimum or maximum pump speed defined for the cassette is invalid.";
				action = "E"; // EnterInError
				break;
			case 30:
				message = "30 - Pump rotor offset in the cassete definition is invalid.";
				action = "E"; // EnterInError
				break;
			case 32:
				message = "32 - Dispensing volume is not within limits allowed for the cassette. Correct the volume or use a different cassette.";
				action = "AA"; // AbortAll
				break;
			case 33:
				message = "33 - Invalid selector channel. Correct the selector channel number.";
				action = "AA"; // AbortAll
				break;
			case 34:
				message = "34 - Invalid dispensing speed. Use a valid value.";
				action = "AA"; // AbortAll
				break;
			case 35:
				message = "35 - Invalid dispensing height. Correct the dispensing heigth.";
				action = "AA"; // AbortAll
				break;
			case 36:
				message = "36 - Invalid predispense volume. Change the predispense volume of the protocol.";
				action = "AA"; // AbortAll
				break;
			case 37:
				message = "37 - Invalid dispensing order of the plate. Use dispensing order 0 or 1.";
				action = "AA"; // AbortAll
				break;
			case 38:
				message = "38 - Invalid X or Y dispensing offset. Fix the offsets. Note that the limits depend on the used plate type.";
				action = "AA"; // AbortAll
				break;
			case 39:
				message = "39 - RFID option not present. Purchase the RFID option for the instrument.";
				action = "E"; // EnterInError
				break;
			case 40:
				message = "40 - RFID tag not present. Insert a cassette type containing a RFID tag.";
				action = "E"; // EnterInError
				break;
			case 41:
				message = "41 - RFID tag data checksum is incorrect. If the cassette has never been calibrated, calibrate it. Else discard the cassette.";
				action = "E"; // EnterInError
				break;
			case 42:
				message = "42 - Selector valve not installed. Do not try to operate the selector unless it is installed.";
				action = "E"; // EnterInError
				break;
			case 43:
				message = "43 - Wrong cassette type. Use a cassette type matching the cassette type of the current protocol.";
				action = "AA"; // AbortAll
				break;
			case 44:
				message = "44 - Protocol, plate or liquid profile may not be modified or deleted because it is currenty used.";
				action = "E"; // EnterInError
				break;
			case 45:
				message = "45 - Cannot modify or delete protocol or a plate because it is read only.";
				action = "E"; // EnterInError
				break;
			case 46:
				message = "46 - Pressure failure. Adjusting the pressure to the required value failed. Check that the reagent bottle cap is properly closed and that all tubes are tightly fitted.";
				action = "R"; // Retry
				break;
			case 47:
				message = "47 - Hash collision. Two different commands have the same hash value. This error is not fatal, but should never be reported.";
				action = "E"; // EnterInError
				break;
			case 48:
				message = "Too high pressure measurement electronics offset. Contact service.";
				action = "E"; // EnterInError
				break;
			case 49:
				message = "Pressure is off. This error is reported in return to dispense or prime commands if the pressure is off.";
				action = "PONR"; // Pressure On and Retry
				break;
			default :
				message = std::to_string(errorNumber) + " - unknown error number. Take a look to the manual or contact service.";
				action = "E"; // EnterInError
				break;
			}
		}
		bool SpecificCore::modernCommandCommunication(std::string command, std::string parameter, int maximumExecutionSeconds, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback)
		{
			if(_isConnected && !_isConnecting && !_isClosing)
			{
				// send
				std::string line = command;
				if(parameter.length() > 0)
				{
					line += " " + parameter;
				}
				if(command == "ABO")
				{
					line = static_cast<unsigned char>(27);
				}
				if(!sendCommand(line))
				{
					return false;
				}

				// wait on answer
				bool commandDone = false;
				long timeoutCounter =  maximumExecutionSeconds * 50;
				while(!commandDone)
				{
					if(_connectionError || _multidropError || _isClosing)
					{
						return false;
					}
					if(commandCallback != nullptr && commandCallback->hasToAbortOrReset())
					{
						return modernCommandCommunication("ABO", "", 30);
					}
					while(_receivedLinesCount > 0)
					{
						std::string currentLine = getReceivedLine();
						if(currentLine.length() >= 9)
						{
							std::string endStatement = currentLine.substr(0, 8);
							if(endStatement == command + " END ")
							{
								int returnValue = 0;
								try
								{
									returnValue = std::stoi(currentLine.substr(8));
								}
								catch (std::exception e)
								{
									returnValue = -1;
								}
								if(returnValue == 0)
								{
									commandDone = true;
								}
								else if (returnValue > 0)
								{
									_multidropError = true;
									_multidropErrorNumber = returnValue;
								}
							}
						}
					}
					if(timeoutCounter <= 0 && !commandDone && !_multidropError)
					{
						std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
						_connectionErrorMessage = "Timeout during command execution - No " + command + " END answer within the estimated time.";
						_connectionError = true;
						closeConnection();
					}
					else
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
						timeoutCounter -= 1;
					}
				}
			}
			else
			{
				if(!_isClosing)
				{
					_connectionErrorMessage = "Programming-fault (should not occure): executeModernCommand was not ready";
					_connectionError = true;
					closeConnection();
				}
				return false;
			}
			return true;
		}
		bool SpecificCore::oldCommandExecution(std::string line, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback, int maximumExecutionSeconds)
		{
			bool retry = true;
			bool returnValue = true;
			while(retry)
			{
				retry = false;
				if(!oldCommandCommunication(line, maximumExecutionSeconds))
				{
					if(_connectionError)
					{
						std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("RS232-Connection-Error: " + _connectionErrorMessage + " - A RS232-Connection-Error is serious problem. Check that the Multidrop is connected and the power is switched on.", "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::fatalError));
						commandCallback->throwError(error);
						returnValue = false;
					}
					else if(_multidropError)
					{
						switch(_multidropErrorNumber)
						{
						case 3 :
							{
								std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: Invalid command argument" , "AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands));
								commandCallback->throwError(error);
								returnValue = false;
							}
							break;
						case 4 :
							{
								std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: The pump is not primed" , "PrimeAndRetry", "Prime the pump with the default amount and retry to execute the command", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
								error->addContinuationTask("AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
								std::string continuationTask = commandCallback->throwError(error);
								if(continuationTask != "PrimeAndRetry")
								{
									returnValue = false;
								}
								else
								{
									retry = true;
									_multidropError = false;
									_multidropErrorNumber = -1;
									if(!oldCommandExecution("P", commandCallback, 60))
									{
										returnValue = false;
										retry = false;
									}
								}
							}
							break;
						case 5 :
							{
								std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: The priming vessel is not inserted into its slot" , "Retry", "Retry to execute the command", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
								error->addContinuationTask("AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
								std::string continuationTask = commandCallback->throwError(error);
								if(continuationTask != "Retry")
								{
									returnValue = false;
								}
								else
								{
									retry = true;
								}
							}
							break;
						case 6 :
							{
								std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: Hardware error. The instrument stops and must be reset manually." , "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands));
								error->addContinuationTask("Retry", "Retry to execute the command", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand);
								std::string continuationTask = commandCallback->throwError(error);
								if(continuationTask != "Retry")
								{
									returnValue = false;
								}
								else
								{
									retry = true;
								}
							}
							break;
						case 7 :
							{
								std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: The pump has lost steps. The next priming will readjust the rotor home position" , "PrimeAndContinue", "Prime the pump with the default amount and end the command current command.", SPICE::BIG::ContinuationTask::TaskTypes::continueCommand));
								error->addContinuationTask("PrimeAndRetry", "Prime the pump with the default amount and retry to execute the command", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
								error->addContinuationTask("AbortAll", "Abort this and all buffered commands", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands);
								std::string continuationTask = commandCallback->throwError(error);
								if(continuationTask == "PrimeAndRetry")
								{
									retry = true;
									_multidropError = false;
									_multidropErrorNumber = -1;
									if(!oldCommandExecution("P", commandCallback, 60))
									{
										retry = false;
										returnValue = false;
									}
								}
								else if(continuationTask == "PrimeAndContinue")
								{
									_multidropError = false;
									_multidropErrorNumber = -1;
									if(!oldCommandExecution("P", commandCallback, 60))
									{
										returnValue =  false;
									}
									returnValue =  true;
								}
								else
								{
									returnValue =  false;
								}
							}
							break;
						default :
							{
								std::shared_ptr<SPICE::BIG::CommandError> error(new SPICE::BIG::CommandError("Multidrop-Error: unknown error (" + std::to_string(_multidropErrorNumber) + ")." , "EnterInError", "Enters the inError state. A SiLA-Reset is needed.", SPICE::BIG::ContinuationTask::TaskTypes::abortAllCommands));
								commandCallback->throwError(error);
								returnValue = false;
							}
							break;
						}
						_multidropError = false;
						_multidropErrorNumber = -1;
					}
					else
					{
						returnValue =  false;
					}
					
				}
			}
			return returnValue;
		}
		bool SpecificCore::oldCommandCommunication(std::string line, int maximumExecutionSeconds)
		{
			if(_isConnected && !_isConnecting && !_isClosing)
			{
				// send
				if(!sendCommand(line))
				{
					return false;
				}
				if(line == "Q")
				{
					return true;
				}

				// wait on answer
				bool commandDone = false;
				long timeoutCounter =  maximumExecutionSeconds * 50;
				while(!commandDone)
				{
					if(_connectionError || _multidropError || _isClosing)
					{
						return false;
					}
					while(_receivedLinesCount > 0)
					{
						std::string currentLine = getReceivedLine();
						if(currentLine.length() >= 2)
						{
							std::string returnState = currentLine.substr(0, 2);
							if(returnState == "OK")
							{
								commandDone = true;
							}
							else if(returnState == "ER")
							{
								_multidropError = true;
								try
								{
									if(currentLine.length() >= 3)
									{
										_multidropErrorNumber = std::stoi(currentLine.substr(2,1));
									}
								}
								catch (std::exception e)
								{
									_multidropErrorNumber = -1;
								}
							}
						}
					}
					if(timeoutCounter <= 0 && !commandDone && !_multidropError)
					{
						std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
						_connectionErrorMessage = "Timeout during command execution - No OK- or ERR-answer within the estimated time.";
						_connectionError = true;
						closeConnection();
					}
					else
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
						timeoutCounter -= 1;
					}
				}
			}
			else
			{
				if(!_isClosing)
				{
					_connectionErrorMessage = "Programming-fault (should not occure): executeOldCommand was not ready";
					_connectionError = true;
					closeConnection();
				}
				return false;
			}
			return true;
		}

		int SpecificCore::openConnection(unsigned int comPortOffset)
		{
			if(_isConnected || _isConnecting || _simulationMode)
			{
				return -1;
			}
			_isConnecting = true;

			_io_service = new boost::asio::io_service();
			_io_service_keep_running = true;
			_io_service_thread = new std::thread(&SpecificCore::runIOService, this);

	#ifdef LINUX
			std::string portList[] = {"/dev/ttyUSB0", "/dev/ttyUSB1"};
	#else
			std::string portList[] = {"COM9", "COM8", "COM7", "COM6", "COM5", "COM4", "COM3", "COM2", "COM1"};
	#endif
			unsigned int length = sizeof(portList) / sizeof(portList[0]);
			int openedPort = -1;

			for(unsigned int i = comPortOffset; i < length; i++)
			{
				try
				{
					_serial_port = new boost::asio::serial_port(*_io_service);
					_serial_port->open(portList[i]);

					_serial_port->set_option(boost::asio::serial_port_base::baud_rate(9600));
					_serial_port->set_option(boost::asio::serial_port_base::character_size(8));
					_serial_port->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::software));
					_serial_port->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
					_serial_port->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));


					if(_serial_port->is_open())
					{
						_serial_port->async_read_some(boost::asio::buffer(_readBuffer, 1024), boost::bind(&SpecificCore::readedCallback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
						_isConnected = true;
						openedPort = i;

						i = length;
					}
					else
					{
						delete _serial_port;
						_serial_port = nullptr;
					}
				}
				catch (std::exception exp)
				{
					if(_serial_port != nullptr)
					{
						delete _serial_port;
						_serial_port = nullptr;
					}
				}
			}
			if(!_isConnected)
			{
				_io_service_keep_running = false;
				if(_io_service != nullptr)
				{
					_io_service->stop();
					while(!_io_service->stopped())
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
					if(_io_service_thread != nullptr)
					{
						if(_io_service_thread->joinable())
						{
							_io_service_thread->join();
						}
						delete _io_service_thread;
						_io_service_thread = nullptr;
					}
					delete _io_service;
					_io_service = nullptr;
				}
			}

			_isConnecting = false;
			return openedPort;

		}
		void SpecificCore::clearReceivedLinesQueue()
		{
			std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
			_receivedLines.empty();
			_receivedLinesCount = 0;
			_nextReceivingLine = "";
		}
		std::string SpecificCore::getReceivedLine()
		{
			std::string returnValue = "";
			if(_receivedLinesCount > 0)
			{
				std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
				returnValue = _receivedLines.front();
				_receivedLines.pop();
				_receivedLinesCount -= 1;
			}
			return returnValue;
		}

		bool SpecificCore::sendCommand(std::string line)
		{
			bool readyCheck = false;
			if(true)
			{
				std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
				readyCheck = _isConnected && _sendingStates == SendingStates::SENDING_NOT_ACTIVE;
			}
			if(readyCheck)
			{
				if(true)
				{
					std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
					_sendingData = line;
#ifdef RS232_COM_DEBUG
					std::cout << "RS232 send: " << line << std::endl;
#endif
					_sendingData += static_cast<unsigned char>(13);
					_sendingData += static_cast<unsigned char>(10);
					_sendingStates = SendingStates::SENDING_ACTIVE;
					_serial_port->async_write_some(boost::asio::buffer(_sendingData, _sendingData.length()), boost::bind(&SpecificCore::writeCallback, this, boost::asio::placeholders::error));
				}

				bool sendingDone = false;
				long timeoutCounter = 250;
				while(!sendingDone)
				{
					if(_connectionError || _isClosing)
					{
						return false;
					}
					if(true)
					{
						std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
						sendingDone = _sendingStates == SendingStates::SENDING_NOT_ACTIVE;
					}
					if(!sendingDone)
					{
						if(timeoutCounter <= 0)
						{
							std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
							_connectionErrorMessage = "Timeout (5s) during sending a command - Remote Mode not active or cable connection disturbed";
							_connectionError = true;
							closeConnection();
						}
						else
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(20));
							timeoutCounter -= 1;
						}
					}
				}
				return true;
			}
			else
			{
				if(!_isClosing)
				{
					_connectionErrorMessage = "Programming-fault (should not occure): startCommand was not ready";
					_connectionError = true;
					closeConnection();
				}
				return false;
			}
		}
		void SpecificCore::writeCallback(boost::system::error_code error)
		{
			std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
			_sendingStates = SendingStates::SENDING_NOT_ACTIVE;
			if(error)
			{
				if(!_isClosing)
				{
					_connectionErrorMessage = error.message();
					_connectionError = true;
					closeConnection();
				}
			}
		}
		void SpecificCore::readedCallback(boost::system::error_code error, size_t size)
		{
			std::lock_guard<std::mutex> lockGuard(_processCoreMutex);
			if(!error)
			{
				for(unsigned int i = 0; i < size; i++)
				{
					if(_readBuffer[i] == 13 && size > i+1 && _readBuffer[i+1] == 10)
					{
						i++;
						if(_nextReceivingLine.length() > 0)
						{
							_receivedLines.push(_nextReceivingLine);
#ifdef RS232_COM_DEBUG
							std::cout << "RS232 read: " << _nextReceivingLine << std::endl;
#endif
							_receivedLinesCount += 1;
							_nextReceivingLine = "";
						}
					}
					else if(i == 0 && _readBuffer[0] == 10)
					{
						int indexLast = _nextReceivingLine.length() - 1;
						if(_nextReceivingLine.at(indexLast) == 13)
						{
							_nextReceivingLine.erase(indexLast);
							if(_nextReceivingLine.length() > 0)
							{
								_receivedLines.push(_nextReceivingLine);
#ifdef RS232_COM_DEBUG
								std::cout << "RS232 read: " << _nextReceivingLine << std::endl;
#endif
								_receivedLinesCount += 1;
								_nextReceivingLine = "";
							}
						}
					}
					else
					{
						_nextReceivingLine += _readBuffer[i];
					}
				}
				_serial_port->async_read_some(boost::asio::buffer(_readBuffer, 1024), boost::bind(&SpecificCore::readedCallback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
			}
			else
			{
				if(!_isClosing)
				{
					_connectionErrorMessage = error.message();
					_connectionError = true;
					closeConnection();
				}
			}
		}

		void SpecificCore::runIOService()
		{
			while(_io_service_keep_running)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				_io_service->run();
			}
		}

	}
}