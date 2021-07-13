// ==============================================================
// File generated on Fri Jul 09 18:39:28 IST 2021
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XCOUNTER_H
#define XCOUNTER_H

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
#include "xcounter_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Axilites_BaseAddress;
} XCounter_Config;
#endif

typedef struct {
    u32 Axilites_BaseAddress;
    u32 IsReady;
} XCounter;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XCounter_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XCounter_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XCounter_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XCounter_ReadReg(BaseAddress, RegOffset) \
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
int XCounter_Initialize(XCounter *InstancePtr, u16 DeviceId);
XCounter_Config* XCounter_LookupConfig(u16 DeviceId);
int XCounter_CfgInitialize(XCounter *InstancePtr, XCounter_Config *ConfigPtr);
#else
int XCounter_Initialize(XCounter *InstancePtr, const char* InstanceName);
int XCounter_Release(XCounter *InstancePtr);
#endif

void XCounter_Start(XCounter *InstancePtr);
u32 XCounter_IsDone(XCounter *InstancePtr);
u32 XCounter_IsIdle(XCounter *InstancePtr);
u32 XCounter_IsReady(XCounter *InstancePtr);
void XCounter_EnableAutoRestart(XCounter *InstancePtr);
void XCounter_DisableAutoRestart(XCounter *InstancePtr);
u32 XCounter_Get_return(XCounter *InstancePtr);


void XCounter_InterruptGlobalEnable(XCounter *InstancePtr);
void XCounter_InterruptGlobalDisable(XCounter *InstancePtr);
void XCounter_InterruptEnable(XCounter *InstancePtr, u32 Mask);
void XCounter_InterruptDisable(XCounter *InstancePtr, u32 Mask);
void XCounter_InterruptClear(XCounter *InstancePtr, u32 Mask);
u32 XCounter_InterruptGetEnabled(XCounter *InstancePtr);
u32 XCounter_InterruptGetStatus(XCounter *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
