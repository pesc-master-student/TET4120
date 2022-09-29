#pragma once

#define BIG_ENDIAN_BYTE_ORDER 1
#define LITTLE_ENDIAN_BYTE_ORDER 2

#ifdef UNIT_TESTING
#define DEVICE_BYTE_ORDER LITTLE_ENDIAN_BYTE_ORDER
#else



#ifdef _WIN32
#define __ORDER_LITTLE_ENDIAN__  0
#define __ORDER_BIG_ENDIAN__  1
#define __BYTE_ORDER__  __ORDER_LITTLE_ENDIAN__
#endif

#ifndef __BYTE_ORDER__
#ifndef __BIG_ENDIAN__
#error "Big endian not defined"
#elif __BIG_ENDIAN__ == 1
#define DEVICE_BYTE_ORDER BIG_ENDIAN_BYTE_ORDER
#else
#error "Error: Use big endian for PowerPC"
#endif
#else
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define DEVICE_BYTE_ORDER LITTLE_ENDIAN_BYTE_ORDER
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define DEVICE_BYTE_ORDER BIG_ENDIAN_BYTE_ORDER
#else
#error "Byte order is undefined"
#endif
#endif

#endif