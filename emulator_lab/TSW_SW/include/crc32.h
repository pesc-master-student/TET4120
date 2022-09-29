#ifndef CRC32_H_INCLUDED
#define CRC32_H_INCLUDED

#include <cstdint>

uint32_t DoCrc32(const uint8_t* data, uint32_t size);
uint32_t Crc32Start() noexcept;
uint32_t Crc32Append(uint32_t crc32, const uint8_t* data, uint32_t size);
uint32_t Crc32Finish(uint32_t crc32) noexcept;

#endif//\#ifndef CRC32_H_INCLUDED
