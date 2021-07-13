// ==============================================================
// File generated on Fri Jul 09 18:39:28 IST 2021
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xcounter.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XCounter_CfgInitialize(XCounter *InstancePtr, XCounter_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axilites_BaseAddress = ConfigPtr->Axilites_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XCounter_Start(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_CTRL) & 0x80;
    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_CTRL, Data | 0x01);
}

u32 XCounter_IsDone(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XCounter_IsIdle(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XCounter_IsReady(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XCounter_EnableAutoRestart(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_CTRL, 0x80);
}

void XCounter_DisableAutoRestart(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_CTRL, 0);
}

u32 XCounter_Get_return(XCounter *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_AP_RETURN);
    return Data;
}
void XCounter_InterruptGlobalEnable(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_GIE, 1);
}

void XCounter_InterruptGlobalDisable(XCounter *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_GIE, 0);
}

void XCounter_InterruptEnable(XCounter *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_IER);
    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_IER, Register | Mask);
}

void XCounter_InterruptDisable(XCounter *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_IER);
    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_IER, Register & (~Mask));
}

void XCounter_InterruptClear(XCounter *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XCounter_WriteReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_ISR, Mask);
}

u32 XCounter_InterruptGetEnabled(XCounter *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_IER);
}

u32 XCounter_InterruptGetStatus(XCounter *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XCounter_ReadReg(InstancePtr->Axilites_BaseAddress, XCOUNTER_AXILITES_ADDR_ISR);
}

