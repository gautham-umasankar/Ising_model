#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>
#include <pthread.h>

#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846
#define N_spins 50 
#define BUFFER_SIZE 16*1024

int p_step = 100;
int trig_delay = 16384+3900;

int N_iters = 30;
int N_noise = 100;
int N_runs = 1;
int buff_size = BUFFER_SIZE;
int buff_per_spin = (int)BUFFER_SIZE/N_spins;

float ALPHA_MAX = 1.5;
float ALPHA_MIN = 1.5;
float ALPHA_STEP = 0.1;

float BETA_MAX = 0.5;
float BETA_MIN = 0.5;
float BETA_STEP = 0.1;

float offset = 0.04;
float sig_f = 0.025;
float scale = 20;

void *acquisition_handler(void *);

pthread_t acquisition_thread;

// Buffer size
//const uint32_t buff_size = 16;

// x_in stores the input
volatile float *x_in;
float *x_k;
volatile float *x_out;

FILE *fp, *j_file;
float *noise;
float **J;

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

void read_J()
{
    int row,column;
    float value;
    char line[20];
    printf("\nReading file for J...\n");
    fgets(line, 20, j_file);
    puts(line);
    while(fgets(line, 20, j_file)!=0)
    {
        // puts(line);
        sscanf(line, "%d %d %f", &row, &column, &value);
        // printf("J[%d][%d]=%f\n",row,column,J[row][column]);
        J[row-1][column-1] = value;
    }
    printf("Done reading.\n");
    fclose(j_file);
}

void feedback(float *fb, float alpha, float beta)
{
    for(int i = 0;i < N_spins;i++)
    {
        for(int j = 1;j < N_spins;j++)
	{
            fb[i] += beta*J[i][(i+j)%N_spins]*x_k[(j+i)%N_spins];
	}
        fb[i] += alpha*x_k[i];
    }
}

void single_iteration(float alpha, float beta, int s,int iteration)
{
    int i, n;

    //compute x_out

    // Multiiply by alpha and add noise
    float *next = (float *)malloc(N_spins * sizeof(float));
    float *feedback_terms = (float *)malloc(N_spins * sizeof(float));
    
    feedback(feedback_terms, alpha, beta);
    // Calculate the feedback value according to the equation
    for(i = 0;i < N_spins;i++)
    {
        n = rand()%N_noise;
        float value = -feedback_terms[i] + noise[n];
        
        // Remove in lab
        value = pow(cos(value + (0.25*M_PI)),2); //Modulator function. 

        //Threshold the output
        if(value >= 1.0)
        {
            value = 1.0;
        }
        else if(value<=-1.0)
        {
            value = -1.0;
        }

        next[i] = 0.01*n;
    }

    // x_out an array that will store the output
    
    // Store the value in the buffer to be given as output for the next
    // buff_size cycles

    for(i = 0;i < N_spins;i++)
    {
        for(int j = i*buff_per_spin;j < (i+1)*(buff_per_spin);j++)
        {
            x_out[j] =next[i];
        }
    }

    // Send the output
    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
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

    rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;    

    // Wait for buffer to get full after Trigger.
    do
    {
        rp_AcqGetTriggerState(&state);
	    rp_AcqGetWritePointer(&pos);
        printf("Pos = %d\n", pos);
    }while(state == RP_TRIG_STATE_TRIGGERED);
	
    rp_AcqGetWritePointer(&pos);
    printf("Pos = %d\n",pos);
    // Get data into buff
    rp_AcqGetOldestDataV(RP_CH_1, &buff_size, x_in);

    // Stop acquisition
    rp_AcqStop();

    //Reset the output to zero
    //rp_GenOutDisable(RP_CH_2);
    //rp_GenReset();

    trig_delay = 3900+16384+1000;

    for(i=0;i<buff_size;i+=p_step)
    {
	    printf("x_out[%d]= %f \n",i,x_out[i]);
    }

    // Average over the buffer size
    for(i = 0; i < buff_size; i+=p_step)
    {
		printf("x_in[%d] = %f \n",i,x_in[i]);
    }

    for(i = 0;i < N_spins; i++)
    {
        x_k[i] = x_in[(i+1)*(buff_per_spin/2)];
    }

    //fprintf(fp,"%f %f %d %d %f",alpha,beta,s,iteration);
    for(i=0;i<buff_size;i++)
    {
	fprintf(fp,"iter=%d %d %f %f\n",iteration,i,x_out[i],x_in[i]);
    }
    for(i = 0;i < N_spins; i++)
    {
        //fprintf(fp," %f",x_k[i]);
    }
    fprintf(fp,"\n");
}


int main (int argc, char **argv) 
{

    fp = fopen("data_2spins.csv","w");
    j_file = fopen("./Maxcut_instances/pw05_100.3.txt","r");

    x_out = (float *)malloc(buff_size * sizeof(float));
    x_in = (float *)malloc(buff_size * sizeof(float));
    noise = (float *)malloc(N_noise * sizeof(float));

    int i,s;

    J = (float **)malloc(N_spins * sizeof(float *));
    for(int k = 0;k < N_spins;k++)
    {
        J[k] = (float *)malloc(N_spins * sizeof(float));
    }

    //read_J();

    //for(i = 0;i< N_spins;i++)
    //    for(int j = 0;j<N_spins;j++)
    //        if(J[i][j]>0)
    //            printf("J[%d][%d]=%f\n",i,j,J[i][j]);

    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    float alpha;
    float beta;

    gen_noise();
    //initialize x_out to zero
    for(i=0;i<buff_size;i++)
    {
	    x_out[i] = noise[i];
    }

    rp_AcqReset();
    rp_AcqSetDecimation(1);

    rp_GenOutEnable(RP_CH_2);
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
                case 'l': // Lower limit of alpha
                        ALPHA_MIN = atof(argv[++a]);
                        break;
                case 't': // Upper limit of beta
                        BETA_MAX = atof(argv[++a]);
                        break;
                case 'g': // Step size of beta
                        BETA_STEP = atof(argv[++a]);
                        break;
                case 'd': // Lower limit of beta
                        BETA_MIN = atof(argv[++a]);
                        break;
                case 'a': // If we want only one value of alpha
                        ALPHA_MIN = atof(argv[++a]);
                        ALPHA_MAX = ALPHA_MIN;
                        break;  
                case 'b': // Change value of beta
                        BETA_MIN = atof(argv[++a]);
                        BETA_MAX = BETA_MIN;
                        break;
                case 'j': //Change value of J
                       // fclose(j_file);
                        //j_file = fopen(argv[++a],"w");
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

    fprintf(fp,"#Alpha Beta Run/Spin Iteration Values\n");
   
    for(alpha = ALPHA_MIN;alpha <= ALPHA_MAX;alpha += ALPHA_STEP)
    {
        for(beta = BETA_MIN;beta <= BETA_MAX;beta += BETA_STEP)
        {
            // Generate new noise for each alpha
            gen_noise();

            //add alpha to the diagonal entries of J
            for(i = 0;i < N_spins;i++)
            {
                J[i][i] = alpha;
            }

            for(s = 0;s < N_runs; s++)
            {
                x_k = (float *)malloc(N_spins * sizeof(float));
                for(i = 0;i < N_iters;i++)
                {
                    single_iteration(alpha, beta, s, i);
                }
                free(x_k);
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
