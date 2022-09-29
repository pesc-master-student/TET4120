#ifndef FIRMWAREFAULTLOGINTERFACE_HPP
#define FIRMWAREFAULTLOGINTERFACE_HPP

#include <stdint.h>
#include <ReadWrite.hpp>

#define ABOUT_NAME_INVALID	( 0xFF )

#define FAULT_LOG_COMMAND_RESET (1)
#define FAULT_LOG_COMMAND_FORCE_TRIGGER (2)

#define FAULT_LOG_SLOW_LOG (1)
#define FAULT_LOG_FAST_LOG (2)

struct FaultLogDescription
{
	FaultLogDescription()
		: m_name( ABOUT_NAME_INVALID ), m_index( 0 ), m_type( 0 )
	{

	}


	FaultLogDescription( uint8_t name, uint8_t index, uint8_t type )
		: m_name( name ), m_index( index ), m_type( type )
	{

	}

	uint8_t m_name;
	uint8_t m_index;
	uint8_t m_type;
};


class WtcpFwFaultLogIf
{
public:
	virtual ~WtcpFwFaultLogIf() = 0;

	virtual uint8_t GetNumLogs() = 0;

	virtual void Command(uint8_t logIndex, uint32_t command) = 0;

	virtual uint32_t GetStatus(uint8_t* const buf, const uint32_t& offset) = 0;

	virtual uint32_t SerializeFaultLogInfo(uint8_t logIndex, uint8_t* const buf, const uint32_t& offset) = 0;

	virtual uint32_t SerializeFaultFaultLogData(uint8_t logIndex, uint32_t sampleNum, uint16_t numSamples, uint8_t& outErrorCode, uint8_t* const buf, const uint32_t& offset) = 0;

private:
};

static inline void WriteFWFaultLogStatus( uint8_t logIndex, const volatile FaultLogDescription& desc, uint32_t version, uint16_t status, uint64_t triggerTime, uint8_t* const buf, uint32_t& offset )
{
	// Index into the fault logs
	ByteArray::WriteU8( logIndex, buf, offset );

	// Set name so we know about endianness
	ByteArray::WriteU8( desc.m_name, buf, offset );

	// Index defaulting to 0
	ByteArray::WriteU8( desc.m_index, buf, offset );

	// Fault log type (slow/fast)
	ByteArray::WriteU8( desc.m_type, buf, offset );

	// Version of the fault log
	ByteArray::WriteU32( version, buf, offset );

	// Status register
	ByteArray::WriteU16( status, buf, offset );

	// Current trigger-time
	ByteArray::WriteU64( triggerTime, buf, offset );
}


static inline void WriteFWFaultLogInfo( const volatile FaultLogDescription& desc, uint32_t version, uint32_t address, uint16_t datasetSize, uint32_t numSamples, uint32_t numPreSamples, uint32_t deltaT, uint64_t triggerTime, uint8_t* const buf, uint32_t& offset )
{
	// Set the type so we know about the endianness
	ByteArray::WriteU8( desc.m_name, buf, offset );

	// Index defaulting to 0
	ByteArray::WriteU8( desc.m_index, buf, offset );

	// Write fault log type
	ByteArray::WriteU8( desc.m_type, buf, offset );

	// Write the version of the log
	ByteArray::WriteU32( version, buf, offset );

	// Write the address of the log
	ByteArray::WriteU32( address, buf, offset );

	// Write the size of a data set
	ByteArray::WriteU16( datasetSize, buf, offset );

	// Write the number of samples
	ByteArray::WriteU32( numSamples, buf, offset );

	// Write the number of pre-samples
	ByteArray::WriteU32( numPreSamples, buf, offset );

	// Write the delta-T
	ByteArray::WriteU32( deltaT, buf, offset );

	// Write down the Tigger Time
	ByteArray::WriteU64( triggerTime, buf, offset );
}




#endif // FIRMWAREFAULTLOGINTERFACE_HPP
