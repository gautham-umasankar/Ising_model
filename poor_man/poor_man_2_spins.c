#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>
#include <pthread.h>

#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846
#define N_spins 2

int p_step = 1000;
int trig_delay = 8189+16384+4200;

int N_iters = 50;
int N_noise = 100;
int N_runs = 1;
int buff_size = 16*1024;

float ALPHA_MAX = 3.0;
float ALPHA_MIN = 0.5;
float ALPHA_STEP = 0.1;
float offset = 0.04;
float sig_f = 0.025;
float scale = 20;
float beta = 1.0;
float J_12 = 1.0;

void *acquisition_handler(void *);

pthread_t acquisition_thread;

// Buffer size
//const uint32_t buff_size = 16;

// x_in stores the input
volatile float *x_in;
float *noise;
volatile float *x_out;
FILE *fp;
float x_k1,x_k2;

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
		printf("\nNoise %d = %f\n",i ,noise[i]);
	}
}

void *acquisition_handler(void *dummy)
{
    rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;    
    int pos;

    // Wait for buffer to get full after Trigger.
    do
    {
        rp_AcqGetTriggerState(&state);
	rp_AcqGetWritePointer(&pos);
        printf("Pos = %d\n", pos);
    }while(state == RP_TRIG_STATE_TRIGGERED);
	
    uint32_t b_size = buff_size;
    // Get data into buff
    rp_AcqGetOldestDataV(RP_CH_1, &b_size, x_in);
    //rp_AcqStart();
}

void single_iteration(float alpha, int s,int iteration)
{
    int i=0;
    //compute x_out

    int n = rand()%N_noise;
    // Multiiply by alpha and add noise
    printf("\nValue of n = %d", n);
    float next1 = alpha*x_k1 + beta*J_12*x_k2 + noise[n];
    next1 = 0.4;

    n = rand()%N_noise;
    printf("\nValue of n = %d\n", n);
    float next2 = alpha*x_k2 + beta*J_12*x_k1 + noise[n];
    next2 = -0.4;
    //Threshold the output
    if(next1 >= 1.0)
    {
	    next1 = 1.0;
    }
    else if(next1<=-1.0)
    {
	    next1 = -1.0;
    }

    if(next2 >= 1.0)
    {
	    next2 = 1.0;
    }
    else if(next2<=-1.0)
    {
	    next2 = -1.0;
    }

    // Calculate the next value according to the equation

    // Remove in lab
    next1 = pow(cos(next1 + (0.25*M_PI)),2); //Modulator function. 

    next2 = pow(cos(next2 + (0.25*M_PI)),2); //Modulator function. 

    // x_out an array that will store the output
    
    // Store the value in the buffer to be given as output for the next
    // buff_size cycles

    //x_out[i] = 0.0;
    for(;i < buff_size/2; i++)
    {
        x_out[i] = next1;
    }
    for(;i < buff_size; i++)
    {
        x_out[i] = next2;
    }
    printf("\nnext1 = %f\n next2=%f\n", next1, next2);


    // Send the output
    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
    rp_GenArbWaveform(RP_CH_2, x_out, buff_size);
    // Enable burst mode
    rp_GenMode(RP_CH_2, RP_GEN_MODE_BURST);
    // One waveform per burst
    rp_GenBurstCount(RP_CH_2, 1);
    // Number of bursts
    rp_GenBurstRepetitions(RP_CH_2, 1);
    // Burst period. Will be dependent on computation time
    // rp_GenBurstPeriod(RP_CH_2, 5000);

    rp_GenAmp(RP_CH_2, 1.0);
    rp_GenFreq(RP_CH_2, 7690.0);

    rp_AcqReset();
    // Start acquisition
    rp_AcqSetTriggerDelay(trig_delay);
    rp_AcqStart();

    // Trigger immediately
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    int pos;
    rp_AcqGetWritePointerAtTrig(&pos);

    printf("Pos right after trigger now:%d",pos);

    // Start the acquisition thread
    pthread_create(&acquisition_thread, NULL, acquisition_handler, NULL);
    rp_GenOutEnable(RP_CH_2);

    // Wait for acquisition to complete
    pthread_join(acquisition_thread,NULL);

    // Stop acquisition
    rp_AcqStop();

    //Reset the output to zero
    rp_GenOutDisable(RP_CH_2);
    rp_GenAmp(RP_CH_2, 0);
    
    for(i=0;i<buff_size;i+=p_step)
	printf("x_out[%d]= %f \n",i,x_out[i]);

    x_k1 = 0.0;
    x_k2 = 0.0;

    // Average over the buffer size
    for(i = 0; i < buff_size/2; i++)
    {
   	    x_k1 += x_in[i];
	    if(i%p_step ==  0)
		printf("x_in[%d] = %f \n",i,x_in[i]);
    }


    printf("x_k1 = %f\n", x_k1);
    // Add the offset
    x_k1 /= (buff_size/2);
    printf("x_k1 = %f\n", x_k1);
    x_k1 -= (offset);
    x_k1 *= scale;
    printf("x_k1 = %f\n", x_k1);

    for(; i < buff_size; i++)
    {
   	x_k2 += x_in[i];
    	if(i%p_step ==  0)
		printf("x_in[%d] = %f \n",i,x_in[i]);

    }

    // Add the offset
    x_k2 /= (buff_size/2);
    x_k2 -= (offset);
    x_k2 *= scale;

    fprintf(fp,"%f %d %d %f %f\n",alpha,s,iteration,x_k1,x_k2);
}


int main (int argc, char **argv) 
{

    fp = fopen("data_2spins.csv","w");
    //fprintf(fp,"Writing a test line\n");
    x_out = (float *)malloc(buff_size * sizeof(float));
    x_in = (float *)malloc(buff_size * sizeof(float));
    noise = (float *)malloc(N_noise * sizeof(float));
    fprintf(fp,"#Alpha Run/Spin Iteration Value1 Value2\n");
    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    int i,s;
    float alpha;
    gen_noise();
    //initialize x_out to zero
    for(i=0;i<buff_size;i++)
	x_out[i] = noise[i];

    rp_AcqReset();
    rp_AcqSetDecimation(1);

    // Think we might have to change this

        
    if(argc > 1)
    {
        for(int a=1;a<argc;a++)
        {
            char opt = argv[a][1];
            switch(opt)
            {
                case 'i': // Number of iterations per spin
                        N_iters =atoi(argv[++a]);
                        break;
                case 'v': // Standard deviation of noise
                        sig_f = atof(argv[++a]);
                        break;
                case 'u': // Upper limit of alpha
                        ALPHA_MAX = atof(argv[++a]);
                        break;
                case 'p': // Step size of alpha
                        ALPHA_STEP = atof(argv[++a]);
                        break;
                case 'd': // Lower limit of alpha
                        ALPHA_MIN = atof(argv[++a]);
                        break;
                case 'a': // If we want only one value of alpha
                        ALPHA_MIN = atof(argv[++a]);
                        ALPHA_MAX = ALPHA_MIN;
                        break;  
                case 'B': // Change value of beta
                        beta = atof(argv[++a]);
                        break;
                case 'J': //Change value of J
                        J_12 = atof(argv[++a]);
                        break;
                case 'N': // Number of spins/runs
                        N_runs = atoi(argv[++a]);
                        break;
                case 'o': // Offset
                        offset = atof(argv[++a]);
                        break;
                case 's': // Scaling factor due to the photodiode
                        scale = atof(argv[++a]);
                        break;
                case 'n': // Number of points in noise
                        N_noise = atoi(argv[++a]);
                        break;
                case 'b': // Buffer size of x_in and x_out
                        buff_size = atoi(argv[++a]);
                        break;
                case 'f': // File to save data in
                        fclose(fp);
                        fp = fopen(argv[++a],"w");
			            break;
                default:printf("Invalid option.\n");
			            return 0;
            }
        }
    }
    //printf("Alpha MAx = %f\nAlpha min= %f\nalpha step =  %f\n offset=%f\n   \
    N_iters= %d\nN_runs= %d\n buff_size=%d\n",ALPHA_MAX,ALPHA_MIN, ALPHA_STEP,\
    offset,N_iters,N_runs,buff_size);
   
    for(alpha = ALPHA_MIN;alpha <= ALPHA_MAX;alpha += ALPHA_STEP)
    {
	// Generate new noise for each alpha
        gen_noise();
        for(s = 0;s < N_runs; s++)
        {
	        x_k1 = 0.0;
            x_k2 = 0.0;
            for(i = 0;i < N_iters;i++)
            {
                single_iteration(alpha,s,i);
            }
        }
    }

    // Releasing resources
    free(x_out);
    free(x_in);
    free(noise);
    rp_Release();
    fclose(fp);
    return EXIT_SUCCESS;
}
