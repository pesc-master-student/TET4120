#pragma once

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class WtcpBasicFaultLogIf
/// \brief  API for collecting fault logs. It is up to the user to create a new class that inherits this class
/// and implements all the abstract methods.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class WtcpBasicFaultLogIf
{
public:
	virtual ~WtcpBasicFaultLogIf() {}
	virtual uint32_t GetNumberOfLogs() const = 0;			///< \brief Returns the number of monitored fault logs. Supports up to 32 logs.
	virtual uint32_t GetEnabledMask() const = 0;			///< \brief Returns a mask of enabled logs. Example: Will return 0x5 if log 1 and 3 are enabled.
	virtual uint32_t GetDataAvailableMask() const = 0;		///< \brief Returns a mask of logs that have new data available. Use the ClearDataAvailable method to clear a flag after downloading a log.
	virtual uint32_t SerializeLogInfo(uint8_t* const pBuf, uint32_t offset, uint32_t bufLen ) const = 0;
	virtual void ClearDataAvailableFlag(uint32_t logIndex) = 0;	///< \brief Clears the data available flag for a log. \param logIndex The index of the log whose data available flag should be cleared.
	virtual uint64_t GetTriggerTimeIndex( uint32_t logIndex ) const = 0;	///< \brief Get the system time point when the log was trigged.
};
