#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>
#include <pthread.h>

#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846
#define ALPHA_MAX 1.15
#define ALPHA_MIN 1.1
#define ALPHA_STEP 0.1
#define N_noise 100
#define N_spins 1 //Number of runs
#define offset 0.5
#define buff_size 16



volatile int N_iters = 100;

void *acquisition_handler(void *);

pthread_t acquisition_thread;

// Buffer size
//const uint32_t buff_size = 16;

// buff stores the input
volatile float *buff;
float *noise;
volatile float *x_n;
FILE *fp;

void gen_noise()
{
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
	sig = sqrt(sig)*40;
	for(i=0;i<N_noise;i++)
	{
		noise[i] = (noise[i] - mu)/sig;
	}
}

void *acquisition_handler(void *dummy)
{
    rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;    

    // Wait for buffer to get full after Trigger.
    do
    {
        rp_AcqGetTriggerState(&state);
        //printf("State = %d\n", state);
    }while(state == RP_TRIG_STATE_TRIGGERED);
    //state != RP_TRIG_STATE_DISABLED this variable doesn't exist
    uint32_t b_size = buff_size;
    // Get data into buff
    rp_AcqGetLatestDataV(RP_CH_1, &b_size, buff);
}

void single_iteration(float alpha, int s,int iteration)
{
    int i;
    // Send the output
    rp_GenArbWaveform(RP_CH_2, x_n, buff_size);
    // Enable burst mode
    rp_GenMode(RP_CH_2, RP_GEN_MODE_BURST);
    // One waveform per burst
    rp_GenBurstCount(RP_CH_2, 1);
    // Number of bursts
    rp_GenBurstRepetitions(RP_CH_2, -1);
    // Burst period. Will be dependent on computation time
    // rp_GenBurstPeriod(RP_CH_2, 5000);

    rp_GenAmp(RP_CH_2, 0.9);
    // rp_GenFreq(RP_CH_2, 4000.0);

    rp_AcqReset();
    // Start acquisition
    rp_AcqStart();

    // Trigger immediately
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);

    // Start the acquisition thread
    pthread_create(&acquisition_thread, NULL, acquisition_handler, NULL);
    rp_GenOutEnable(RP_CH_2);

    // Wait for acquisition to complete
    pthread_join(acquisition_thread,NULL);

    // Stop acquisition
    rp_AcqStop();

    //Reset the output to zero
    rp_GenAmp(RP_CH_2, 0);

    // Value from photodiode
    float x_k = 0.0;
    
    for(i = 0;i<buff_size;i++)
	printf("x_n[%d] = %f \n",i,x_n[i]);
    // Average over the buffer size
    for(i = 0; i < buff_size; i++)
    {
        printf("buff[%d] = %f\n",i,buff[i]);
        x_k += buff[i];
    }

    // Add the offset
    x_k /= buff_size;
    x_k -= (offset);
    fprintf(fp,"%f %d %d %f\n",alpha,s,iteration,x_k);
    

    int n = rand()%N_noise;
    // Multiiply by alpha and add noise
    float next = alpha*x_k+noise[n];
    
    // Calculate the next value according to the equation

    next = pow(cos(x_k + (0.25*M_PI)),2);

    
    
    // x_n an array that will store the output
    //float *x_n = (float *)malloc(buff_size * sizeof(float));

    // Store the value in the buffer to be given as output for the next
    // buff_size cycles
    for(i = 0;i < buff_size; i++)
    {
        x_n[i] = next;
    }
    printf("next = %f\n\n", next);
}


int main (int argc, char **argv) 
{

    fp = fopen("data.csv","w");
    //fprintf(fp,"Writing a test line\n");
    x_n = (float *)malloc(buff_size * sizeof(float));
    buff = (float *)malloc(buff_size * sizeof(float));
    noise = (float *)malloc(N_noise * sizeof(float));
    fprintf(fp,"# Alpha Run/Spin Iteration Value\n");
    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    int i,s;
    float alpha;
    gen_noise();
    //initialize x_n to zero
    for(i=0;i<buff_size;i++)
	x_n[i] = noise[i];

    rp_AcqReset();
    rp_AcqSetDecimation(1);

    // Think we might have to change this
    rp_AcqSetTriggerDelay(8200);

    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
        
    // Initialise variables
    if(argc == 2)
    {
        N_iters  = atoi(argv[1]);
    }

    for(alpha = ALPHA_MIN;alpha <= ALPHA_MAX;alpha += ALPHA_STEP)
    {
	// Generate new noise for each alpha
        gen_noise();
        for(s = 0;s < N_spins; s++)
        {
            for(i = 0;i < N_iters;i++)
            {
                single_iteration(alpha,s,i);
            }
        }
    }

    // Releasing resources
    free(x_n);
    free(buff);
    free(noise);
    rp_GenOutDisable(RP_CH_2);
    rp_Release();
    fclose(fp);
    return EXIT_SUCCESS;
}
