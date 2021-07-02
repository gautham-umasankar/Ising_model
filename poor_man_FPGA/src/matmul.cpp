#include "matmul.h"
void hello(){
	printf("Hello\n");
}

void matrix_mul(dt_mat matrix_in[N][N], dt_in in_vector[N], dt_out out_vector[N], bool write_r){
	#pragma HLS INTERFACE s_axilite port=write_r
	#pragma HLS INTERFACE s_axilite port=out_vector
	#pragma HLS INTERFACE s_axilite port=in_vector
	#pragma HLS INTERFACE s_axilite port=matrix_in
	#pragma HLS INTERFACE s_axilite port=return
	static dt_out temp[N];
	#pragma HLS ARRAY_PARTITION variable=temp complete dim=1
	// double buffering
	// static dt_mat matrix[N][N]; add port to write data in.
	// use axilite for that to comm with arm
	//

	static dt_mat matrix[N][N];
	#pragma HLS ARRAY_PARTITION variable=matrix complete dim=1
	#pragma HLS RESOURCE variable=matrix core=RAM_2P_BRAM
	if(write_r == 1){
		matrix_write:for(int i=0;i<N;i++){
		#pragma HLS PIPELINE
			for(int j=0;j<N;j++){
				matrix[i][j] = matrix_in[i][j];
			}
		}
	}

	else{
		mod:for(int k=0;k<(int)n;k++){
			Column:for(int j=0;j<N;j++){
				#pragma HLS PIPELINE
				Row:for(int i=0;i<num_units;i++){
						temp[k*num_units + i] += matrix[k*num_units + i][j]*in_vector[j];
					}
			}
		}

		write_out:for(int i=0;i<N;i++){
			#pragma HLS PIPELINE
			out_vector[i] = temp[i];
			temp[i] = 0;
		}
	}
}
