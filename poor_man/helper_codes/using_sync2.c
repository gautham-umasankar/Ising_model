#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>
#include <pthread.h>

#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846
#define BUFFER_SIZE 16*1024
#define SYNC_BUFFER_SIZE 1000

int p_step = 1000;
int trig_delay;
int t1 = 7830;	//16384+3900;
int t2 = 8130;	//3900+16384+1200;
int breps = 1; 	// 3
int bcounts = 1;
float freq = 7630.0;

int N_spins = 1;
int N_iters = 30;
int N_noise = 100;
int N_runs = 1;
int buff_size = BUFFER_SIZE;
int buff_per_spin;

float ALPHA_MAX = 1.5;
float ALPHA_MIN = 1.5;
float ALPHA_STEP = 0.1;

float BETA_MAX = 0.5;
float BETA_MIN = 0.5;
float BETA_STEP = 0.1;

float offset = 0.5;
float sig_f = 0.04;
float scale = 1;
float att = 0.9;

void single_iteration(float, float, int, int);
int find_shift();

// x_in stores the input
float *x_in;
float *x_out;

FILE *fp, *j_file;

int find_shift_huffman(float value, int exp_ind)
{
    int index = -1, con1 = 0, i;
    for(i = 0;i<SYNC_BUFFER_SIZE-1 && index == -1;i++)
    {
        if(x_in[i] > -0.5 && x_in[i+1] <= -0.5)
        {
            index = i;
        }
    }

    while(x_in[i++] <= -0.5)
    {
        con1++;
    }
    // Need to change this return value in case the normal method doesnt work
    // and we need to use huffman
    return con1;
}

int find_shift()
{
    int i = 0, index = -1;
    while(i < SYNC_BUFFER_SIZE)
    {
        if(x_in[i++]>0.2)
        {
            index = i;
            break;
        }
    }
    if(index == 0)
    {
        while(i < SYNC_BUFFER_SIZE)
        {
            if(x_in[i++]<-0.2)
            {
                index = i;
                break;
            }
        }
        return index - (int)(SYNC_BUFFER_SIZE/2);
    }
    else
    {
        return index;
    }
    
}

void add_sync_part_huffman()
{
    int i=0,j=1;
    while(i<SYNC_BUFFER_SIZE)
    {
        int temp = 0;
        while(temp<j)
        {
            x_out[i++] = -1;
        }
        x_out[i++] = 0;
    }
    for(i=BUFFER_SIZE - SYNC_BUFFER_SIZE;i<BUFFER_SIZE;i++)
        x_out[i] = 0;
}

void add_sync_part()
{
    int i=0;
    for(;i<SYNC_BUFFER_SIZE/2;i++)
    {
        x_out[i] = 1;
    }

    for(;i<SYNC_BUFFER_SIZE;i++)
    {
        x_out[i] = -1;
    }
}

void single_iteration(float alpha, float beta, int s,int iteration)
{
    int i, n;

    //compute x_out

    for(i = 0;i <= N_spins;i++)
    {
	
        n = rand()%N_noise;
	    for(int j = SYNC_BUFFER_SIZE + i*buff_per_spin;j < (i+1)*(buff_per_spin) + SYNC_BUFFER_SIZE;j++)
        {
		x_out[j] = 0.01*n;
		//printf("Value=%f and index=%d\n", x_out[j],j);
        }
    }
    
    int pos1,pos2;
    // Send the output
    rp_GenArbWaveform(RP_CH_2, x_out, buff_size); //Does it start generating here itself?
    rp_AcqReset();
    rp_AcqGetWritePointer(&pos1);
    // printf("Pos at the start after AcqReset() = %d\n",pos1);
    // Start acquisition
    rp_AcqSetTriggerDelay(trig_delay);
    rp_AcqStart();
    rp_GenOutEnable(RP_CH_2);

    // Trigger immediately
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
    
    //rp_AcqGetWritePointerAtTrig(&pos1);

    //printf("Pos right after trigger now:%d\n",pos1);

    rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;    

    i = 0;
    // Wait for buffer to get full after Trigger.
    do
    {
	
        rp_AcqGetTriggerState(&state);
	    rp_AcqGetWritePointer(&pos2);
	//if(pos1>pos2)
 		//printf("i = %d pos_previous = %d pos_current = %d New iteration \n",i,pos1,pos2);
	//else
 		//printf("i = %d pos_previous = %d pos_current = %d\n",i,pos1,pos2);

	//pos1 = pos2;
        // printf("i = %d Pos = %d\n",i, pos2);
    	// i+=1;
    }while(state == RP_TRIG_STATE_TRIGGERED);
	
    rp_AcqGetWritePointer(&pos1);
    // printf("Pos after do while = %d\n",pos1);
    // Get data into buff
    rp_AcqGetOldestDataV(RP_CH_1, &buff_size, x_in);

    // Stop acquisition
    rp_AcqStop();
    rp_AcqGetWritePointer(&pos1);
    // printf("Pos after acqstop() = %d\n",pos1);
 
    //Reset the output to zero
    rp_GenOutDisable(RP_CH_2);
    //rp_GenReset();

    trig_delay = t2;

    // for(i=0;i<buff_size;i+=p_step)
    // {
		//printf("x_out[%d]= %f \n",i,x_out[i]);
    // }

    // for(i = 0; i < buff_size; i+=p_step)
    // {
	//     // printf("x_in[%d] = %f \n",i,x_in[i]);
    // }

    i = 92;
    int shift = find_shift(x_in[i]/att, i);
    printf("Iteration = %d , Shift = %d\n",iteration, shift);
    // for(i=SYNC_BUFFER_SIZE;i<buff_size-SYNC_BUFFER_SIZE;i++)
    // {
	//    fprintf(fp,"iter=%d %d %f %f %f\n",iteration,i,x_out[i],x_in[i]/att,x_in[i+shift]/att);
    // }
    for(i=0;i<BUFFER_SIZE;i++)
    {
	   fprintf(fp,"iter=%d %d %f %f %f\n",iteration,i,x_out[i],x_in[i]/att);//,x_in[i+shift]/att);
    }
}

int main (int argc, char **argv) 
{
    system ("cat /opt/redpitaya/fpga/fpga_0.94.bit > /dev/xdevcfg");
    struct tm *timenow;
    char sync_filename[] = "xout_xin_using_sync.csv";

    //time_t now = time(NULL);
    //timenow = localtime(&now);
    //strftime(sync_filename, sizeof(sync_filename), "./../data/xout_xin_%d_%m_%Y_%H_%M_%S.csv", timenow);

    char comment[100];
    printf("Enter comment on file: ");
    fgets(comment, sizeof(comment), stdin);  // read string
    printf("Comment entered:");
    fp = fopen(sync_filename,"w");

    fprintf(fp, "#%s\n",comment);
    printf("%s\n", comment);

    if(argc > 1)
    {
        for(int a=1;a<argc;a++)
        {
            char opt = argv[a][1];
            switch(opt)
            {
		case 'N': //Number of spins
			N_spins = atoi(argv[++a]);
			break;
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
                        fclose(j_file); 
                        j_file = fopen(argv[++a],"r");
 			            char line_for_N[20];
			            fgets(line_for_N, 20, j_file);
                        sscanf(line_for_N, "%d %*d", &N_spins);
                        break;
                case 'r': // Number of spins/runs
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
                case 'F':
                        freq = atof(argv[++a]);
                        break;
                case 'B':
                        if(argv[a][2] == 'c')
                            bcounts = atoi(argv[++a]);
                        else
                            breps = atof(argv[++a]);
                        break;
                        
                case 'T': //trig_delay
                        if(argv[a][2] == '1')
                            t1 = atof(argv[++a]);
                        else
                            t2 = atof(argv[++a]);
                        break;
                        
                default:printf("Invalid option: %c\n",opt);
			            return 0;
            }
        }
    }

    x_out = (float *)calloc(buff_size, sizeof(float));
    x_in = (float *)calloc(buff_size, sizeof(float));

    int i,s;
    // Initialization of API
    if (rp_Init() != RP_OK) 
    {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }
    else
    {
	printf("Initialisation of RedPitaya API done!\n");
    }

    float alpha;
    float beta;

    rp_AcqReset();
    rp_AcqSetDecimation(1);

    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
    // Send the output
    // Enable burst mode
    rp_GenMode(RP_CH_2, RP_GEN_MODE_BURST); 
    
    // One waveform per burst
    rp_GenBurstCount(RP_CH_2, bcounts);
   
     // Number of bursts
    rp_GenBurstRepetitions(RP_CH_2, breps);
   
    // Burst period. Will be dependent on computation time
    //rp_GenBurstPeriod(RP_CH_2, 130000);

    //rp_GenAmp(RP_CH_2, 1.0);
    rp_GenFreq(RP_CH_2, freq);
    
   

    buff_per_spin = (int)(BUFFER_SIZE - 2*SYNC_BUFFER_SIZE)/N_spins;
    trig_delay = t1; 
    srand(time(0));
    add_sync_part();
    for(alpha = ALPHA_MIN;alpha <= ALPHA_MAX;alpha += ALPHA_STEP)
    {
        for(beta = BETA_MIN;beta <= BETA_MAX;beta += BETA_STEP)
        {
            for(s = 0;s < N_runs; s++)
            {
                for(i = 0;i < N_iters;i++)
                {
                    single_iteration(alpha, beta, s, i);
                }
	        //printf("Alpha = %f Beta = %f The cut value is: %f\n",alpha,beta, cut_value());
            }
        }
    }

    // Releasing resources
    free(x_out);
    free(x_in);
    rp_Release();
    fclose(fp);
    return EXIT_SUCCESS;
}
