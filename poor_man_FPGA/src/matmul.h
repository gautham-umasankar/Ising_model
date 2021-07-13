#ifndef __MATMUL_H__
#define __MATMUL_H__

#include "/tools/Xilinx/Vivado/2018.3/include/gmp.h"
#define HALF_ENABLE_CPP11_CMATH 0
#include <ap_fixed.h>
#include <stdio.h>

#define N 8 //Input and Output array Size
#define num_units 8 // Number of dot product units
#define n N/num_units // N/num_units
#define TOL 0.01 //Tolerence in the accuracy of output

 typedef ap_fixed<16, 7> fixed_7_9;
 typedef ap_fixed<32, 14> fixed_14_18;
//typedef short int dt_in; //for integer
//typedef short int dt_out;
//typedef short int dt_mat;
//typedef float dt_in; //for fixed point
//typedef float dt_out;
//typedef float dt_mat;

 typedef fixed_7_9 dt_in; //for fixed point
 typedef fixed_7_9 dt_mat;
 typedef fixed_14_18 dt_out;

using namespace std;

// matmul function definition
void matrix_mul(dt_mat matrix[N][N] , dt_in in_vector[N], dt_out out_vector[N], bool);
void hello();

#endif
