// ==============================================================
// File generated on Mon Jul 12 11:14:21 IST 2021
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:36:41 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// AXILiteS
// 0x000 : Control signals
//         bit 0  - ap_start (Read/Write/COH)
//         bit 1  - ap_done (Read/COR)
//         bit 2  - ap_idle (Read)
//         bit 3  - ap_ready (Read)
//         bit 7  - auto_restart (Read/Write)
//         others - reserved
// 0x004 : Global Interrupt Enable Register
//         bit 0  - Global Interrupt Enable (Read/Write)
//         others - reserved
// 0x008 : IP Interrupt Enable Register (Read/Write)
//         bit 0  - Channel 0 (ap_done)
//         bit 1  - Channel 1 (ap_ready)
//         others - reserved
// 0x00c : IP Interrupt Status Register (Read/TOW)
//         bit 0  - Channel 0 (ap_done)
//         bit 1  - Channel 1 (ap_ready)
//         others - reserved
// 0x140 : Data signal of write_r
//         bit 0  - write_r[0] (Read/Write)
//         others - reserved
// 0x144 : reserved
// 0x080 ~
// 0x0ff : Memory 'matrix_in_V' (64 * 16b)
//         Word n : bit [15: 0] - matrix_in_V[2n]
//                  bit [31:16] - matrix_in_V[2n+1]
// 0x100 ~
// 0x10f : Memory 'in_vector_V' (8 * 16b)
//         Word n : bit [15: 0] - in_vector_V[2n]
//                  bit [31:16] - in_vector_V[2n+1]
// 0x120 ~
// 0x13f : Memory 'out_vector_V' (8 * 32b)
//         Word n : bit [31:0] - out_vector_V[n]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XMATRIX_MUL_AXILITES_ADDR_AP_CTRL           0x000
#define XMATRIX_MUL_AXILITES_ADDR_GIE               0x004
#define XMATRIX_MUL_AXILITES_ADDR_IER               0x008
#define XMATRIX_MUL_AXILITES_ADDR_ISR               0x00c
#define XMATRIX_MUL_AXILITES_ADDR_WRITE_R_DATA      0x140
#define XMATRIX_MUL_AXILITES_BITS_WRITE_R_DATA      1
#define XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_BASE  0x080
#define XMATRIX_MUL_AXILITES_ADDR_MATRIX_IN_V_HIGH  0x0ff
#define XMATRIX_MUL_AXILITES_WIDTH_MATRIX_IN_V      16
#define XMATRIX_MUL_AXILITES_DEPTH_MATRIX_IN_V      64
#define XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_BASE  0x100
#define XMATRIX_MUL_AXILITES_ADDR_IN_VECTOR_V_HIGH  0x10f
#define XMATRIX_MUL_AXILITES_WIDTH_IN_VECTOR_V      16
#define XMATRIX_MUL_AXILITES_DEPTH_IN_VECTOR_V      8
#define XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_BASE 0x120
#define XMATRIX_MUL_AXILITES_ADDR_OUT_VECTOR_V_HIGH 0x13f
#define XMATRIX_MUL_AXILITES_WIDTH_OUT_VECTOR_V     32
#define XMATRIX_MUL_AXILITES_DEPTH_OUT_VECTOR_V     8

