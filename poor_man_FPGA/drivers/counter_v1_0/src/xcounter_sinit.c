// ==============================================================
// File generated on Fri Jul 09 18:39:28 IST 2021
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xcounter.h"

extern XCounter_Config XCounter_ConfigTable[];

XCounter_Config *XCounter_LookupConfig(u16 DeviceId) {
	XCounter_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XCOUNTER_NUM_INSTANCES; Index++) {
		if (XCounter_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XCounter_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XCounter_Initialize(XCounter *InstancePtr, u16 DeviceId) {
	XCounter_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XCounter_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XCounter_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

