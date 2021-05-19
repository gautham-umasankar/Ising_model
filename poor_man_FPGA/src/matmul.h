#ifndef __MATMUL_H__
#define __MATMUL_H__

#include "/tools/Xilinx/Vivado/2018.3/include/gmp.h"
#include <ap_fixed.h>
//#include <iostream>

#define N 3 //Input and Output array Size
#define TOL 0.01 //Tolerence in the accuracy of output

//typedef ap_fixed<17,7> fixed_177;
typedef short int dt_in; //for fixed point
typedef short int dt_out;
typedef short int dt_mat;

using namespace std;

// matmul function definition
void matrix_mul(dt_mat* , dt_in* , dt_out*);

#endif
