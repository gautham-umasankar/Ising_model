// ==============================================================
// File generated on Mon Jul 12 11:14:21 IST 2021
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XMATRIX_MUL_H
#define XMATRIX_MUL_H

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
#include "xmatrix_mul_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Axilites_BaseAddress;
} XMatrix_mul_Config;
#endif

typedef struct {
    u32 Axilites_BaseAddress;
    u32 IsReady;
} XMatrix_mul;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XMatrix_mul_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XMatrix_mul_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XMatrix_mul_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XMatrix_mul_ReadReg(BaseAddress, RegOffset) \
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
int XMatrix_mul_Initialize(XMatrix_mul *InstancePtr, u16 DeviceId);
XMatrix_mul_Config* XMatrix_mul_LookupConfig(u16 DeviceId);
int XMatrix_mul_CfgInitialize(XMatrix_mul *InstancePtr, XMatrix_mul_Config *ConfigPtr);
#else
int XMatrix_mul_Initialize(XMatrix_mul *InstancePtr, const char* InstanceName);
int XMatrix_mul_Release(XMatrix_mul *InstancePtr);
#endif

void XMatrix_mul_Start(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_IsDone(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_IsIdle(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_IsReady(XMatrix_mul *InstancePtr);
void XMatrix_mul_EnableAutoRestart(XMatrix_mul *InstancePtr);
void XMatrix_mul_DisableAutoRestart(XMatrix_mul *InstancePtr);

void XMatrix_mul_Set_write_r(XMatrix_mul *InstancePtr, u32 Data);
u32 XMatrix_mul_Get_write_r(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_matrix_in_V_BaseAddress(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_matrix_in_V_HighAddress(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_matrix_in_V_TotalBytes(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_matrix_in_V_BitWidth(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_matrix_in_V_Depth(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Write_matrix_in_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length);
u32 XMatrix_mul_Read_matrix_in_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length);
u32 XMatrix_mul_Write_matrix_in_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length);
u32 XMatrix_mul_Read_matrix_in_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length);
u32 XMatrix_mul_Get_in_vector_V_BaseAddress(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_in_vector_V_HighAddress(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_in_vector_V_TotalBytes(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_in_vector_V_BitWidth(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_in_vector_V_Depth(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Write_in_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length);
u32 XMatrix_mul_Read_in_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length);
u32 XMatrix_mul_Write_in_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length);
u32 XMatrix_mul_Read_in_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length);
u32 XMatrix_mul_Get_out_vector_V_BaseAddress(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_out_vector_V_HighAddress(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_out_vector_V_TotalBytes(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_out_vector_V_BitWidth(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Get_out_vector_V_Depth(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_Write_out_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length);
u32 XMatrix_mul_Read_out_vector_V_Words(XMatrix_mul *InstancePtr, int offset, int *data, int length);
u32 XMatrix_mul_Write_out_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length);
u32 XMatrix_mul_Read_out_vector_V_Bytes(XMatrix_mul *InstancePtr, int offset, char *data, int length);

void XMatrix_mul_InterruptGlobalEnable(XMatrix_mul *InstancePtr);
void XMatrix_mul_InterruptGlobalDisable(XMatrix_mul *InstancePtr);
void XMatrix_mul_InterruptEnable(XMatrix_mul *InstancePtr, u32 Mask);
void XMatrix_mul_InterruptDisable(XMatrix_mul *InstancePtr, u32 Mask);
void XMatrix_mul_InterruptClear(XMatrix_mul *InstancePtr, u32 Mask);
u32 XMatrix_mul_InterruptGetEnabled(XMatrix_mul *InstancePtr);
u32 XMatrix_mul_InterruptGetStatus(XMatrix_mul *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
