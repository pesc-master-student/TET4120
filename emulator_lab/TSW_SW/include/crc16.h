#pragma once

#include <cstdint>
#include <gsl-lite.hpp>

uint16_t doCrc16(const uint8_t* data, uint32_t size);

uint16_t DoCrc16( gsl::span<const uint8_t> data ) noexcept;

