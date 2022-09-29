#include "emu_dc.h"
#ifndef __linux__
int emu_dc_CfgInitialize(emu_dc *InstancePtr, emu_dc_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->emu_dc_BaseAddress = ConfigPtr->emu_dc_BaseAddress;

    InstancePtr->IsReady = 1;
    return XST_SUCCESS;
}
#endif
void emu_dc_voltage_pu_bit_conv2_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 0, Data);
}
int emu_dc_voltage_pu_bit_conv2_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 0);
    return Data;
}
void emu_dc_voltage_pu_bit_conv1_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 4, Data);
}
int emu_dc_voltage_pu_bit_conv1_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 4);
    return Data;
}
void emu_dc_ra_write(emu_dc *InstancePtr, u32 Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 8, Data);
}
u32 emu_dc_ra_read(emu_dc *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 8);
    return Data;
}
void emu_dc_polepairs_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 12, Data);
}
int emu_dc_polepairs_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 12);
    return Data;
}
void emu_dc_magnetization_4q_1q_write(emu_dc *InstancePtr, u32 Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 16, Data);
}
u32 emu_dc_magnetization_4q_1q_read(emu_dc *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 16);
    return Data;
}
void emu_dc_kn_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 20, Data);
}
int emu_dc_kn_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 20);
    return Data;
}
void emu_dc_k_fnmechtstep_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 24, Data);
}
int emu_dc_k_fnmechtstep_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 24);
    return Data;
}
void emu_dc_current_pu_bit_conv2_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 28, Data);
}
int emu_dc_current_pu_bit_conv2_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 28);
    return Data;
}
void emu_dc_current_pu_bit_conv1_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 32, Data);
}
int emu_dc_current_pu_bit_conv1_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 32);
    return Data;
}
void emu_dc_vdc2_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 36, Data);
}
int emu_dc_vdc2_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 36);
    return Data;
}
void emu_dc_vdc1_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 40, Data);
}
int emu_dc_vdc1_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 40);
    return Data;
}
void emu_dc_ts_la_write(emu_dc *InstancePtr, u32 Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 44, Data);
}
u32 emu_dc_ts_la_read(emu_dc *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 44);
    return Data;
}
void emu_dc_ts_tm_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 48, Data);
}
int emu_dc_ts_tm_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 48);
    return Data;
}
void emu_dc_ts_tf_write(emu_dc *InstancePtr, u32 Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 52, Data);
}
u32 emu_dc_ts_tf_read(emu_dc *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 52);
    return Data;
}
void emu_dc_tl_write(emu_dc *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    emu_dc_WriteReg(InstancePtr->emu_dc_BaseAddress, 56, Data);
}
int emu_dc_tl_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 56);
    return Data;
}
int emu_dc_tlsum_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 60);
    return Data;
}
int emu_dc_ia_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 64);
    return Data;
}
int emu_dc_if_x0_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 68);
    return Data;
}
u32 emu_dc_motor_id_read(emu_dc *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 72);
    return Data;
}
int emu_dc_speed_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 76);
    return Data;
}
int emu_dc_te_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 80);
    return Data;
}
int emu_dc_theta_el_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 84);
    return Data;
}
int emu_dc_theta_mech_read(emu_dc *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = emu_dc_ReadReg(InstancePtr->emu_dc_BaseAddress, 88);
    return Data;
}
