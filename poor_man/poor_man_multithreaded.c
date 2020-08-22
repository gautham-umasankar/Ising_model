#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <inttypes.h>
#include <pthread.h>

#include "redpitaya/rp.h"

#define M_PI 3.14159265358979323846

void *acquisition_handler(void *);

volatile bool ready_to_process = 0;
pthread_t acquisition_thread;
// pthread_mutex_t process_lock;//dont think this is needed

// Buffer size
// const uint32_t buff_size = 16; //See if this works while passing
uint32_t buff_size = 16;
volatile float *buff;

void *acquisition_handler(void *dummy)
{
    rp_acq_trig_state_t state;

    rp_AcqReset();
    rp_AcqSetDecimation(1);
    rp_AcqSetTriggerDelay(0);

    rp_AcqStart();

    // After acquisition is started some time delay is needed in order to 
    // acquire fresh samples in to buffer
    // Here we have used time delay of one second but you can calculate exact
    // value taking in to account buffer length and sampling rate
    sleep(1);

    state = RP_TRIG_STATE_TRIGGERED;

    while(1)
    {
        while(ready_to_process)
        {}

        rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);

        do
        {
            rp_AcqGetTriggerState(&state);
            //printf("State = %d\n", state);
        }while(state != RP_TRIG_STATE_TRIGGERED);

        // Get data into buff
        rp_AcqGetLatestDataV(RP_CH_1, &buff_size, buff);

        // Signal to show that data is ready to be processed
        ready_to_process = 1;
    }
}


int main (int argc, char **argv) 
{
    // Initialization of API
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);

    // Enable burst mode
    rp_GenMode(RP_CH_2, RP_GEN_MODE_BURST);
    // One waveform per burst
    rp_GenBurstCount(RP_CH_2, 1);
    // Number of bursts
    rp_GenBurstRepetitions(RP_CH_2, -1);
    // Burst period. Will be dependent on computation time
    rp_GenBurstPeriod(RP_CH_2, 5000);

    rp_GenAmp(RP_CH_1, 0.7);
    // rp_GenFreq(RP_CH_1, 4000.0);

        
    // Initialise variables

    // x_k is the "current" value of the photovoltage
    float x_k = 0.0;
    float next = 1.0, old_next = 0.0;

    // x_n an array that will store the output
    float *x_n = (float *)malloc(buff_size * sizeof(float));

    // buff stores the input
    buff = (float *)malloc(buff_size * sizeof(float));

    int i;

    // Start the acquisition thread
    pthread_create(&acquisition_thread, NULL, acquisition_handler, NULL);

    sleep(1);

    while(fabs(next - old_next)>0.0001)
    {
        old_next = next;
        while(!ready_to_process)
        {}

        // Average over the buffer size
        for(i = 0; i < buff_size; i++)
        {
            //printf("%f\n", buff[i]);
            x_k += buff[i];
        }
        x_k /= buff_size;

        printf("x_k = %f\n", x_k);

        // Calculate the next value according to the equation
        next = pow(cos(x_k - (0.25*M_PI)),2) - 0.5;

        // Store the value in the buffer to be given as output for the next
        // buff_size cycles
        for(i = 0;i < buff_size; i++)
        {
            x_n[i] = next;
        }
	    printf("next = %f\n\n", next);

        // Send the output
        rp_GenArbWaveform(RP_CH_2, x_n, buff_size);

        // Signal to show that data is ready to be acquired
        ready_to_process = 0;

        rp_GenOutEnable(RP_CH_2);
    }

    // Releasing resources
    free(x_n);
    free(buff);
    rp_GenOutDisable(RP_CH_2);
    rp_AcqStop(RP_CH_1);
    rp_Release();

    pthread_join(acquisition_thread,NULL);

    return EXIT_SUCCESS;
}
