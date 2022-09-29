#pragma once
#include "DigitalOscilloscopeSource.hpp"

constexpr uint8_t WTCP_DIGSCOPE_RISING_EDGE = 1;
constexpr uint8_t WTCP_DIGSCOPE_FALLING_EDGE = 2;
constexpr uint8_t WTCP_DIGSCOPE_BOTH_EDGES = 3;
constexpr uint8_t WTCP_DIGSCOPE_AUTO_TRIG = 4;

constexpr int32_t ENABLE_WAIT_CNT = 0;
constexpr int32_t DELAYED_READ = 1;

class DigitalOscilloscopeClass
{
public:
	DigitalOscilloscopeClass() = default;

	bool Enable();
	bool Disable();
	void Service();
	uint32_t GetNumberOfSources() const { return m_nSources; }
	inline void SetSources( DigitalOscilloscopeSource** src, uint8_t no )
	{
		m_pSources = src;
		m_nSources = no;
	}
	int32_t SerializeSources( uint8_t* const buf, const uint32_t& offset, const int32_t& maxLength ) const;
	int32_t SerializeSignals( uint8_t source, uint8_t* const buf, const uint32_t& offset,
							  const int32_t& maxLength ) const;
	int32_t GetSerializedStatus( uint8_t* const buf, const uint32_t& offset, const int32_t& maxLength ) const;
	int32_t SelectSignalsDeserialized( const uint8_t* const buf, uint16_t len );
	int32_t SetSamplingAndTriggerDeserialized( const uint8_t* const buf, uint16_t len );

	// Interface
	virtual bool Enable( bool bEnable ) = 0;
	virtual bool SelectSignals( uint8_t sourceIndex, const int8_t* const signals, uint8_t noSignal ) = 0;
	virtual bool ConfigureSampling( int32_t nPretrigSamples, uint32_t nDownsample ) = 0;
	virtual uint16_t GetDataReadyStatus() = 0;
	virtual bool ConfigureTrigger( uint16_t masters, uint8_t source, double level, uint8_t edge ) = 0;

protected:
	void DeselectAllSources();
	DigitalOscilloscopeSource** m_pSources = nullptr; ///< List of sources
	uint16_t m_nSourceMask = 0;						  ///< The selected sources
	uint8_t m_nSources = 0;							  ///< number of sources in the source list
	uint32_t m_nDownSampleCnt = 0;					  ///< Specifies the amount of downsampling (how many sample
													  ///< periods between each sample)
	int32_t m_nWaitEnableCntr = ENABLE_WAIT_CNT;	  ///< Number of cycles to wait after the scope has been enabled
													  ///< before checking its status
	int32_t m_nDelayedRead = DELAYED_READ;			  ///< Number of cycles to wait before reading raw data after the
													  ///< data available flag is set
	bool m_bEnabled = false;						  ///< Oscilloscope enabled flag
	bool m_bProcessedDataReady = false;				  ///< Processed data ready flag
	bool m_bDataReady = false;						  ///< Raw data ready test flag
};
