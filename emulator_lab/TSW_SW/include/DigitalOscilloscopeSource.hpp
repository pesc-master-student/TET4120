#pragma once
#include <Array.hpp>
#include "DigitalOscilloscopeSignal.hpp"

constexpr uint8_t MAX_NUMBER_OF_SAMPLED_SIGNALS = 4;

using ActiveSignalsType = Array<int8_t, MAX_NUMBER_OF_SAMPLED_SIGNALS>;

class DigitalOscilloscopeSource
{
public:
	DigitalOscilloscopeSource();
	inline void SetName( const char* name ) { m_pName = name; }									///< Sets the name of the source /param name The name of the source. 0 terminated C string
	int32_t Serialize( uint8_t* const buf, const uint32_t& offset, const int32_t& maxLength ) const;
	int32_t SerializeSignals( uint8_t* const buf, const uint32_t& offset, const int32_t& maxLength ) const;
	void Service();
	inline bool GetActiveFlag() const { return m_bActive; }										///< Returns the source's active flag
	inline void SetSignals( DigitalOscilloscopeSignal* pSignals, uint8_t noSignals ) { m_pSignals = pSignals; m_nSignals = noSignals; }	///< Set the source's signal array
	void ProcessData();
	inline void SetDestinationBuffer( uint8_t* const pDestbuf, uint32_t size ) { m_pProcessedBuffer = pDestbuf; m_nProcessedBufferSize = size; }
	inline uint32_t GetSampleCnt() const { return m_nSamples; }									///< Returns the total number of samples per signal.
	inline int32_t GetPresamples() const { return m_nPreSamples; }								///< Returns the number of samples before trigging.
	inline void ClearActiveSignalList() { m_activeSignals.assign( -1 ); m_nActiveSignals = 0; }	///< Clears the list of active signals.
	inline void ClearActiveFlag() { m_bActive = false; }										///< Clears the source's active flag
	inline void SetActiveFlag() { m_bActive = true; }											///< Sets the source's active flag
	inline uint8_t GetSignalCount() const { return m_nSignals; }								///< Return the number of signals this source has
	inline uint8_t GetActiveSignalCount() const { return m_nActiveSignals; }					///< Return the number of active signals for this source
	inline const int8_t* GetActiveSignalList() const { return m_activeSignals.data(); }			///< Get the list of active signals
	bool AddSignal( uint8_t signal );															///< Add a signal to the source
	void Prepare();
	inline bool ProcessedDataReadyFlag() const { return m_bProcessedDataReady; }				///< Returns the Process data ready flag
	inline void SetDataReadyFlag() { m_bDataReady = true; }										///< Set the processed data ready flag
	inline void SetSampleTime( double ts ) { m_sampleTime = ts; }								///< Set the sample time
	inline void SetAvailable( bool available ) { m_bAvailable = available; }					///< Set the source available flag
	inline bool IsMaster() const { return m_bMasterSource; }
	// Interface
	virtual bool Enable( bool bEnable ) = 0;
	virtual const uint8_t* GetSourceBuffer() const = 0;
	virtual uint32_t GetSourceBufferLength() const = 0;
	virtual uint16_t GetTotalDataLength() const = 0;
	virtual uint16_t GetActualPreTrigLength() const = 0;
	virtual const uint64_t& GetTimeStamp() const = 0;

protected:
	double m_sampleTime;						///< Default value set in the constructor is 10 ns. 
	const char* m_pName;						///< The name of the source, example : GDM1. 
	DigitalOscilloscopeSignal* m_pSignals;		///< Pointer to an array of signals. 
	uint8_t m_nSignals;							///< The length of the signal array. 
	uint8_t* m_pProcessedBuffer;				///< Pointer to the buffer where the processed data should be stored. 
	uint32_t m_nProcessedBufferSize;			///< Size of the destination buffer. 
	uint32_t m_nSamples;						///< Number of samples per signal (Pre + Post samples)
	int32_t m_nPreSamples;						///< Number of samples before trigging
	bool m_bActive;								///< Is the oscilloscope active
	bool m_bDataReady;							///< Are there any raw data ready?
	bool m_bProcessedDataReady;					///< Are there any processed data ready?
	uint8_t m_nActiveSignals;					///< Number of active signals
	ActiveSignalsType m_activeSignals;			///< Array of active signals
	bool m_bAvailable;							///< Is the source available
	bool m_bMasterSource;						///< Indicates that this source is the trigger master
};
