#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>
#include <pthread.h>

#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846
#define BUFFER_SIZE 16*1024
#define sig_f 0.4

int trig_delay = 16384+3900;
int N_spins = 4;
int N_noise = 100;
int p_step = 1000;

int buff_size = BUFFER_SIZE;
int buff_per_spin;

void gen_noise();
//void single_iteration(float, float, int, int);


// x_in stores the input
volatile float *x_in;
volatile float *x_out;
float *noise;


void gen_noise()
{
    // Generate noise with 0 mean and some deviation. Deviation can be changed
    // at run time
	int i = 0;
	srand(time(0));
	float mu = 0.0, sig = 0.0;
	for(;i<N_noise;i++)
	{
		noise[i] = rand()%64;
		mu += noise[i];
		sig += noise[i]*noise[i];
	}
	mu /= N_noise;
	sig /= N_noise;
	sig -= mu*mu;
	sig = sqrt(sig)/sig_f;
	for(i=0;i<N_noise;i++)
	{
		noise[i] = (noise[i] - mu)/sig;
		// printf("\nNoise %d = %f\n",i ,noise[i]);
	}
}


int main (int argc, char **argv) 
{
    printf("Before bus error\n"); 
    x_out = (float *)calloc(buff_size, sizeof(float));
    x_in = (float *)calloc(buff_size, sizeof(float));
    noise = (float *)calloc(N_noise, sizeof(float));

    int i,s,n;
    float value;    
    
    // Initialization of API
    if (rp_Init() != RP_OK) 
    {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }


    //rp_AcqReset();
    //rp_AcqSetDecimation(1);

    gen_noise();

    rp_GenOutEnable(RP_CH_2);

    buff_per_spin = (int)BUFFER_SIZE/N_spins;
  
    for(i = 0;i < N_spins;i++)
    {
        n = rand()%N_noise;
	value = noise[n];
	printf("Value = %f\n",value);
        //Threshold the output
        if(value >= 1.0)
        {
            value = 1.0;
        }
        else if(value<=-1.0)
        {
            value = -1.0;
        }

	for(int j = i*buff_per_spin;j < (i+1)*(buff_per_spin);j++)
        {
            x_out[j] = value;
        }
    }
    for(i = 0;i<BUFFER_SIZE;i+=p_step)
	printf("x_out[%d] = %f\n",i,x_out[i]);

    // Send the output
    /*rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
    rp_GenArbWaveform(RP_CH_2, x_out, buff_size); //Does it start generating here itself?
    // Enable burst mode
    rp_GenMode(RP_CH_2, RP_GEN_MODE_BURST); 
    // One waveform per burst
    rp_GenBurstCount(RP_CH_2, 1);
    // Number of bursts
    rp_GenBurstRepetitions(RP_CH_2, 3);
    // Burst period. Will be dependent on computation time
    //rp_GenBurstPeriod(RP_CH_2, 130000);

    //rp_GenAmp(RP_CH_2, 1.0);
    rp_GenFreq(RP_CH_2, 7690.0);*/

    // Releasing resources
    free(x_out);
    free(x_in);
    free(noise);
    rp_Release();
 
    return EXIT_SUCCESS;
}





//void single_iteration(float alpha, float beta, int s,int iteration)
//{
    /*rp_AcqReset();
    // Start acquisition
    rp_AcqSetTriggerDelay(trig_delay);
    rp_AcqStart();

    // Trigger immediately
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    int pos;
    rp_AcqGetWritePointerAtTrig(&pos);

    //printf("Pos right after trigger now:%d\n",pos);

    rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;    

    // Wait for buffer to get full after Trigger.
    do
    {
        rp_AcqGetTriggerState(&state);
	rp_AcqGetWritePointer(&pos);
        //printf("Pos = %d\n", pos);
    }while(state == RP_TRIG_STATE_TRIGGERED);
	
    rp_AcqGetWritePointer(&pos);
    //printf("Pos = %d\n",pos);
    // Get data into buff
    rp_AcqGetOldestDataV(RP_CH_1, &buff_size, x_in);

    // Stop acquisition
    rp_AcqStop();

    //Reset the output to zero
    //rp_GenOutDisable(RP_CH_2);
    //rp_GenReset();

    trig_delay = 3900+16384+1200;

    for(i=0;i<buff_size;i+=p_step)
    {
	    printf("x_out[%d]= %f \n",i,x_out[i]);
    }

    for(i = 0; i < buff_size; i+=p_step)
    {
	    printf("x_in[%d] = %f \n",i,x_in[i]);
    }

    for(i = 0;i < N_spins; i++)
    {
	    int index = (2*i+1)*buff_per_spin/2;
	    printf("Index = %d \n",index);
            x_k[i] = x_in[index];
	    x_k[i] -= offset;
	    x_k[i] *= scale;
    }

    fprintf(fp2,"%f %f %d %d",alpha,beta,s,iteration);

    for(i=0;i<buff_size;i++)
    {
	    fprintf(fp,"iter=%d %d %f %f\n",iteration,i,x_out[i],x_in[i]);
    }
    for(i = 0;i < N_spins; i++)
    {
        fprintf(fp2," %f",x_k[i]);
    }
    fprintf(fp2,"\n");*/
//}

