#pragma once
#include <types.hpp>
#include "scaling.h"

enum class DigscopeDataType
{
	DIG_SCOPE_DATA_TYPE_INVALID = 0x00,
	DIG_SCOPE_DATA_TYPE_UNSIGNED_INTEGER = 0x01,
	DIG_SCOPE_DATA_TYPE_SIGNED_INTEGER = 0x02,
	DIG_SCOPE_DATA_TYPE_FLOATING_POINT = 0x03,
	DIG_SCOPE_DATA_TYPE_BOOLEAN = 0x04,
	DIG_SCOPE_DATA_TYPE_BITFIELD = 0x05
};


class DigitalOscilloscopeSignal
{
public:
	DigitalOscilloscopeSignal();
	int32_t Serialize( uint8_t* const buf, const uint32_t& offset, const int32_t& maxLength ) const;

	void SetScaling( const ScalingStruct& scaling );

	void SetNameAndUnit( const char* const name, const char* const unit );
	void SetProcessedType( DigscopeDataType type, uint8_t size );
	void SetRawDataType( uint8_t signalIndex, uint64_t signedMask, uint8_t rawSize );

	void SetAlwaysSelectedFlag() { m_bAlwaysSelected = true; }

	DigscopeDataType GetRawDataType() const { return m_rawDataType; }
	uint8_t GetRawWidth() const { return m_nRawSize; }
	uint8_t GetProcessedWidth() const { return m_nProcessedSize; }
	DigscopeDataType GetProcessedType() const { return m_processedDataType; }

	const ScalingStruct& GetScaling() const { return m_scaling; }

private:
	const char* m_pName;			///< The name of the signal. 
	const char* m_pUnit;			///< The unit the signal is presented in after scaling and offset adjustments. 

	ScalingStruct		m_scaling;

	DigscopeDataType m_processedDataType;	///< The data type of the processed signal after scaling and offset adjustment. 
	uint8_t m_nProcessedSize;				///< The processed signal is m_nProcessedSize bytes wide.

	DigscopeDataType m_rawDataType;			///< The data type of the raw signal. 
	uint8_t m_nRawSize;						///< The raw signal is m_nRawSize bytes wide. 
	bool m_bAlwaysSelected;					///< Is this signal always sampled when the oscilloscope is running?
};

union DigscopeSignalUnion
{
	DigscopeSignalUnion()
	{
		signalRegs[0] = 0;
		signalRegs[1] = 0;
	}

	uint16_t signalRegs[2];
	uint8_t signals[4];

};

DigscopeSignalUnion GetCombinedDigscopeSignals( const int8_t* const signals, uint8_t noSignals, bool isBigEndian );

