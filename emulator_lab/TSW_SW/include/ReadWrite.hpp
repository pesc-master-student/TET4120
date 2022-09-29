#ifndef READ_WRITE_HPP
#define READ_WRITE_HPP

#include <types.hpp>
#include <cstring> // memcpy

namespace ByteArray
{
	uint8_t ReadU8( const unsigned char* const pByteArray, uint32_t& offset );
	uint16_t ReadU16( const unsigned char* const pByteArray, uint32_t& offset );
	uint32_t ReadU32( const unsigned char* const pByteArray, uint32_t& offset );
	uint64_t ReadU64( const unsigned char* const pByteArray, uint32_t& offset );

	uint8_t ReadU8( const unsigned char* const pByteArray );
	uint16_t ReadU16( const unsigned char* const pByteArray );
	uint32_t ReadU32( const unsigned char* const pByteArray );
	uint64_t ReadU64( const unsigned char* const pByteArray );

	void WriteU8( const uint8_t val, uint8_t* byteArray, uint32_t& offset );
	void WriteU16( const uint16_t val, uint8_t* byteArray, uint32_t& offset );
	void WriteU32( const uint32_t val, uint8_t* byteArray, uint32_t& offset );
	void WriteU64( const uint64_t val, uint8_t* byteArray, uint32_t& offset );

	void WriteU8( const uint8_t val, uint8_t* byteArray, const uint32_t& offset );
	void WriteU16( const uint16_t val, uint8_t* byteArray, const uint32_t& offset );
	void WriteU32( const uint32_t val, uint8_t* byteArray, const uint32_t& offset );
	void WriteU64( const uint64_t val, uint8_t* byteArray, const uint32_t& offset );
}

#endif // READ_WRITE_HPP
