#pragma once

#include <types.hpp>

class WtcpLoadFpgaInterface
{
public:
	virtual bool IsReady() const = 0;							///< Return true if the interface is properly initialized and ready
	virtual uint32_t GetBufferStartAddress() const = 0;			///< Return the start address of the temporary buffer used to store the firmware image before it is loaded into the FPGA
	virtual uint32_t GetBufferLength() const = 0;				///< Return the length ( in bytes ) of the temporary buffer
	virtual void SetImageLength( uint32_t len ) = 0;			///< The signal server should call this command after downloading the image. Shall be set equal to the length in bytes of the downloaded image.
	virtual uint32_t GetExcpectedImageLength() const = 0;		///< Return the excepted length of firmware images, 0 is don't know/care. Can be used to force the signal server to check that the image has the proper length before starting the download.
	virtual bool StartImageLoadProcess() = 0;					///< Starts the load process. Requires that an image has been loaded before processing.
	virtual uint32_t GetLastError() const = 0;					///< Returns the last error after ClearErrors was called. Returns 0 if there are no errors.
	virtual void ClearErrors() = 0;								///< Resets the error variable
	virtual bool IsDone() const = 0;							///< Returns true if the last load process was successfully. Is set to false at: boot, when starting a download processes. Only set to true on successful transfer.
};
