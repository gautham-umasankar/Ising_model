#include "matmul.h"

dt_out dot_product(dt_mat matrix[N*N], dt_in in_vector[N], dt_out out_vector[N], unsigned short int i){
	dt_out temp = 0;
	Column:for(unsigned short int j=0;j<N;j++){
		temp += matrix[i + j]*in_vector[j];
	}
	return temp;
}

void matrix_mul(dt_mat matrix[N*N], dt_in in_vector[N], dt_out out_vector[N]){
	Row:for(unsigned short int i=0;i<N;i++){
		out_vector[i] = dot_product(matrix, in_vector, out_vector, i*N);
	}
}
