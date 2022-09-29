#pragma once

#include <types.hpp>
class SetBit
{
public:
	uint16_t RunSetClear(const uint32_t& Inputword, const uint32_t& InputBit, const uint16_t& Outputword, const uint16_t& OutputBit);
	uint16_t RunSet(const uint32_t& Inputword, const uint32_t& InputBit, const uint16_t& Outputword, const uint16_t& OutputBit);
	uint16_t DecodeFault(const uint32_t& Inputword, const uint16_t& Outputword);
	uint16_t DecodeStatus(const uint32_t& Inputword, const uint16_t& Outputword);
	uint16_t Fault(const fp32_t& ActualValue, const fp32_t& LimitValue, const uint16_t& FaultBit);
	uint16_t Warning(const fp32_t& ActualValue, const fp32_t& LimitValue);

private:
	uint16_t   m_status;
	uint16_t   m_status_code;
	uint16_t   m_output;

};

