#include <cstdio>
#include <iostream>
#include <fstream>

#include "matmul.h"

int main(){
	ifstream fp;
    dt_mat matrix[N][N];
    dt_in in_vector[N];
    dt_out out_vector[N];
    dt_in inputRead;

    cout<<"N = "<<N<<"\n";
    fp.open("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/mat_file_fp.txt",ios::in);
//    fp = fopen("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/mat_file_fp.txt", "r");
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
        	fp >> matrix[i][j];
//			cout<<matrix[i][j]<<" ";
//			printf("%f ",matrix[i][j].to_float());
        }
//        cout<<"\n";
    }
    matrix_mul(matrix ,in_vector , out_vector, 1);

    fp.close();
    fp.open("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/in_vec_file_fp.txt",ios::in);
//    fp = fopen("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/in_vec_file_fp.txt", "r");

    for(int i=0;i<N;i++){
    	fp >> inputRead ;
    	in_vector[i] = inputRead;
//    	cout<<in_vector[i]<<"\n";
    }

    matrix_mul(matrix ,in_vector , out_vector, 0);

    fp.close();
    fp.open("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/out_vec_file_fp.txt",ios::in);
//    fp = fopen("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/out_vec_file_fp.txt", "r");
    int num_errors = 0;
    dt_out expected_el;
    for(int i=0;i<N;i++){
    	fp >> expected_el;
//        cout<<out_vector[i]<<" vs expected = "<<expected_el<<"\n";
        if(expected_el != out_vector[i]){
            num_errors++;
        }
    }
    fp.close();

    if(num_errors == 0){
        printf("Matrix Multiplication passed.\n");
    }
    else{
        printf("Failed for %d elements.\n", num_errors);
        return 1;
    }
    return 0;
}
