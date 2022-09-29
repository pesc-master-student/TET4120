#pragma once
#include <types.hpp>
#include <array>

#define DATALOGGER_NO_VARIABLES 40 ///< The maximum number of variables that can be logged at the same time.

constexpr uint8_t TRIGGER_CONDITION_NEVER = 0x00; ///< No trigger condition. Will never trig.
constexpr uint8_t TRIGGER_CONDITION_LT = 0x01;	///< Trig when "value" is less than "trigger value"
constexpr uint8_t TRIGGER_CONDITION_EQ = 0x02;	///< Trig when "value" is equal to "trigger value"
constexpr uint8_t TRIGGER_CONDITION_LEQ = 0x03;   ///< Trig when "value" is less than or equal to "trigger value"
constexpr uint8_t TRIGGER_CONDITION_GT = 0x04;	///< Trig when "value" is larger than "trigger value"
constexpr uint8_t TRIGGER_CONDITION_NEQ = 0x05;   ///< Trig when "value" is not equal "trigger value"
constexpr uint8_t TRIGGER_CONDITION_GEQ = 0x06;   ///< Trig when "value" is larger than or equal to "trigger value"
constexpr uint8_t TRIGGER_CONDITION_ALL = 0x07;   ///< Trig when data logger is enabled.

/// ************************************************************************************************
/// \enum    TriggerVariableTypes
/// \brief   Enum for selecting trigger variable type. Can select unsigned integer, integer or floating point.
///
/// ************************************************************************************************
enum TriggerVariableTypes
{
	INVALID = 0,		  ///< Invalid type
	UNSIGNED_INTEGER = 1, ///< The trigger type is an unsigned integer of some size x defined outside of this enum
	SIGNED_INTEGER = 2,   ///< The trigger type is an integer of some size x defined outside of this enum
	FLOATING_POINT = 3,   ///< The trigger type is a floating point variable
};

/// ************************************************************************************************
/// \union    TriggerValueType
/// \brief    Union for making it easier to read a trigger value in different formats with the targets endian.
///
/// ************************************************************************************************
union TriggerValueType {
	uint8_t uval8;
	uint16_t uval16;
	uint32_t uval32;
	uint64_t uval64;
	int8_t val8;
	int16_t val16;
	int32_t val32;
	int64_t val64;
	fp32_t fp32_val;
	fp64_t fp64_val;
	uint8_t bytes[8];
};

/// ************************************************************************************************
/// \struct   DataloggerVariable
/// \brief    Defines a logged variable
///
/// ************************************************************************************************
struct DataloggerVariable
{
	uint8_t* m_pVariableAddress = nullptr; ///< The variable's address
	uint8_t m_nSize = 0;			 ///< The variable's size in bytes
};

/// ************************************************************************************************
/// \enum    DataloggerProbeTypes
/// \brief   Enum for defining what type of probe is used.
///
/// ************************************************************************************************
enum DataloggerProbeTypes
{
	PROBE_UNINITIALIZED = 0, ///< The type of probe has not been selected
	PROBE_SYNCHRONOUS = 1,   ///< The probe is synchronous. It is sampled regularly e.g. every 250 us
	PROBE_ASYNCHRONOUS =
		2, ///< The probe is asynchronous. It is sampled irregularly and the sampling interval is given by a variable.
};

constexpr uint8_t DATALOGGER_VARIABLE_ENCODING_TYPE_UNSIGNED_INTEGER = 1;
constexpr uint8_t DATALOGGER_VARIABLE_ENCODING_TYPE_SIGNED_INTEGER = 2;
constexpr uint8_t DATALOGGER_VARIABLE_ENCODING_TYPE_FLOATING_POINT = 3;

/// ************************************************************************************************
/// \class   DataloggerProbe
/// \brief   Implements a probe object. Contains all properties of a single probe.
///
/// ************************************************************************************************
class DataloggerProbe
{
public:
	int32_t Serialize( uint8_t* const buf,
					   const uint32_t& offset,
					   const int32_t& maxLength ) const; ///< \brief Serialize the probe to a buffer
	inline void SetProbeType( DataloggerProbeTypes probeType ) noexcept
	{
		m_eProbeType = probeType;
	} ///< \brief Set the probe type. \param probeType The type of probe
	inline void SetSampleTime( float sampleTime ) noexcept
	{
		m_sampleTime = sampleTime;
	} ///< \brief Set the probes sample time (Synchronous probes only). \param sampleTime Sample time in seconds
	inline void SetName( const char* name ) noexcept
	{
		m_pName = name;
	} ///< \brief Set the probe's name. \param name The name of the probe
	inline void SetAsynchronousVariableAddress( const void* pVariable ) noexcept
	{
		m_nAsynchronousTimeVariableAddress = reinterpret_cast<uint32_t>( pVariable );
	} ///< \brief Sets the address of the variable containing sample time information (Asynchronous probes only). \param
	  ///< pVariable The pointer to the variable.
	inline void SetAsynchronousVariableSize( uint8_t size ) noexcept
	{
		m_nAsynchronousTimeVariableSize = size;
	} ///< \brief Sets the size in bytes of the variable containing the sample time of an asynchronous probe.
	inline void SetAsynchronousVariableType( uint8_t type ) noexcept
	{
		m_nAsynchronousTimeVariableType = type;
	} ///< \brief Sets the type of the variable containing the sample time of an asynchronous probe.
protected:
	const char* m_pName = nullptr; ///< The name of the probe. A pointer to a const char string.
	DataloggerProbeTypes m_eProbeType = PROBE_UNINITIALIZED; ///< The probe type.
	float m_sampleTime = 0.0;								 ///< The sample time (seconds). This is only valid for
															 ///< synchronous probes

	uint32_t m_nAsynchronousTimeVariableAddress = 0; ///< The address of the variable containing asynchronous
													 ///< time information
	uint8_t m_nAsynchronousTimeVariableSize = 0;	 ///< The size of the variable containing asynchronous
													 ///< time information
	uint8_t m_nAsynchronousTimeVariableType = 0;	 ///< The type of the variable containing asynchronous
													 ///< time information
};

constexpr uint8_t DATALOGGER_ERRORCODE_NO_ERROR = 0x00;
constexpr uint8_t DATALOGGER_ERRORCODE_BUFFER_TOO_SMALL = 0x12;
constexpr uint8_t DATALOGGER_ERRORCODE_INVALID_PROBE = 0x13;
constexpr uint8_t DATALOGGER_ERRORCODE_INVALID_TRIGGER_TYPE = 0x14;

class Datalogger;

class TriggerCheckBase
{
public:
	TriggerCheckBase() = default;
	TriggerCheckBase( const TriggerCheckBase& ) = default;
	TriggerCheckBase( TriggerCheckBase&& ) = default;
	TriggerCheckBase& operator=( const TriggerCheckBase& ) = default;
	TriggerCheckBase& operator=( TriggerCheckBase&& ) = default;
	virtual ~TriggerCheckBase() {}

	virtual bool TriggerCheck( const Datalogger& dl ) const = 0;
};

class Datalogger
{
public:
	Datalogger();
	void ResetAll();
	void Reset();
	void Run( uint8_t probe );
	void SetTrigger( void* pvTriggerVariable,
					 TriggerVariableTypes triggerVarType,
					 uint8_t triggerCondition,
					 uint8_t triggerVarSize,
					 TriggerValueType triggervalue );

	void SetBuffer( uint8_t* pBuffer, uint32_t length );
	bool AddVariable( uint8_t* pVariable, uint8_t size );
	uint8_t SetSampling( uint32_t noPostSampels, uint32_t noPreSamples, uint32_t downSampling, uint8_t probe );
	uint8_t Enable();
	void Disable();

	inline bool GetEnabledFlag() const noexcept ///< Get the status of the data logger. Is it enabled or not?
	{
		return m_bEnabled;
	}

	inline bool GetTriggerStatusFlag() const noexcept ///< Check if the data logger has trigged.
	{
		return m_bTrigged;
	}

	inline bool GetDataAvailableFlag() const noexcept ///< Check if there is any data available yet.
	{
		return m_bDataAvailable;
	}

	inline uint32_t GetActualPostSampleCount() const noexcept ///< Get the number of samples after trigging.
	{
		return m_nBufferedPostSamples;
	}

	uint32_t GetActualPreSampleCount() const; ///< Get the number of samples before trigging.

	inline uint32_t GetSlotSize() const noexcept ///< Get the size of each slot in the buffer
	{
		return m_nSlotSize;
	}

	inline uint32_t GetNoSlots() const noexcept ///< Get the number of slots in the buffer
	{
		return m_nSlots;
	}

	inline uint32_t GetBufferStartAddress() const noexcept ///< Get the start address of the buffer
	{
		return reinterpret_cast<uint32_t>( m_pBuffer );
	}

	inline uint32_t GetBufferLength() const noexcept ///< Get the length of the buffer in bytes
	{
		return m_nSlotSize * m_nSlots;
	}

	uint32_t GetFirstSampleOffset() const;

	inline uint8_t GetNumberOfProbes() const noexcept ///< Get the number of probes in the probe array
	{
		return m_nProbes;
	}

	inline void SetProbes( DataloggerProbe* pProbes, uint8_t noProbes ) noexcept ///< Set the probe array and its size
	{
		m_pProbes = pProbes;
		m_nProbes = noProbes;
	}

	inline const uint64_t& GetSessionId() const noexcept ///< Get the session id
	{
		return m_nSessionId;
	}

	inline void SetSessionId( const uint64_t& sessionId ) noexcept ///< Set the session id
	{
		m_nSessionId = sessionId;
	}

	int32_t SerializeProbes( uint8_t* const buf, const uint32_t& offset, const int32_t& maxLength ) const;

	inline const void* GetTriggerVariable() const noexcept
	{
		return m_pvTriggerVariable;
	}

	inline const TriggerValueType& GetTriggerValue() const noexcept
	{
		return m_uTriggerValue;
	}

	inline uint8_t GetTriggerConditions() const noexcept
	{
		return m_nTriggerConditions;
	}

	inline TriggerVariableTypes GetTriggerVariableType() const noexcept
	{
		return m_eTriggerVariableType;
	}

	inline uint8_t GetTriggerVarSize() const noexcept
	{
		return m_nTriggerVarSize;
	}

	inline uint32_t GetPostSampleCount() const noexcept
	{
		return m_nSamplesPostTrig;
	}

	inline uint32_t GetPreSampleCount() const noexcept
	{
		return m_nSamplesPreTrig;
	}

	inline uint32_t GetDownsampling() const noexcept
	{
		return m_nDownSampling;
	}

	inline uint8_t GetCurrentProbe() const noexcept
	{
		return m_nCurrentProbe;
	}

	inline const std::array<DataloggerVariable, DATALOGGER_NO_VARIABLES>& GetVariableCollection() const noexcept
	{
		return m_variableCollection;
	}

	inline uint32_t GetNumberOfVariables() const noexcept
	{
		return m_nVariables;
	}

protected: // Functions
	void SampleData();
	void UpdateSlotSize();
	bool CheckBufferSize();

protected:								  // Variables
	const char* m_sName = nullptr;		  ///< Datalogger name, useful when there are more than one
	DataloggerProbe* m_pProbes = nullptr; ///< Pointer to an array of probes if any.
	uint8_t m_nProbes = 0;				  ///< Number of probes in the array pointed to by m_pProbes.

	// ******************************
	// Configuration variables
	// ******************************

	// Buffer variables
	uint8_t* m_pBuffer = nullptr; ///< The buffer used to stored sampled data points
	uint32_t m_nBufferSize = 0U;  ///< The size of the data buffer
	uint32_t m_nSlotSize = 0U;	///< The size in bytes of each slot.
	uint32_t m_nSlots = 0U;		  ///< The number of slots in the buffer:
								  ///< m_nSlots * m_nSlotSize <= m_nBufferSize.

	// ** Everything below this line is cleared by the Reset function

	// Trigger variables
	void* m_pvTriggerVariable = nullptr;				   ///< A pointer to the trigger variable.
	uint8_t m_nTriggerVarSize = 0;						   ///< The size in bytes of the trigger variable.
	TriggerVariableTypes m_eTriggerVariableType = INVALID; ///< The trigger variable's type.
	TriggerValueType m_uTriggerValue;					   ///< The trigger variable's value condition.
	uint8_t m_nTriggerConditions = 0; ///< The trigger condition related to its actual value and the value of
									  ///< m_uTriggerValue.
	uint64_t m_nSessionId = 0;		  ///< A session identifier used by the signal server.

	TriggerCheckBase* m_pfTriggerCheck = nullptr;

	uint32_t m_nSamplesPostTrig = 0U; ///< The number of samples that should be buffered after the trigger
									  ///< conditions have been met
	uint32_t m_nSamplesPreTrig = 0U;  ///< The maximum number of samples that should be buffered before the
									  ///< trigger conditions are met
	uint8_t m_nCurrentProbe = 0;	  ///< The currently select probe in the array m_pProbes.
	uint32_t m_nDownSampling = 0U;	///< Downsampling, 0 and 1 means no downsampling

	uint8_t m_nVariables = 0U; ///< Number of variables selected for logging
	std::array<DataloggerVariable, DATALOGGER_NO_VARIABLES> m_variableCollection; ///< Collection of variables that
																					///< should be logged.

	// ********************************
	// Run time variables
	// ********************************
	uint32_t m_nCurrentSlot = 0U;		  ///< Current slot in the buffer that logged values should be saved to.
	uint32_t m_nBufferedPostSamples = 0U; ///< The number of samples sampled after the trigger conditon was met
										  ///< ( 0 if not yet trigged)
	uint32_t m_nBufferedPreSamples = 0U;  ///< Number of samples sampled before the trigger condition was met.
	uint32_t m_downSampleCnt = 0U;		  ///< Down sample counter, will count up to m_nDownSampling before
										  ///< sampling a variable.
	bool m_bTrigged = false;			  ///< Has the data logger trigged yet?
	bool m_bEnabled = false;			  ///< Is the data logger enabled?
	bool m_bDataAvailable = false;		  ///< Is there any data available?
};
