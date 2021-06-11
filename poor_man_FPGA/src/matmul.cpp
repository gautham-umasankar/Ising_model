#include "matmul.h"

void matrix_mul(dt_mat matrix_in[N][N], dt_in in_vector[N], dt_out out_vector[N], bool write){
	static short temp[N];
	// double buffering
	// static dt_mat matrix[N][N]; add port to write data in.
	// use axilite for that to comm with arm
	//

	static dt_mat matrix[N][N];
	if(write == 1){
		matrix_write:for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				matrix[i][j] = matrix_in[i][j];
			}
		}
	}

	else{
		mod:for(int k=0;k<(int)n;k++){
			Column:for(int j=0;j<N;j++){
				Row:for(int i=0;i<num_units;i++){
						temp[k*num_units + i] += matrix[k*num_units + i][j]*in_vector[j];
					}
			}
		}

		write_out:for(int i=0;i<N;i++){
			out_vector[i] = temp[i];
			temp[i] = 0;
		}
	}
}
