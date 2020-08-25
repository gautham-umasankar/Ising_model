#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>
#include <pthread.h>

#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846

void *acquisition_handler(void *);

const float c = 0.5;

volatile bool ready_to_process = 0;
pthread_t acquisition_thread;
// pthread_mutex_t process_lock;//dont think this is needed

// Buffer size
// const uint32_t buff_size = 16; //See if this works while passing
uint32_t buff_size = 16*1024;
volatile float *buff;

void *acquisition_handler(void *dummy)
{
    rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;    

    // After acquisition is started some time delay is needed in order to 
    // acquire fresh samples in to buffer
    // Here we have used time delay of one second but you can calculate exact
    // value taking in to account buffer length and sampling rate

    state = RP_TRIG_STATE_TRIGGERED;
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);

    //sleep(1);
    
    do
    {
        rp_AcqGetTriggerState(&state);
        printf("State = %d\n", state);
    }while(state != RP_TRIG_STATE_TRIGGERED);


    // Get data into buff
    rp_AcqGetLatestDataV(RP_CH_1, &buff_size, buff);
}


int main (int argc, char **argv) 
{
    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    rp_AcqReset();
    rp_AcqSetDecimation(1);
    rp_AcqSetTriggerDelay(0);

    rp_AcqStart();

    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);

    // Enable burst mode
    rp_GenMode(RP_CH_2, RP_GEN_MODE_CONTINUOUS);
    // One waveform per burst
    //rp_GenBurstCount(RP_CH_2, 1);
    // Number of bursts
    //rp_GenBurstRepetitions(RP_CH_2, 1);
    // Burst period. Will be dependent on computation time
    //rp_GenBurstPeriod(RP_CH_2, 5000);
    
    rp_GenAmp(RP_CH_1, 0.7);
    // rp_GenFreq(RP_CH_1, 4000.0);

        
    // Initialise variables

    // x_n an array that will store the output
    float *x_n = (float *)malloc(1000 * sizeof(float));

    // buff stores the input
    buff = (float *)malloc(buff_size * sizeof(float));

    int i;

    for(i=0;i<1000/2;i++)
    {
        x_n[i]=c;
    }

    for(;i<1000;i++)
    {
        x_n[i]=c;
    }
    pthread_create(&acquisition_thread, NULL, acquisition_handler, NULL);

    // Send the output
    rp_GenArbWaveform(RP_CH_2, x_n, 1000);

    rp_GenTrigger(1);
    //sleep(8192/1250);
    rp_GenOutEnable(RP_CH_2);

    // Wait for acquisition to complete
    pthread_join(acquisition_thread,NULL);

    printf("Acquired data:\n");
    for(i=0;i<buff_size;i+=1)
    {
        printf("%d : %f\n",i,buff[i]);
    }

    // Releasing resources
    free(x_n);
    free(buff);
    rp_GenOutDisable(RP_CH_2);
    rp_AcqStop(RP_CH_1);
    rp_Release();

    return EXIT_SUCCESS;
}
