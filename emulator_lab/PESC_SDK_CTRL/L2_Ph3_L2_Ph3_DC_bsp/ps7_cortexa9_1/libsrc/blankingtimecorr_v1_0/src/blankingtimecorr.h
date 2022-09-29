#ifndef BLANKINGTIMECORR__H
#define BLANKINGTIMECORR__H
#ifdef __cplusplus
extern "C" {
#endif
/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "blankingtimecorr_hw.h"
/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 blankingtimecorr_BaseAddress;
} blankingtimecorr_Config;
#endif
/**
* The blankingtimecorr driver instance data. The user is required to
* allocate a variable of this type for every blankingtimecorr device in the system.
* A pointer to a variable of this type is then passed to the driver
* API functions.
*/
typedef struct {
    u32 blankingtimecorr_BaseAddress;
    u32 IsReady;
} blankingtimecorr;
/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define blankingtimecorr_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define blankingtimecorr_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define blankingtimecorr_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define blankingtimecorr_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif
/************************** Function Prototypes *****************************/
#ifndef __linux__
int blankingtimecorr_Initialize(blankingtimecorr *InstancePtr, u16 DeviceId);
blankingtimecorr_Config* blankingtimecorr_LookupConfig(u16 DeviceId);
int blankingtimecorr_CfgInitialize(blankingtimecorr *InstancePtr, blankingtimecorr_Config *ConfigPtr);
#else
int blankingtimecorr_Initialize(blankingtimecorr *InstancePtr, const char* InstanceName);
int blankingtimecorr_Release(blankingtimecorr *InstancePtr);
#endif
/**
* Write to ic_in gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the ic_in instance to operate on.
* @param	Data is value to be written to gateway ic_in.
*
* @return	None.
*
* @note    .
*
*/
void blankingtimecorr_ic_in_write(blankingtimecorr *InstancePtr, int Data);
/**
* Read from ic_in gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the ic_in instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int blankingtimecorr_ic_in_read(blankingtimecorr *InstancePtr);
/**
* Write to ib_in gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the ib_in instance to operate on.
* @param	Data is value to be written to gateway ib_in.
*
* @return	None.
*
* @note    .
*
*/
void blankingtimecorr_ib_in_write(blankingtimecorr *InstancePtr, int Data);
/**
* Read from ib_in gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the ib_in instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int blankingtimecorr_ib_in_read(blankingtimecorr *InstancePtr);
/**
* Write to ia_in gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the ia_in instance to operate on.
* @param	Data is value to be written to gateway ia_in.
*
* @return	None.
*
* @note    .
*
*/
void blankingtimecorr_ia_in_write(blankingtimecorr *InstancePtr, int Data);
/**
* Read from ia_in gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the ia_in instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int blankingtimecorr_ia_in_read(blankingtimecorr *InstancePtr);
/**
* Write to blankingtime gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the blankingtime instance to operate on.
* @param	Data is value to be written to gateway blankingtime.
*
* @return	None.
*
* @note    .
*
*/
void blankingtimecorr_blankingtime_write(blankingtimecorr *InstancePtr, int Data);
/**
* Read from blankingtime gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the blankingtime instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int blankingtimecorr_blankingtime_read(blankingtimecorr *InstancePtr);
/**
* Read from t_samp_on gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the t_samp_on instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 blankingtimecorr_t_samp_on_read(blankingtimecorr *InstancePtr);
/**
* Read from ta_on gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the ta_on instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 blankingtimecorr_ta_on_read(blankingtimecorr *InstancePtr);
/**
* Read from tb_on gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the tb_on instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 blankingtimecorr_tb_on_read(blankingtimecorr *InstancePtr);
/**
* Read from tc_on gateway of blankingtimecorr. Assignments are LSB-justified.
*
* @param	InstancePtr is the tc_on instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 blankingtimecorr_tc_on_read(blankingtimecorr *InstancePtr);
#ifdef __cplusplus
}
#endif
#endif
