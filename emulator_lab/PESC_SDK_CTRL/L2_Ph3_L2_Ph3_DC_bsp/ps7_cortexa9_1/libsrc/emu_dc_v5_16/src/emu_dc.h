#ifndef EMU_DC__H
#define EMU_DC__H
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
#include "emu_dc_hw.h"
/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 emu_dc_BaseAddress;
} emu_dc_Config;
#endif
/**
* The emu_dc driver instance data. The user is required to
* allocate a variable of this type for every emu_dc device in the system.
* A pointer to a variable of this type is then passed to the driver
* API functions.
*/
typedef struct {
    u32 emu_dc_BaseAddress;
    u32 IsReady;
} emu_dc;
/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define emu_dc_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define emu_dc_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define emu_dc_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define emu_dc_ReadReg(BaseAddress, RegOffset) \
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
int emu_dc_Initialize(emu_dc *InstancePtr, u16 DeviceId);
emu_dc_Config* emu_dc_LookupConfig(u16 DeviceId);
int emu_dc_CfgInitialize(emu_dc *InstancePtr, emu_dc_Config *ConfigPtr);
#else
int emu_dc_Initialize(emu_dc *InstancePtr, const char* InstanceName);
int emu_dc_Release(emu_dc *InstancePtr);
#endif
/**
* Write to voltage_pu_bit_conv2 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the voltage_pu_bit_conv2 instance to operate on.
* @param	Data is value to be written to gateway voltage_pu_bit_conv2.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_voltage_pu_bit_conv2_write(emu_dc *InstancePtr, int Data);
/**
* Read from voltage_pu_bit_conv2 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the voltage_pu_bit_conv2 instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_voltage_pu_bit_conv2_read(emu_dc *InstancePtr);
/**
* Write to voltage_pu_bit_conv1 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the voltage_pu_bit_conv1 instance to operate on.
* @param	Data is value to be written to gateway voltage_pu_bit_conv1.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_voltage_pu_bit_conv1_write(emu_dc *InstancePtr, int Data);
/**
* Read from voltage_pu_bit_conv1 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the voltage_pu_bit_conv1 instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_voltage_pu_bit_conv1_read(emu_dc *InstancePtr);
/**
* Write to ra gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ra instance to operate on.
* @param	Data is value to be written to gateway ra.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_ra_write(emu_dc *InstancePtr, u32 Data);
/**
* Read from ra gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ra instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 emu_dc_ra_read(emu_dc *InstancePtr);
/**
* Write to polepairs gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the polepairs instance to operate on.
* @param	Data is value to be written to gateway polepairs.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_polepairs_write(emu_dc *InstancePtr, int Data);
/**
* Read from polepairs gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the polepairs instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_polepairs_read(emu_dc *InstancePtr);
/**
* Write to magnetization_4q_1q gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the magnetization_4q_1q instance to operate on.
* @param	Data is value to be written to gateway magnetization_4q_1q.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_magnetization_4q_1q_write(emu_dc *InstancePtr, u32 Data);
/**
* Read from magnetization_4q_1q gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the magnetization_4q_1q instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 emu_dc_magnetization_4q_1q_read(emu_dc *InstancePtr);
/**
* Write to kn gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the kn instance to operate on.
* @param	Data is value to be written to gateway kn.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_kn_write(emu_dc *InstancePtr, int Data);
/**
* Read from kn gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the kn instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_kn_read(emu_dc *InstancePtr);
/**
* Write to k_fnmechtstep gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the k_fnmechtstep instance to operate on.
* @param	Data is value to be written to gateway k_fnmechtstep.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_k_fnmechtstep_write(emu_dc *InstancePtr, int Data);
/**
* Read from k_fnmechtstep gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the k_fnmechtstep instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_k_fnmechtstep_read(emu_dc *InstancePtr);
/**
* Write to current_pu_bit_conv2 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the current_pu_bit_conv2 instance to operate on.
* @param	Data is value to be written to gateway current_pu_bit_conv2.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_current_pu_bit_conv2_write(emu_dc *InstancePtr, int Data);
/**
* Read from current_pu_bit_conv2 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the current_pu_bit_conv2 instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_current_pu_bit_conv2_read(emu_dc *InstancePtr);
/**
* Write to current_pu_bit_conv1 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the current_pu_bit_conv1 instance to operate on.
* @param	Data is value to be written to gateway current_pu_bit_conv1.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_current_pu_bit_conv1_write(emu_dc *InstancePtr, int Data);
/**
* Read from current_pu_bit_conv1 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the current_pu_bit_conv1 instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_current_pu_bit_conv1_read(emu_dc *InstancePtr);
/**
* Write to vdc2 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the vdc2 instance to operate on.
* @param	Data is value to be written to gateway vdc2.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_vdc2_write(emu_dc *InstancePtr, int Data);
/**
* Read from vdc2 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the vdc2 instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_vdc2_read(emu_dc *InstancePtr);
/**
* Write to vdc1 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the vdc1 instance to operate on.
* @param	Data is value to be written to gateway vdc1.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_vdc1_write(emu_dc *InstancePtr, int Data);
/**
* Read from vdc1 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the vdc1 instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_vdc1_read(emu_dc *InstancePtr);
/**
* Write to ts_la gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ts_la instance to operate on.
* @param	Data is value to be written to gateway ts_la.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_ts_la_write(emu_dc *InstancePtr, u32 Data);
/**
* Read from ts_la gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ts_la instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 emu_dc_ts_la_read(emu_dc *InstancePtr);
/**
* Write to ts_tm gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ts_tm instance to operate on.
* @param	Data is value to be written to gateway ts_tm.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_ts_tm_write(emu_dc *InstancePtr, int Data);
/**
* Read from ts_tm gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ts_tm instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_ts_tm_read(emu_dc *InstancePtr);
/**
* Write to ts_tf gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ts_tf instance to operate on.
* @param	Data is value to be written to gateway ts_tf.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_ts_tf_write(emu_dc *InstancePtr, u32 Data);
/**
* Read from ts_tf gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ts_tf instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 emu_dc_ts_tf_read(emu_dc *InstancePtr);
/**
* Write to tl gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the tl instance to operate on.
* @param	Data is value to be written to gateway tl.
*
* @return	None.
*
* @note    .
*
*/
void emu_dc_tl_write(emu_dc *InstancePtr, int Data);
/**
* Read from tl gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the tl instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_tl_read(emu_dc *InstancePtr);
/**
* Read from tlsum gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the tlsum instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_tlsum_read(emu_dc *InstancePtr);
/**
* Read from ia gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the ia instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_ia_read(emu_dc *InstancePtr);
/**
* Read from if_x0 gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the if_x0 instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_if_x0_read(emu_dc *InstancePtr);
/**
* Read from motor_id gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the motor_id instance to operate on.
*
* @return	u32
*
* @note    .
*
*/
u32 emu_dc_motor_id_read(emu_dc *InstancePtr);
/**
* Read from speed gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the speed instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_speed_read(emu_dc *InstancePtr);
/**
* Read from te gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the te instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_te_read(emu_dc *InstancePtr);
/**
* Read from theta_el gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the theta_el instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_theta_el_read(emu_dc *InstancePtr);
/**
* Read from theta_mech gateway of emu_dc. Assignments are LSB-justified.
*
* @param	InstancePtr is the theta_mech instance to operate on.
*
* @return	int
*
* @note    .
*
*/
int emu_dc_theta_mech_read(emu_dc *InstancePtr);
#ifdef __cplusplus
}
#endif
#endif
