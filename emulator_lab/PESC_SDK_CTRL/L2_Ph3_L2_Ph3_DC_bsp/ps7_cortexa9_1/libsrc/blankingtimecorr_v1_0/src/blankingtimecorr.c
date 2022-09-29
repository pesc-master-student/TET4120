#include "blankingtimecorr.h"
#ifndef __linux__
int blankingtimecorr_CfgInitialize(blankingtimecorr *InstancePtr, blankingtimecorr_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->blankingtimecorr_BaseAddress = ConfigPtr->blankingtimecorr_BaseAddress;

    InstancePtr->IsReady = 1;
    return XST_SUCCESS;
}
#endif
void blankingtimecorr_ic_in_write(blankingtimecorr *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    blankingtimecorr_WriteReg(InstancePtr->blankingtimecorr_BaseAddress, 0, Data);
}
int blankingtimecorr_ic_in_read(blankingtimecorr *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 0);
    return Data;
}
void blankingtimecorr_ib_in_write(blankingtimecorr *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    blankingtimecorr_WriteReg(InstancePtr->blankingtimecorr_BaseAddress, 4, Data);
}
int blankingtimecorr_ib_in_read(blankingtimecorr *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 4);
    return Data;
}
void blankingtimecorr_ia_in_write(blankingtimecorr *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    blankingtimecorr_WriteReg(InstancePtr->blankingtimecorr_BaseAddress, 8, Data);
}
int blankingtimecorr_ia_in_read(blankingtimecorr *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 8);
    return Data;
}
void blankingtimecorr_blankingtime_write(blankingtimecorr *InstancePtr, int Data) {

    Xil_AssertVoid(InstancePtr != NULL);

    blankingtimecorr_WriteReg(InstancePtr->blankingtimecorr_BaseAddress, 12, Data);
}
int blankingtimecorr_blankingtime_read(blankingtimecorr *InstancePtr) {

    int Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 12);
    return Data;
}
u32 blankingtimecorr_t_samp_on_read(blankingtimecorr *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 16);
    return Data;
}
u32 blankingtimecorr_ta_on_read(blankingtimecorr *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 20);
    return Data;
}
u32 blankingtimecorr_tb_on_read(blankingtimecorr *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 24);
    return Data;
}
u32 blankingtimecorr_tc_on_read(blankingtimecorr *InstancePtr) {

    u32 Data;
    Xil_AssertVoid(InstancePtr != NULL);

    Data = blankingtimecorr_ReadReg(InstancePtr->blankingtimecorr_BaseAddress, 28);
    return Data;
}
