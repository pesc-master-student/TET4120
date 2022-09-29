/**
* @file emu_dc_sinit.c
*
* The implementation of the emu_dc driver's static initialzation
* functionality.
*
* @note
*
* None
*
*/
#ifndef __linux__
#include "xstatus.h"
#include "xparameters.h"
#include "emu_dc.h"
extern emu_dc_Config emu_dc_ConfigTable[];
/**
* Lookup the device configuration based on the unique device ID.  The table
* ConfigTable contains the configuration info for each device in the system.
*
* @param DeviceId is the device identifier to lookup.
*
* @return
*     - A pointer of data type emu_dc_Config which
*    points to the device configuration if DeviceID is found.
*    - NULL if DeviceID is not found.
*
* @note    None.
*
*/
emu_dc_Config *emu_dc_LookupConfig(u16 DeviceId) {
    emu_dc_Config *ConfigPtr = NULL;
    int Index;
    for (Index = 0; Index < XPAR_EMU_DC_NUM_INSTANCES; Index++) {
        if (emu_dc_ConfigTable[Index].DeviceId == DeviceId) {
            ConfigPtr = &emu_dc_ConfigTable[Index];
            break;
        }
    }
    return ConfigPtr;
}
int emu_dc_Initialize(emu_dc *InstancePtr, u16 DeviceId) {
    emu_dc_Config *ConfigPtr;
    Xil_AssertNonvoid(InstancePtr != NULL);
    ConfigPtr = emu_dc_LookupConfig(DeviceId);
    if (ConfigPtr == NULL) {
        InstancePtr->IsReady = 0;
        return (XST_DEVICE_NOT_FOUND);
    }
    return emu_dc_CfgInitialize(InstancePtr, ConfigPtr);
}
#endif
