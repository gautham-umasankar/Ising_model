#include <cstdio>
#include <iostream>

#include "matmul.h"

int main(){
    FILE *fp;
    dt_mat matrix[N*N];
    dt_in in_vector[N];
    dt_out out_vector[N];

    fp = fopen("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/mat_file.txt", "r");
    if(fp == NULL){
    	cout<<"Couldn't open input matrix file!\n";
    	return 1;
    }
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            fscanf(fp, "%d", &matrix[i*N+j]);
            cout<<matrix[i*N+j]<<" ";
        }
        cout<<"\n";
    }
    fclose(fp);

    fp = fopen("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/in_vec_file.txt", "r");
    if(fp == NULL){
    	cout<<"Couldn't open input vector file!\n";
    	return 1;
    }
    for(int i=0;i<N;i++){
        fscanf(fp, "%d", &in_vector[i]);
        cout<<in_vector[i]<<"\n";
    }
    fclose(fp);

    matrix_mul(matrix ,in_vector , out_vector);

    fp = fopen("/media/parsidd/G_drive/College/QCQI/Project_work/Ising_model/poor_man_FPGA/testcases/out_vec_file.txt", "r");
    if(fp == NULL){
    	cout<<"Couldn't open output file!\n";
    	return 1;
    }
    int expected_el, num_errors = 0;
    for(int i=0;i<N;i++){
        fscanf(fp, "%d", &expected_el);
        cout<<out_vector[i]<<" vs expected = "<<expected_el<<"\n";
        if(expected_el != out_vector[i]){
            num_errors++;
        }
    }
    fclose(fp);
    
    if(num_errors == 0){
        printf("Matrix Multiplication passed.\n");
    }
    else{
        printf("Failed for %d elements.\n", num_errors);
        return 1;
    }
    return 0;
}
