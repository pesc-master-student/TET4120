#ifndef FLASHPROGRAMMER_HPP
#define FLASHPROGRAMMER_HPP

#include <types.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class FlashProgrammer
/// \brief  API for flash programming. Contains basic functionality and callbacks to
///			user code for accessing the flash.
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class FlashProgrammer
{
public:
	FlashProgrammer() : 
		m_pfWriteData(nullptr),
		m_pfReadData(nullptr), 
		m_pfChipErase(nullptr),
		m_pfSectorErase(nullptr),
		m_pfGetStatusReady(nullptr),
		m_pfReconfigure(nullptr),
		m_pfInit(nullptr),
		m_name(nullptr),
		m_pContext(nullptr),
		m_imageStartAddress(0),
		m_imageBufferLength(0)
	{
	}

	inline void SetContext(void* pContext) { m_pContext = pContext;}
	inline void* GetContext() { return m_pContext;}

	inline void SetImageBuffer( uint32_t startAddress, uint32_t length) { m_imageStartAddress = startAddress; m_imageBufferLength = length;}
	inline uint32_t GetImageStartAddress() const { return m_imageStartAddress;}
	inline uint32_t GetImageBufferLength() const { return m_imageBufferLength;}
	inline uint32_t GetImageActualLength() const { return m_imageActualLength;}

	void SetWriteFunction( bool (*pfWriteData)(void* pContext, uint32_t address, const uint8_t* const pData, uint32_t noBytes) ) { m_pfWriteData = pfWriteData; }
	void SetReadFunction( bool (*pfReadData)(void* pContext, uint32_t address, uint32_t noBytes, uint8_t* pBuf, uint32_t index, uint32_t bufLen, bool useFullAddressRange) ) { m_pfReadData = pfReadData; }
	void SetChipEraseFunction( bool (*pfChipErase)(void* pContext) ) { m_pfChipErase = pfChipErase; }
	void SetSectorEraseFunction( bool (*pfSectorErase)(void* pContext,uint32_t address) ) { m_pfSectorErase = pfSectorErase; }
	void SetStatusReadyFunction( bool (*pfGetStatusReady)(void* pContext) ) {m_pfGetStatusReady = pfGetStatusReady;}
	void SetReconfigureFunction( bool (*pfReconfigure)(void* pContext) ) {m_pfReconfigure = pfReconfigure;}
	void SetInitFunction(bool (*pfInit)(void* pContext)) { m_pfInit = pfInit;};
	void SetGetExtendedErrorCodeFunction( bool (*pfGetExtendedErrorCode)( void* pContext, uint32_t& error ) ) { m_pfGetExtendedErrorCode = pfGetExtendedErrorCode; }
	uint8_t WriteData(uint32_t address, const uint8_t* const pData, uint32_t noBytes);
	uint8_t ChipErase();
	uint8_t SectorErase(uint32_t address);
	uint8_t GetStatus();
	uint8_t ReadData(uint32_t address, uint32_t length, uint8_t* pBuf, uint32_t offset, uint32_t bufferLength, bool useFullAddressRange);
	uint8_t Reconfigure();
	bool GetExtendedErrorCode( uint32_t& error );

	void Init();
	void SetName(const char* pName) { m_name = pName;}
	const char* GetName() const { return m_name;}

private:
	bool (*m_pfWriteData)(void* pContext, uint32_t address, const uint8_t* const pData, uint32_t noBytes);						///< Callback for writing data to flash
	bool (*m_pfReadData)(void* pContext, uint32_t address, uint32_t noBytes, uint8_t* pBuf, uint32_t index, uint32_t bufLen, bool useFullAddressRange); ///< Callback for reading data from flash
	bool (*m_pfChipErase)(void* pContext);																						///< Callback for erasing the whole flash
	bool (*m_pfSectorErase)(void* pContext,uint32_t address);																	///< Callback for erasing single sectors
	bool (*m_pfGetStatusReady)(void* pContext);																					///< Callback for getting status information from the flash
	bool (*m_pfReconfigure)(void* pContext);
	bool (*m_pfInit)(void* pContext);
	bool (*m_pfGetExtendedErrorCode)(void* pContext, uint32_t& error);
	
	const char* m_name;
	void* m_pContext;					///< A void* pointer that will be included in every callback. Can among other things be used as a this pointer.
	uint32_t m_imageStartAddress;		///< If we want to download the whole image before programming this buffer will be used
	uint32_t m_imageBufferLength;		///< The length of the image buffer
	uint32_t m_imageActualLength;		///< The number of valid data bytes in the buffer (How much we have written to the buffer)
};

#endif //\ FLASHPROGRAMMER_HPP
