/**
	SPI Multidrop
	SpecificCore.h
	Purpose: Central class of a specific implemention. Usually used to communicate with the device. An instance is just deleted and new generated with the reset command. Every command can use it.

	@author Lukas Mueller (ilt.hsr.ch)
	@version 0.1 2015_06_12
*/

#ifndef SPECIFICCORE_H
#define SPECIFICCORE_H

#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "ICommandCallback.h"
#include "GeneralFunctions.h"

namespace SPI
{
	namespace Multidrop
	{
		class SpecificCore
		{
			public:

				enum DeviceType {
				MultidropCombi,
				MultidropCombiNL,
				MultidropDW,
				MultidropMicro,
				Multidrop384};

				SpecificCore(bool simulationMode);
				virtual ~SpecificCore();

				bool startupFindDevice();
				bool openConnection();
				bool closeConnection();
				bool isReadyToReset();

				DeviceType getDeviceType();
				std::string getMultidropDeviceTypeName();
				std::string getMultidropFirmwareVersion();
				std::string getMultidropSerialNo();

				int calculateTimeDispense(unsigned int volume);
				int calculateTimeDispenseByColumn(unsigned int volume, unsigned int startColumn, unsigned int stopColumn);
				int calculateTimeEmptyTubesAndPrime(unsigned long volume);

				bool init(unsigned int plateType, bool usePlateTypeConfig, unsigned int predispenseVolume, unsigned int dispensingHeight, unsigned int dispensingOrder, int dispenseOffsetX, int dispenseOffsetY, unsigned int cassetteType, unsigned int pumpSpeed, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool init(unsigned int plateType, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool setParameters(unsigned int plateType, bool usePlateTypeConfig, unsigned int predispenseVolume, unsigned int dispensingHeight, unsigned int dispensingOrder, int dispenseOffsetX, int dispenseOffsetY, unsigned int cassetteType, unsigned int pumpSpeed, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool setParameters(unsigned int plateType, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool reset(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool dispense(unsigned int volume, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool dispenseByColumn(unsigned int volume, unsigned int startColumn, unsigned int stopColumn, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool emptyTubesVolume(unsigned long volume, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool emptyTubesTime(unsigned long time, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool emptyTubes(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool prime(unsigned long volume, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool shake(unsigned int time, unsigned int distance, unsigned int speed, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool shake(unsigned int time, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool prepareForInOutput(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool checkForPauseAndReturn(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);

			protected:
			private:

				void loadLastMultidropVersion();
				void setNewMultidropVersion(DeviceType newDeviceType, std::string multidropVersionInformation);

				bool getMultidropVersion();
				bool validateMultidropVersion(std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback);
				bool readMultidropVersion(bool& found, DeviceType& deviceType, std::string& multidropVersionInformation);

				bool modernCommandExecution(std::string command, std::string parameter, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback, int maximumExecutionSeconds = 300);
				void modernCommandErrorAndAction(int errorNumber, std::string& message, std::string& action);
				bool modernCommandCommunication(std::string command, std::string parameter, int maximumExecutionSeconds, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback = nullptr);
				bool oldCommandExecution(std::string line, std::shared_ptr<SPICE::BIG::ICommandCallback> commandCallback, int maximumExecutionSeconds = 300);
				bool oldCommandCommunication(std::string line, int maximumExecutionSeconds);

				int openConnection(unsigned int comPortOffset);
				void clearReceivedLinesQueue();
				std::string getReceivedLine();
				bool sendCommand(std::string line);
				void writeCallback(boost::system::error_code error);
				void readedCallback(boost::system::error_code error, size_t size);
				void runIOService();

				enum SendingStates {SENDING_NOT_ACTIVE, SENDING_ACTIVE};
				DeviceType _deviceType;

				unsigned int _settedCassetteType;
				unsigned int _settedPlateTypeCombi;
				unsigned int _settedSpeed;
				unsigned int _settedPlateTypeMicroDW384;

				// connection flags
				std::atomic<bool> _simulationMode;
				std::atomic<bool> _isConnected;
				std::atomic<bool> _isConnecting;
				std::atomic<bool> _isClosing;
				SendingStates _sendingStates;
				std::atomic<bool> _connectionError;
				std::atomic<bool> _multidropError;
				std::string _connectionErrorMessage;
				int _multidropErrorNumber;

				// sending and receiving data
				std::string _sendingData;
				unsigned char _readBuffer[1024];
				std::queue<std::string> _receivedLines;
				std::atomic<int> _receivedLinesCount;
				std::string _nextReceivingLine;
				std::string _multidropVersionInformation;
				std::string _multidropFirmwareVersion;
				std::string _multidropSerialNo;
				std::string _multidropDeviceTypeName;

				// ports and threads
				boost::asio::io_service* _io_service;
				boost::asio::serial_port* _serial_port;
				std::thread* _io_service_thread;
				std::atomic<bool> _io_service_keep_running;
				std::mutex _processCoreMutex;
				unsigned int _portOffset;


		};
	}
}
#endif // SPECIFICCORE_H
