#include <cstdio>
#include <iostream>
#include <fstream>
#include <chrono>

#include "matmul.h"
#include "xmatrix_mul.h"
#include "xcounter.h"
//#include "xmatrix_mul_hw.h"

int main(){

    int unsigned period = 1000000; // uS

	ifstream fp;
    dt_mat matrix[N][N];
    dt_in in_vector[N];
    dt_out out_vector[N];

    float cpu_matrix[N][N];
    float cpu_in_vector[N];
    float cpu_out_vector[N];

    dt_in inputRead;
    XMatrix_mul mml;
    XMatrix_mul *ptr = &mml;
	XCounter xc;
	XCounter *xptr = &xc;

	XCounter_Initialize(xptr,"api");
    XMatrix_mul_Initialize(ptr, "api");
	printf("Initialisation done.\n");
	XCounter_EnableAutoRestart(xptr);

    using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;


    printf("Gonna try the write thingy\n");
    int x = XMatrix_mul_Get_write_r(ptr);
    printf("write_r before trying to write = %d\n",x);
    XMatrix_mul_Set_write_r(ptr,1);

    x = XMatrix_mul_Get_write_r(ptr);
    printf("write_r after trying to write = %d\n",x);

    cout<<"N = "<<N<<"\n";
    fp.open("../testcases/mat_file_fp.txt",ios::in);
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
        	fp >> matrix[i][j];
        	cpu_matrix[i][j] = matrix[i][j].to_float();
//			cout<<matrix[i][j]<<" ";
			printf("%f ",matrix[i][j].to_float());
        }
        cout<<"\n";
    }
//    hello();
    XMatrix_mul_Write_matrix_in_V_Words(ptr, 0, (int*)matrix, N*sizeof(int));
    dt_mat writ[N][N];
    XMatrix_mul_Read_matrix_in_V_Words(ptr, 0, (int*)writ, N*sizeof(int));
    printf("Reading matrix\n");
    for(int i=0;i<N;i++){
            for(int j=0;j<N;j++){
    			printf("%f ",writ[i][j].to_float());
            }
            cout<<"\n";
        }
//    matrix_mul(matrix ,in_vector , out_vector, 1);

    fp.close();
    fp.open("../testcases/in_vec_file_fp.txt",ios::in);

    for(int i=0;i<N;i++){
    	fp >> inputRead ;
    	in_vector[i] = inputRead;
    	cpu_in_vector[i] = in_vector[i].to_float();
    	cout<<in_vector[i]<<"\n";
    }

    XMatrix_mul_Write_in_vector_V_Words(ptr, 0, (int *)in_vector, sizeof(int));

    dt_in inv[N];
    XMatrix_mul_Read_in_vector_V_Words(ptr, 0, (int *)inv, sizeof(int));

    printf("Sizeof(int)=%d,reading in vector\n",sizeof(int));
    for(int i=0;i<N;i++){
        	cout<<inv[i]<<"\n";
        }

    XMatrix_mul_Start(ptr);
    while(!XMatrix_mul_IsDone(ptr)){
    	printf("Waiting to write into matrix...\n");
    }

    XMatrix_mul_Set_write_r(ptr,0);
    auto t1 = high_resolution_clock::now();
    XMatrix_mul_Start(ptr);

    while(!XMatrix_mul_IsDone(ptr)){
    	printf("Waiting for multiplication to finish...\n");
    }
    auto t2 = high_resolution_clock::now();
    XMatrix_mul_Read_out_vector_V_Words(ptr, 0, (int *)out_vector, 2*sizeof(int));

	duration<double, std::milli> ms_double = t2 - t1;

	printf("Time taken by fpga in double:%lf ms\n",ms_double.count());
//    matrix_mul(matrix ,in_vector , out_vector, 0);

	t1 = high_resolution_clock::now();
	for(int i=0;i<N;i++){
		cpu_out_vector[i] = 0;
		for(int j=0;j<N;j++){
			cpu_out_vector[i] += cpu_matrix[i][j]*cpu_in_vector[j];
		}
	}
	t2 = high_resolution_clock::now();
	ms_double = t2 - t1;

	printf("Time taken by software in double:%lf ms\n",ms_double.count());

    fp.close();
    fp.open("../testcases/out_vec_file_fp.txt",ios::in);
    int num_errors = 0;
    for(int i=0;i<N;i++){
//    	fp >> cpu_out_vector_el;
        cout<<out_vector[i]<<" vs cpu_out_vector = "<<cpu_out_vector[i]<<"\n";
        if(cpu_out_vector[i] != out_vector[i].to_float()){
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
