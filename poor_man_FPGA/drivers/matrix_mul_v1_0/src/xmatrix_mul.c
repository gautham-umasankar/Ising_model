// ==============================================================
// File generated on Mon Jul 12 11:14:21 IST 2021
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xmatrix_mul.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XMatrix_mul_CfgInitialize(XMatrix_mul *InstancePtr, XMatrix_mul_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axilites_BaseAddress = ConfigPtr->Axilites_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XMatrix_mul_Start(XMatrix_mul *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_AP_CTRL) & 0x80;
    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_AP_CTRL, Data | 0x01);
}

u32 XMatrix_mul_IsDone(XMatrix_mul *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XMatrix_mul_IsIdle(XMatrix_mul *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XMatrix_mul_IsReady(XMatrix_mul *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XMatrix_mul_EnableAutoRestart(XMatrix_mul *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_AP_CTRL, 0x80);
}

void XMatrix_mul_DisableAutoRestart(XMatrix_mul *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_AP_CTRL, 0);
}

void XMatrix_mul_Set_write_r(XMatrix_mul *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_WRITE_R_DATA, Data);
}

u32 XMatrix_mul_Get_write_r(XMatrix_mul *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_WRITE_R_DATA);
    return Data;
}

u32 XMatrix_mul_Get_matrix_in_V_BaseAddress(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE);
}

u32 XMatrix_mul_Get_matrix_in_V_HighAddress(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_HIGH);
}

u32 XMatrix_mul_Get_matrix_in_V_TotalBytes(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + 1);
}

u32 XMatrix_mul_Get_matrix_in_V_BitWidth(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMATRIX_MUL_AXILITES_WIDTH_MATRIX_IN_V;
}

u32 XMatrix_mul_Get_matrix_in_V_Depth(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMATRIX_MUL_AXILITES_DEPTH_MATRIX_IN_V;
}

u32 XMatrix_mul_Write_matrix_in_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XMatrix_mul_Read_matrix_in_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + (offset + i)*4);
    }
    return length;
}

u32 XMatrix_mul_Write_matrix_in_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XMatrix_mul_Read_matrix_in_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE + offset + i);
    }
    return length;
}

u32 XMatrix_mul_Get_in_vector_V_BaseAddress(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE);
}

u32 XMatrix_mul_Get_in_vector_V_HighAddress(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_HIGH);
}

u32 XMatrix_mul_Get_in_vector_V_TotalBytes(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + 1);
}

u32 XMatrix_mul_Get_in_vector_V_BitWidth(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMATRIX_MUL_AXILITES_WIDTH_IN_VECTOR_V;
}

u32 XMatrix_mul_Get_in_vector_V_Depth(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMATRIX_MUL_AXILITES_DEPTH_IN_VECTOR_V;
}

u32 XMatrix_mul_Write_in_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XMatrix_mul_Read_in_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + (offset + i)*4);
    }
    return length;
}

u32 XMatrix_mul_Write_in_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XMatrix_mul_Read_in_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE + offset + i);
    }
    return length;
}

u32 XMatrix_mul_Get_out_vector_V_BaseAddress(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE);
}

u32 XMatrix_mul_Get_out_vector_V_HighAddress(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_HIGH);
}

u32 XMatrix_mul_Get_out_vector_V_TotalBytes(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + 1);
}

u32 XMatrix_mul_Get_out_vector_V_BitWidth(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMATRIX_MUL_AXILITES_WIDTH_OUT_VECTOR_V;
}

u32 XMatrix_mul_Get_out_vector_V_Depth(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMATRIX_MUL_AXILITES_DEPTH_OUT_VECTOR_V;
}

u32 XMatrix_mul_Write_out_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XMatrix_mul_Read_out_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + (offset + i)*4);
    }
    return length;
}

u32 XMatrix_mul_Write_out_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XMatrix_mul_Read_out_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_HIGH - XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axilites_BaseAddress + XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE + offset + i);
    }
    return length;
}

void XMatrix_mul_InterruptGlobalEnable(XMatrix_mul *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_GIE, 1);
}

void XMatrix_mul_InterruptGlobalDisable(XMatrix_mul *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_GIE, 0);
}

void XMatrix_mul_InterruptEnable(XMatrix_mul *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_IER);
    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_IER, Register | Mask);
}

void XMatrix_mul_InterruptDisable(XMatrix_mul *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_IER);
    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_IER, Register & (~Mask));
}

void XMatrix_mul_InterruptClear(XMatrix_mul *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XMatrix_mul_WriteReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_ISR, Mask);
}

u32 XMatrix_mul_InterruptGetEnabled(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_IER);
}

u32 XMatrix_mul_InterruptGetStatus(XMatrix_mul *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XMatrix_mul_ReadReg(InstancePtr->Axilites_BaseAddress, XMATRIX_MUL_AXILITES_ADDR_ISR);
}

