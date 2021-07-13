// ==============================================================
// File generated on Mon Jul 12 11:14:21 IST 2021
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xmatrix_mul.h"

extern XMatrix_mul_Config XMatrix_mul_ConfigTable[];

XMatrix_mul_Config *XMatrix_mul_LookupConfig(u16 DeviceId) {
	XMatrix_mul_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XMATRIX_MUL_NUM_INSTANCES; Index++) {
		if (XMatrix_mul_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XMatrix_mul_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XMatrix_mul_Initialize(XMatrix_mul *InstancePtr, u16 DeviceId) {
	XMatrix_mul_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XMatrix_mul_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XMatrix_mul_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

