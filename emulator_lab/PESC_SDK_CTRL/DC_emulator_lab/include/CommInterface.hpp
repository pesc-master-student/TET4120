#pragma once
#include <array>
#include <cstdint>
const int32_t NUM_RECEIVE_BUFFERS = 3;

class ControlHeader
{
public:
	ControlHeader() : m_nInputReadIndex(0), m_nOutputWriteIndex(0), m_nInputWriteIndex(0), m_nOutputReadIndex(0) {}
	inline bool IsEmpty() const { return m_nInputReadIndex == m_nInputWriteIndex; }
	void IncrementInputReadIndex();

	inline uint32_t GetReadIndex() const { return ( m_nInputReadIndex < NUM_RECEIVE_BUFFERS) ? m_nInputReadIndex : 0; }
	inline uint32_t GetWriteIndex() const { return ( m_nOutputWriteIndex < NUM_RECEIVE_BUFFERS) ? m_nOutputWriteIndex : 0;}
	void IncrementWriteIndex();

private:
    volatile uint32_t m_nInputReadIndex; 	//Only written to by app on CPU1
    volatile uint32_t m_nOutputWriteIndex;	//Only written to by app on CPU1

    volatile uint32_t m_nInputWriteIndex;	//Only written to by linux
    volatile uint32_t m_nOutputReadIndex; 	//Only written to by linux
    
    std::array<uint32_t, 4> m_nReserved;
};

class Packet
{
public:
	inline void SetAddress( uint32_t address ) { m_address = address; }
	inline void SetPort( uint16_t port ){ m_port = port; }
	inline void SetSize( uint16_t sz ) { m_data_len = sz; }
	inline std::array<volatile uint8_t, 0x2000>& GetBuffer() { return m_data;}
	inline uint16_t GetSize() const { return m_data_len; }
	inline uint32_t GetAddress() const { return m_address; }
	inline uint16_t GetPort() const { return m_port; }
private:
    volatile uint32_t m_address;
    volatile uint16_t m_port;
    volatile uint16_t m_data_len;
    std::array<volatile uint8_t, 0x2000> m_data;
};

class CommunicationHandler
{
public:
	void AddPacket( uint32_t address, uint16_t port, uint16_t data_len, const uint8_t* data );
	ControlHeader& GetControl() { return m_control; }
	inline Packet& Receive() { return m_input[(int)m_control.GetReadIndex()]; }
	inline Packet& GetRxPacket( std::size_t index ){ return m_input[index];}
private:
	ControlHeader m_control;
	std::array<Packet,NUM_RECEIVE_BUFFERS> m_input;	
	std::array<Packet,NUM_RECEIVE_BUFFERS> m_output;
};


