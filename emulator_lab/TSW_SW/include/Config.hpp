#ifndef CONFIG_HPP_INCLUDED
#define CONFIG_HPP_INCLUDED

#include <Endianess.hpp>

constexpr uint16_t WTCP_VERSION = 600;	///< Version number in the form Major.Minor, the value written here is divided
										///< by 1000 to create the Major version whereas the Minor version is the
										///< remainder of this division.

#define WTCP_DEBUG_VARIABLES	///< Include some debug variables in the WTCP structure

#define WTCP_SMALL	0x01		///< Small, Limited, version of WTCP
#define WTCP_FULL	0x02		///< Fully featured version

// Standard capabilities
#define CAPABILITY_FLAG_PING						0x00001
#define CAPABILITY_FLAG_RESET						0x00002
#define CAPABILITY_FLAG_PUT_VARIABLE				0x00004
#define CAPABILITY_FLAG_GET_VERSION_APP				0x00008
#define CAPABILITY_FLAG_GET_MEMORY_BLOCK_8			0x00010
#define CAPABILITY_FLAG_PUT_MEMORY_BLOCK_8			0x00020
#define CAPABILITY_FLAG_SET_MEMORY_BLOCK_8			0x00040
#define CAPABILITY_FLAG_GET_VARIABLES				0x00080
#define CAPABILITY_FLAG_RUN_FUNCTION				0x00100
#define CAPABILITY_FLAG_CRC16						0x00200
#define CAPABILITY_FLAG_CRC32						0x00400
#define CAPABILITY_FLAG_WRITE_READ_VARIABLES		0x00800
#define CAPABILITY_FLASH_PROGRAMMING				0x01000

//Extended capabilities
#define CAPABILITY_FLAG_DATALOGGER					0x010000
#define CAPABILITY_FLAG_EVENTS						0x020000
#define CAPABILITY_FLAG_TIME						0x040000
#define CAPABILITY_FLAG_SLAVE_INTERFACE				0x100000

#ifdef HAS_DIGSCOPE_CAPABILITY
#define CAPABILITY_FLAG_DIG_SCOPE					0x080000
#define INCLUDE_DIGITAL_OSCILLOSCOPE
#else
#define CAPABILITY_FLAG_DIG_SCOPE					0x000000
#endif

#ifdef HAS_FW_FAULT_LOG_CAPABILITY
#define CAPABILITY_FLAG_FW_FAULT_LOG_INTERFACE		0x200000
#define CAPABILITY_FLAG_FW_FAULT_LOG_INTERFACE_OLD	0x400000
#define INCLUDE_FW_FAULT_LOG_INTERFACE
#else
#define CAPABILITY_FLAG_FW_FAULT_LOG_INTERFACE		0x000000
#define CAPABILITY_FLAG_FW_FAULT_LOG_INTERFACE_OLD	0x000000
#endif

#ifdef HAS_LOAD_FPGA_CAPABILITY
#define CAPABILITY_FLAG_LOAD_FPGA					0x800000
#define INLCUDE_LOAD_FPGA_INTERFACE
#else
#define CAPABILITY_FLAG_LOAD_FPGA					0x000000
#endif

#ifdef HAS_BASIC_FAULT_LOG_IF_CAPABILITY
#define CAPABILITY_FLAG_BASIC_FAULT_LOG_IF		   0x1000000
#define INLCUDE_BASIC_FAULT_LOG_INTERFACE
#else
#define CAPABILITY_FLAG_BASIC_FAULT_LOG_IF		   0x0000000
#endif

#ifdef HAS_RUN_INHIBIT_SUPPORT
#define CAPABILITY_FLAG_RUN_INHIBIT				   0x2000000
#else
#define CAPABILITY_FLAG_RUN_INHIBIT				   0x0000000
#endif

#define CAPABILITIES_STANDARD (0x01FFF | CAPABILITY_FLAG_LOAD_FPGA)

#ifdef SMALL
#define WTCP_TYPE	WTCP_SMALL
const unsigned long long CAPABILITIES = CAPABILITIES_STANDARD;
#else
#ifdef FULL
#define WTCP_TYPE	WTCP_FULL

#define INCLUDE_WTCP_DATALOGGER
#define INCLUDE_WTCP_EVENTS
#define INCLUDE_WTCP_TIME

#define INCLUDE_SLAVE_INTERFACE


const unsigned long long CAPABILITIES = CAPABILITIES_STANDARD | CAPABILITY_FLAG_DATALOGGER | CAPABILITY_FLAG_EVENTS | CAPABILITY_FLAG_TIME | CAPABILITY_FLAG_DIG_SCOPE | CAPABILITY_FLAG_SLAVE_INTERFACE | CAPABILITY_FLAG_FW_FAULT_LOG_INTERFACE | CAPABILITY_FLAG_LOAD_FPGA | CAPABILITY_FLAG_BASIC_FAULT_LOG_IF | CAPABILITY_FLAG_RUN_INHIBIT;

#else
#error NONE
#endif //\#ifdef FULL

#endif

// ********************************************************
//           Get the Endian from the compiler
// ********************************************************
/*
#ifndef DEVICE_BYTE_ORDER

#define BIG_ENDIAN_BYTE_ORDER 1
#define LITTLE_ENDIAN_BYTE_ORDER 2

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

#endif //\#ifndef DEVICE_BYTE_ORDER*/


#endif //\#ifndef CONFIG_HPP_INCLUDED

