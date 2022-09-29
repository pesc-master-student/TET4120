#ifndef WTCP_H_INCLUDED
#define WTCP_H_INCLUDED
#include "Config.hpp"
#include "gsl-lite.hpp"

class FlashProgrammer;

#ifdef INCLUDE_SLAVE_INTERFACE
class WtcpSlaveInterface;
#endif

#ifdef INCLUDE_FW_FAULT_LOG_INTERFACE
class WtcpFwFaultLogIf;
#endif

#ifdef INLCUDE_LOAD_FPGA_INTERFACE
class WtcpLoadFpgaInterface;
#endif

#ifdef INLCUDE_BASIC_FAULT_LOG_INTERFACE
class WtcpBasicFaultLogIf;
#endif

#ifdef INCLUDE_DIGITAL_OSCILLOSCOPE
class DigitalOscilloscopeClass;
#endif

#ifdef INCLUDE_WTCP_DATALOGGER
class Datalogger;
#endif


constexpr uint8_t WTCP_APPLICATION_TYPE_SW_BOOTLOADER = 0x01;  ///< Application running from internal memory and able
															   ///< to load another application
constexpr uint8_t WTCP_APPLICATION_TYPE_RAM = 0x02;			   ///< An application that is downloaded after booting
															   ///< the FPGA
constexpr uint8_t WTCP_APPLICATION_TYPE_FW_BOOTLOADER = 0x03;  ///< Bootloader capable of loading firmware.
															   ///< (Programming the configuration flash of FPGAs)
constexpr uint8_t WTCP_APPLICATION_TYPE_XFW_BOOTLOADER = 0x04; ///< Bootloader capable of loading firmware with
															   ///< extended functionality (Like access to the golden
															   ///< image in a multi boot configuration).
/// ************************************************************************************************
/// \class	Wtcp
///
/// \brief	The Wartsila Trondheim Communication Protocol implementation.
/// \details The WTCP protocol is a master slave protocol. The master initiates all communication and the slave only
///          replies to requests from the master.\n 
///          Packet structure:\n
///			 All packets consist of a packet header, payload and a checksum. Little endian byte ordering is used for
///          all packets.\n
///          <b>Request packets</b>\n
///          The request packet has a packet header of 6 bytes where the first two bytes are used to specify the
///          operation that should be done on the target.
///          The next two bytes is the 16 bit packet identifier that is used to identify the response or acknowledge
///          packet from the target.
///          The last two bytes in the packet header is the length of the data part of the packet.\n
///          <TABLE border="1" cellspacing="0">
///			 <TR> <TD> Byte Index </TD> <TD> Content </TD> </TR>
///          <TR> <TD> 0 </TD> <TD> Command  Group </TD> </TR>
///			 <TR> <TD> 1 </TD> <TD> Command </TD> </TR>
///			 <TR> <TD> 2->3 </TD> <TD> Packet ID </TD></TR>
///          <TR> <TD> 4->5 </TD> <TD> Payload Length </TD></TR>
///			 <TR> <TD> 6 -> (6 + Payload Length - 1) </TD> <TD> Payload </TD> </TR>
///			 <TR> <TD> 6 + Payload Length -> 7 + Payload Length </TD><TD> CRC16</TD> </TR>
///			 </TABLE>
///
///          <b>Response packets</b>\n
///          The response packet is sent from the target to the host on completion of the requested operation.
///          The response packet's header is equal to the request packet with two exceptions:
///          - The packet data length field is equal to the length of the data part of the response packet
///          - The response packet header is 7 bytes where the last byte is an error code\n
///          <TABLE border="1" cellspacing="0">
///			 <TR> <TD> Byte Index </TD> <TD> Content </TD> </TR>
///          <TR> <TD> 0 </TD> <TD> Command  Group </TD> </TR>
///			 <TR> <TD> 1 </TD> <TD> Command </TD> </TR>
///			 <TR> <TD> 2->3 </TD> <TD> Packet ID </TD></TR>
///          <TR> <TD> 4->5 </TD> <TD> Payload Length </TD></TR>
///			 <TR> <TD> 6 </TD> <TD> Error Code </TD></TR>
///			 <TR> <TD> 7 -> (7 + Payload Length - 1) </TD> <TD> Payload </TD> </TR>
///			 <TR> <TD> 7 + Payload Length -> 8 + Payload Length </TD><TD> CRC16</TD> </TR>
///			 </TABLE>
///
/// \author	William Gullvik
/// \date	27.3.2015
/// *************************************************************************************************
class Wtcp
{
public: //Functions
	//Wtcp();

	void Init();
	void Service();

	inline void SetReceiveCallback( uint16_t ( *receivePacket )( uint8_t** ppPacket, uint32_t* identifier ) ) noexcept
	{
		m_pfnReceivePacket = receivePacket;
	} ///< \brief Used to set the receive callback. The receive callback is used by WTCP to check for new packets.

	inline void
	SetTransmitCallback( void ( *transmitPacket )( uint8_t* ppPacket, uint16_t size, uint32_t identifier ) ) noexcept
	{
		m_pfnTransmitPacket = transmitPacket;
	} ///< \brief Used to set the transmit callback. The transmit callback is used by WTCP to send responses to received
	  ///< requests.
	
	inline void SetReleasePacketCallback( void ( *releasePacket )( uint32_t identifier ) ) noexcept
	{
		m_pfnReleasePacket = releasePacket;
	} ///< \brief Used to set the release packet callback. The release packet callback is used by WTCP to send remove
	  ///< packets from the input buffer after they have been processed.
	inline void SetResetProcessorCallback( void ( *resetProcessor )() ) noexcept
	{
		m_pfnResetProcessor = resetProcessor;
	} ///< \brief Used to set the reset processor callback. The reset processor callback is used by WTCP to reset the
	  ///< processor (usually as part of loading new software)
	
	inline void SetHardResetCallback( void ( *hardreset )() ) noexcept
	{
		m_pfnHardReset = hardreset;
	} ///< \brief Sets the hard reset callback. This performs a full hardware reset of the device. This may not always
	  ///< be possible for all devices.
	
	inline void SetResetWatchdogTimerCallback( void ( *pfnResetWatchdogTimer )( uint32_t ) ) noexcept
	{
		m_pfnResetWatchdogTimer = pfnResetWatchdogTimer;
	} ///< \brief Sets the reset watchdog callback function.

#ifdef INCLUDE_WTCP_DATALOGGER
	inline void AddDataloggers(
		Datalogger* pDataloggerArray,
		uint32_t noDataloggers ) ///< \brief Add an array of data loggers to WTCP. \param pDataloggerArray Array of data
								 ///< loggers. \param noDataloggers The number of data loggers in the array.
	{
		m_pDataloggers = pDataloggerArray; m_nDataloggers = noDataloggers;
	}
#endif

#ifdef INCLUDE_WTCP_EVENTS
	inline void SetGetEventsCallback(uint16_t (*pfnGetEvents)(uint8_t* pTxBuffer,uint32_t MaxBufferLength)) { m_pfnGetEvents = pfnGetEvents;}					///< \brief Used to initialize the callback function used to retrieve events from the user application.
	inline void SetClearFaultsCallback(uint8_t (*pfnClearFaults)(void)) { m_pfnClearFaults = pfnClearFaults;}													///< \brief Initializes the callback function used to clear fault events in the user application
	inline void SetClearAlarmsCallback(uint8_t (*pfnClearAlarms)(void)) { m_pfnClearAlarms = pfnClearAlarms;}													///< \brief Initializes the callback function used to clear alarm events in the user application
	inline void SetGetEventFlagsCallback(uint16_t (*pfnGetEventFlags)(void)) { m_pfnGetEventFlags = pfnGetEventFlags;}											///< \brief Used to initialize the callback function used to retrieve active event flags from the user application. These flags are the active limit, active alarm and active fault flags.
	inline void SetEnableEventMonitoringCallback( void( *pfnEnableEventMonitoring )( bool ) ) { m_pfnEnableEventMonitoring = pfnEnableEventMonitoring; }		///< \brief Initialize callback function used to request the user application to enable event monitoring.
	inline void SetGetEventsActiveFaultWordsMask( uint16_t( *pfnGetActiveFaultsWord )( void ) ) { m_pfnGetEventsActiveFaultWordsMask = pfnGetActiveFaultsWord; }
#endif

#ifdef INCLUDE_WTCP_TIME
	inline void SetSetTimeOffsetCallback(void (*pfnSetTimeOffset)(const uint64_t& offset)) { m_pfnSetTimeOffset = pfnSetTimeOffset; }							///< \brief Initializes callback function used to set the initial time offset in the user application.
	inline void SetSynchronizeTimeCallback(void (*pfnSynchronizeTime)(const uint64_t& currentTime)) { m_pfnSynchronizeTime = pfnSynchronizeTime; }				///< \brief Initializes clock synchronization callback.
#endif

#ifdef INCLUDE_DIGITAL_OSCILLOSCOPE
	inline void AddDigitalOscilloscopes(DigitalOscilloscopeClass** pDigScope, uint8_t numScopes) { m_pDigScopes = pDigScope; m_nDigscopes = numScopes; }			///< \brief Add an array of digital oscilloscopes
#endif

	inline void SetApplicationIdentifier(uint32_t appId) { m_applicationIdentifier = appId;}																		///< Set the application identifier property.
	inline void SetApplicationVersionNumber(uint32_t appVer) { m_applicationVersionNumber = appVer;}																///< Set the application version number property	
	inline void SetApplicationBuildNumber(uint32_t buildNum) { m_applicationBuildNumber = buildNum;}																///< Set the application build number property	

	inline void SetFirmwareIdentifierStr(const char* pStr) { m_pFirmwareIdentifierStr = pStr;}
	inline void SetFirmwareVersion(uint32_t version) { m_firmwareVersionNumber = version;}
	inline void SetFirmwareBuildnumber(uint32_t buildNum) { m_firmwareBuildNumber = buildNum;}

	inline void SetHardwareIdentifierStr(const char* pStr) { m_pHardwareIdentifierStr = pStr;}
	inline void SetHardwareVersionStr(const char* pStr) { m_pHardwareVersionNumberStr = pStr;}
	inline void SetHardwareSerialNumberStr(const char* pStr) { m_pHardwareSerialNumberStr = pStr;}

	inline void SetBuffers( uint8_t* pTxBuf, uint32_t nTxBufSize, uint32_t nMaxRxSize )
	{
		m_pTxBuf = pTxBuf;
		m_nTransmitBufferSize = nTxBufSize;
		m_nMaxReceiveBufferSize = nMaxRxSize;
	} ///< Initialize pointer to the transmit buffer and specify the receive and transmit buffer size. \param pTxBuf A
	  ///< pointer to the transmit buffer. \param nTxBufSize The size of the transmit buffer. \param nMaxRxSize The
	  ///< maximum size of the receive buffer.

	inline void AddFlashProgrammers( FlashProgrammer* pArrayOfFlashProgrammers, uint8_t noProgrammersInArray )
	{
		m_pFlashProgrammers = pArrayOfFlashProgrammers;
		m_nFlashProgrammers = noProgrammersInArray;
	}

#ifdef INCLUDE_SLAVE_INTERFACE
	inline void SetSlaveInterface( WtcpSlaveInterface** ppSlaves, uint8_t numSlaves)
	{
		m_ppSlaveInterfaces = ppSlaves; m_nSlaveInterfaces = numSlaves;
	}
#endif

#ifdef INCLUDE_FW_FAULT_LOG_INTERFACE
	inline void SetFwFaultLogInterface( WtcpFwFaultLogIf* pfaultLog )
	{
		m_pFwFaultLogInterface = pfaultLog;
	}
#endif

#ifdef INLCUDE_LOAD_FPGA_INTERFACE
	inline void SetLoadFpgaInterface( WtcpLoadFpgaInterface* pInterface )
	{
		m_pLoadFpgaInterface = pInterface;
}
#endif

#ifdef INLCUDE_BASIC_FAULT_LOG_INTERFACE
	inline void SetBasicFaultLogIf(WtcpBasicFaultLogIf* pFaultLogIf)
	{
		m_pBasicFaultLogIf = pFaultLogIf;
	}
#endif

#ifdef HAS_RUN_INHIBIT_SUPPORT
	void SetRunInhibitCommandHandler(void(*handler)(bool inhibit_enable))
	{
		m_pfnHandleRunInhibit = handler;
	}
#endif

protected: // Functions
	void Decode(const uint8_t* const pPacket,const uint16_t packetSize,uint32_t identifier);

	bool SanityCheck(const uint8_t* const pPacket,const uint16_t packetSize);

	bool SanityCheck( const gsl::span<const uint8_t>& packet );
	
	static uint16_t GetPacketDataLength( const gsl::span<const uint8_t>& packet );

	static uint16_t GetPacketDataLength( const uint8_t* pPacket ) noexcept;

	static uint16_t GetPacketChecksum( const uint8_t* const pPacket, const uint16_t packetSize );

	static uint16_t GetPacketChecksum( const gsl::span<const uint8_t>& packet );

	static void SetPacketLength(uint8_t* pPacket, uint16_t length);
	
	uint8_t PutVariables(const uint8_t* pRx) const;
	
	uint8_t GetVersion(const uint8_t* const pRx,uint8_t* const pTxData,uint16_t* const pnBytesWritten) const;
	
	uint8_t GetMemoryBlock( const uint8_t* pRx, uint8_t* pTxData, uint16_t& bytesWritten ) const;
	
	uint8_t PutMemoryBlock(const uint8_t* pRx) const;
	
	uint8_t SetMemory(const uint8_t* pRx) const;
	
	uint8_t RunFunction(const uint8_t* const pRx) const;
	
	uint8_t DoChecksum(const uint8_t* pRx,uint8_t* const pTxData,uint16_t& bytesWritten) const;
	
	uint8_t GetVariables(const uint8_t* pRx, uint8_t* pTxData, uint16_t& bytesWritten) const;
	
	uint8_t WriteReadVariables(const uint8_t* pRx, uint8_t* pTxData, uint16_t& bytesWritten) const;
	
	uint8_t Reset(const uint8_t* const pRx) const;
	
	uint8_t Ping( const uint8_t* pRx ) const;

#ifdef INCLUDE_WTCP_EVENTS
	uint8_t GetEvents(const uint8_t* const pRx,uint8_t* const pTxData,uint16_t* const pnBytesWritten) const;
#endif

#ifdef INCLUDE_WTCP_DATALOGGER
	uint8_t DecodeDatalogger( const uint8_t* pRx, uint8_t* pTxData, uint16_t& bytesWritten ) const;
#endif

#ifdef INCLUDE_WTCP_TIME
	uint8_t DecodeTimeCommands( const uint8_t* pRx, uint8_t* pTxData, uint16_t& bytesWritten ) const;
#endif

#ifdef INCLUDE_DIGITAL_OSCILLOSCOPE
	uint8_t DecodeDigitalOscilloscope(const uint8_t* const pRx,uint8_t* const pTxData,uint16_t* const pnBytesWritten) const;
#endif

#ifdef INCLUDE_SLAVE_INTERFACE
	uint8_t DecodeSlaveInterfaceCommands(const uint8_t* const pRx,uint8_t* const pTxData,uint16_t* const pnBytesWritten) const;
#endif

	uint8_t DecodeFlashProgrammerCommands(const uint8_t* const pRx,uint8_t* const pTxData,uint16_t* const pnBytesWritten) const;

#ifdef INCLUDE_FW_FAULT_LOG_INTERFACE
	uint8_t DecodeFwFaultLogInterfaceCommands( const uint8_t* const pRx, uint8_t* const pTxData, uint16_t* const pnBytesWritten ) const;
#endif

#ifdef INLCUDE_LOAD_FPGA_INTERFACE
	uint8_t DecodeLoadFpgaInterfaceCommands( const uint8_t* const pRx, uint8_t* const pTxData, uint16_t* const pnBytesWritten ) const;
#endif

#ifdef INLCUDE_BASIC_FAULT_LOG_INTERFACE
	uint8_t DecodeBasicFaultLogIf(const uint8_t* pRx, uint8_t* pTxData, uint16_t& bytesWritten) const;
#endif

protected: //Variables

	uint16_t (*m_pfnReceivePacket)(uint8_t** ppPacket, uint32_t* identifier) = nullptr;			///< A pointer to a function that receives packets.
	void (*m_pfnTransmitPacket)(uint8_t* ppPacket,uint16_t size, uint32_t identifier) = nullptr;	///< A pointer to a function that transmits packets.
	void (*m_pfnReleasePacket)(uint32_t identifier) = nullptr;									///< A pointer to a function that releases received packets from the buffer
	void (*m_pfnResetProcessor)() = nullptr;														///< A pointer to a function that is able to reset the processor.
	void (*m_pfnHardReset)() = nullptr;															///< A pointer to a function able to perform a hard reset (reset CPU and peripherals and reload firmware)

	void (*m_pfnResetWatchdogTimer )( uint32_t timeout_ms ) = nullptr;							///< A pointer to a function that resets a watchdog timer to the specified timeout. The usage of this function is user specific but can be used for display timeouts etc.

#ifdef INCLUDE_WTCP_EVENTS
	uint16_t (*m_pfnGetEvents)(uint8_t* pTxBuffer,uint32_t MaxBufferLength) = nullptr;	///< A pointer to a function that serializes events into the transmit buffer
	uint8_t (*m_pfnClearFaults)(void) = nullptr;										///< A pointer to a function that clears active faults.
	uint8_t (*m_pfnClearAlarms)(void) = nullptr;										///< A pointer to a function that clears active alarms.
	uint16_t (*m_pfnGetEventFlags)(void) = nullptr;										///< A pointer to a function that reads event flags from the application.
	void ( *m_pfnEnableEventMonitoring )( bool bEnable ) = nullptr;						///< A pointer to a function that enables or disables event monitoring in the application software.
	uint16_t( *m_pfnGetEventsActiveFaultWordsMask ) ( void ) = nullptr;
#endif 

#ifdef INCLUDE_WTCP_DATALOGGER
	Datalogger* m_pDataloggers = nullptr;				///< A pointer to an array of dataloggers.
	uint32_t m_nDataloggers = 0;						///< The number of dataloggers that are available.
#endif

#ifdef INCLUDE_DIGITAL_OSCILLOSCOPE
	DigitalOscilloscopeClass** m_pDigScopes = nullptr;	///< A pointer to an array  digital oscilloscope pointers
	uint8_t m_nDigscopes = 0;							///< Number of digital oscilloscopes in the m_pDigScopes array.
#endif

#ifdef INCLUDE_WTCP_TIME
	void (*m_pfnSetTimeOffset)(const uint64_t& offset) = nullptr;		 ///< A pointer to a function used to set the
																		 ///< current time offset
	void (*m_pfnSynchronizeTime)(const uint64_t& currentTime) = nullptr; ///< A pointer to a function in the application
																		 ///< that is used to synchronize clocks.
#endif

#ifdef INCLUDE_SLAVE_INTERFACE
	WtcpSlaveInterface** m_ppSlaveInterfaces = nullptr;		///< Array of pointers to slaves
	uint8_t m_nSlaveInterfaces = 0;							///< Number of slaves in m_ppSlaveInterfaces
#endif

#ifdef INCLUDE_FW_FAULT_LOG_INTERFACE
	WtcpFwFaultLogIf* m_pFwFaultLogInterface = nullptr;		///< A pointer to the fault log interface.
#endif

#ifdef INLCUDE_LOAD_FPGA_INTERFACE
	WtcpLoadFpgaInterface* m_pLoadFpgaInterface = nullptr;	///< A pointer to the load FPGA interface
#endif

#ifdef INLCUDE_BASIC_FAULT_LOG_INTERFACE
	WtcpBasicFaultLogIf* m_pBasicFaultLogIf = nullptr;		///< A pointer to the Basic Fault Log interface
#endif

#ifdef HAS_RUN_INHIBIT_SUPPORT
	void(*m_pfnHandleRunInhibit)(bool inhibit_enable) = nullptr;
#endif

	FlashProgrammer* m_pFlashProgrammers = nullptr;		///< A pointer to an array of flash programmers.
														///< The m_nFlashProgrammers variable is used to indicate the
														///< number of flash programmers in the array

	uint32_t m_applicationIdentifier = 0;				///< The application id number
	uint32_t m_applicationVersionNumber = 0;			///< The application version number
	uint32_t m_applicationBuildNumber = 0;				///< The application build number
	const char* m_pFirmwareIdentifierStr = nullptr;		///< A pointer to Null terminated string containing the firmware identifier string ( if applicable, else 0 )
	uint32_t m_firmwareVersionNumber = 0;				///< Firmware build number ( if applicable, else 0 )
	uint32_t m_firmwareBuildNumber = 0;					///< Firmware build number ( if applicable, else 0 )
	const char* m_pHardwareIdentifierStr = nullptr;		///< A pointer to Null terminated string containing the hardware identifier.
	const char* m_pHardwareVersionNumberStr = nullptr;	///< A pointer to Null terminated string containing the hardware version number.
	const char* m_pHardwareSerialNumberStr = nullptr;	///< A pointer to Null terminated string containing the hardware serial number.

#ifdef WTCP_DEBUG_VARIABLES
	uint32_t rx_cnt = 0;						///< The number of received packets.
	uint32_t tx_cnt = 0;						///< The number of transmitted packets.
	uint32_t rx_failed = 0;						///< The number of invalid packets that have been received.
	uint32_t rx_packet_len_error = 0;			///< Number of packet length errors in received packets.
	uint32_t rx_packet_chksum_error = 0;		///< Number of checksum errors in received packets.
#endif
	uint8_t* m_pTxBuf = nullptr;				///< A pointer to the transmit buffer.

	uint16_t m_nTransmitBufferSize = 0;			///< The size in bytes of the transmit buffer
	uint16_t m_nMaxReceiveBufferSize = 0;		///< The size of the receive buffer
	uint8_t m_nFlashProgrammers = 0;			///< Number of flash programmers in the m_pFlashProgrammers array.
};

#endif //\#ifndef WTCP_H_INCLUDED
