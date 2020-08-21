#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846
#define N 10 //Number of spins
#define N_iter 50 //Number of iterations
uint32_t buff_size = 20; //Number of samples per spin

void *acquisition_handler(void *);



void *acquisition_handler(void *inp_ptr)
{
	rp_acq_trig_state_t state;
	int j,k;
	float x_k;
    	// buffer array
    	float *buff = (float *)malloc(buff_size * sizeof(float));
    	
	//type casting the relevant pointer where the data is stored
	float *data;
	data = (float *) inp_ptr;

	
	//Capturing N spins	
	for(j = 0; j<N; j++)
	{
        	
		//Figure out the trigger
		//Figure out the time taken by the trigger block
		//Don't miss samples
		rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
        	state = RP_TRIG_STATE_TRIGGERED;

        	do
        	{
        	    	rp_AcqGetTriggerState(&state);
            		printf("State = %d\n", state);
        	}while(state != RP_TRIG_STATE_TRIGGERED);

        	// Get data into buff
	        rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);

        	/*
		// Average over the buffer size
        	for(i = 0; i < buff_size; i++)
	        {
        	    //printf("%f\n", buff[i]);
	            x_k += buff[i];
        	}
	        x_k /= buff_size;
		*/

		//Taking the last value in the 20 samples
		x_k = buff[19];
	
        	//printf("x_k = %f \n", x_k);
	
		//Store it in the input array
		data[j] = x_k;
	}
	free(buff);
}

void output_gen(float *x_out)
{
	int j,k;
	float *buff = (float *)malloc(buff_size * sizeof(float));
	//Put the output spin by spin
	for(j=0;j<N;j++)
	{
	
        	// Store the value in the buffer to be given as output for the next
	        // buff_size cycles
		for(k=0;k<buff_size;k++)
		{
			buff[k] = x_out[j];
		}	
	        
		//Check whether burst works with this
		rp_GenArbWaveform(RP_CH_2, buff, buff_size);

        	// rp_GenTrigger(RP_CH_2);
	        rp_GenOutEnable(RP_CH_2);
	}
	free(buff);
}

int main (int argc, char **argv) 
{
	pthread_t acquisition_thread;
    	//FILE *fp;
    	//fp = fopen("./out_data/x_n.csv", "w+");
    	// Initialization of API
	if (rp_Init() != RP_OK) {
        	fprintf(stderr, "Red Pitaya API init failed!\n");
        	return EXIT_FAILURE;
    	}

    	// Initialise variables

   	// x_k is the "current" value of the photovoltage
    	float x_k = 0.0;

    	// a - self feedback factor
    	float a = 1.1;

    	// b - coupling strength
    	float b = 1;
	
    	//J is the coupling array
    	float *J = (float *)malloc(N * N * sizeof(float));

    	// x_in is an array that will store the input
    	float *x_in = (float *)malloc(N * sizeof(float));

    	// f is the array that stores the feedback term
    	float *f = (float *) malloc(N * sizeof(float));

    	// x_out is the array that stores the output
    	float *x_out = (float *)malloc(N * sizeof(float));

    	// buff stores the input
     	float *buff = (float *)malloc(buff_size * sizeof(float));

     	float next;

    	int i,j,k;

     	//initialize J (J is diagonal as of now)
     	for(i = 0;i<N; i++)
     	{
		for(j = 0;j<N;j++)
		{
			if(i==j)
			{
				*(J + i*N + j) = 0 ;
			}
			else
			{
				*(J + i*N + j) = 0 ;
			}	
		} 
     	}

     	//initialize f 
     	for(i=0;i<N;i++)
    	{
		f[i] = 0;
     	}

    	//initialize x_out
    	for(i=0;i<N;i++)
    	{
		x_out[i] = 0;
    	}	

    	rp_acq_trig_state_t state;

    	rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
    	rp_GenAmp(RP_CH_2, 1);

    	// Enable burst mode
    	rp_GenMode(RP_CH_2, RP_GEN_MODE_BURST);
    	// One waveform per burst
    	rp_GenBurstCount(RP_CH_2, 1);
    	// Number of bursts
    	rp_GenBurstRepetitions(RP_CH_2, -1);
    	// Burst period. Will be dependent on computation time
    	rp_GenBurstPeriod(RP_CH_2, 5000);

    	rp_AcqReset();
    	rp_AcqSetDecimation(1);
    	rp_AcqSetTriggerDelay(0);

    	rp_AcqStart();

    	// After acquisition is started some time delay is needed in order to 
    	// acquire fresh samples in to buffer
    	// Here we have used time delay of one second but you can calculate exact
    	// value taking in to account buffer length and sampling rate

    	sleep(1);

    	for(i = 0;i<N_iter;i++)
    	//Loops over N iterations
    	{
		sleep(1);
		//Wait for 1 second before each iteration
		printf("Iteration number: %d\n",i);
	
		//Start the acquisition thread
		pthread_create(&acquisition_thread, NULL, acquisition_handler, (void *) x_in);
	
		//Simulataneously start putting out the feedback
		output_gen(x_out);
		
		//Make sure the acquisition thread is complete
		pthread_join(acquisition_thread, NULL);
		

        	// Calculating the Feedback array
		for(j=0;j<N;j++)
		{
			f[j] = a*x_in[j];
			for(k=0;k<N;k++)
			{
				f[j]+= (*(J + j*N +k)) * b * x_in[k];
			}
		}
	
		//Simulating the modulator's action	
		for(j=0;j<N;j++)
		{
			x_out[j] = pow(cos(f[j] - (0.25*M_PI)),2) - 0.5;
		}
	
		for(j=0;j<N;j++)
		{
			printf("x_in_%d [%d] = %f x_out_%d [%d] = %f\n",i,j,x_in[j],i+1,j,x_out[j]);
		}
    	}	

   	// Releasing resources
    	free(x_in);
    	free(x_out);
    	free(f);
    	free(J);
    	free(buff);
    	rp_Release();
    	//fclose(fp);

   	return EXIT_SUCCESS;
}
