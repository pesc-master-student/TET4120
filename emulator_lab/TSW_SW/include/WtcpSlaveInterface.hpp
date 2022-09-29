#ifndef WTCPSLAVEINTERFACE_HPP
#define WTCPSLAVEINTERFACE_HPP

#include <types.hpp>

constexpr uint8_t WTCP_SLAVE_INTERFACE_NO_ERROR = 0;
constexpr uint8_t WTCP_SLAVE_INTERFACE_ERROR_NOT_IMPLEMENTED = 1;
constexpr uint8_t WTCP_SLAVE_INTERFACE_ERROR_NO_LINK = 2;
constexpr uint8_t WTCP_SLAVE_INTERFACE_ERROR_TIMEOUT = 3;
constexpr uint8_t WTCP_SLAVE_INTERFACE_ERROR_FAILED = 4;

class WtcpSlaveInterface
{
public:
	// Required
	virtual uint8_t ReadRegister( uint8_t address, uint16_t& data) = 0;
	virtual uint32_t ReadRegisters( const uint8_t* addresses, uint8_t num_reads, uint8_t* const buf, uint32_t offset, uint32_t max_length, uint8_t& ec ) = 0;
	virtual uint8_t WriteRegister( uint8_t  address, uint16_t data) = 0;
	virtual int32_t SerializeData(uint8_t* const buf,const uint32_t& offset, uint32_t max_length ) const = 0;
	virtual int32_t SerializeDetailedData(uint8_t* const buf,const uint32_t& offset, uint32_t max_length ) const = 0;

	virtual uint32_t ReadSlaveRegisters( uint8_t slave_index, uint8_t num_slave_reads, const uint8_t* slave_addresses, uint8_t* const buf, uint32_t offset, uint32_t max_length, uint8_t& ec ) = 0;
	virtual uint8_t WriteSlaveRegister( uint8_t slave_index, uint8_t address, uint16_t data ) = 0;
protected:

private:
	
};

#endif //\ WTCPSLAVEINTERFACE_HPP
