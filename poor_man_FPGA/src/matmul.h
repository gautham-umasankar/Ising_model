#ifndef __MATMUL_H__
#define __MATMUL_H__

#include "/tools/Xilinx/Vivado/2018.3/include/gmp.h"
#include <ap_fixed.h>
//#include <iostream>

#define N 64 //Input and Output array Size
#define num_units 16 // Number of dot product units
#define n N/num_units // N/num_units
#define TOL 0.01 //Tolerence in the accuracy of output

//typedef ap_fixed<17,7> fixed_177;
typedef short int dt_in; //for fixed point
typedef short int dt_out;
typedef short int dt_mat;

using namespace std;

// matmul function definition
void matrix_mul(dt_mat matrix[N][N] , dt_in in_vector[N], dt_out out_vector[N]);

#endif
