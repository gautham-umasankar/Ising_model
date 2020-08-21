#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "redpitaya/rp.h"

int main (int argc, char **argv) 
{
    if (rp_Init() != RP_OK) {
        fprintf(stderr, "Red Pitaya API init failed!\n");
        return EXIT_FAILURE;
    }

    // Buffer size
    uint32_t buff_size = 16;
    
    // Initialise variables

    // x_n an array that will store the output
    float *x_n = (float *)malloc(buff_size * sizeof(float));
   
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

    for(i = 0;i < buff_size; i++)
    {
        x_n[i] = 1.0;
    }

    // Print the value calculated.
    printf("next: %f \n", next);
    fprintf(fp, "%f\n", next);

    // Send the output
    rp_GenArbWaveform(RP_CH_2, x_n, buff_size);

    // rp_GenTrigger(RP_CH_2);
    rp_GenOutEnable(RP_CH_2);

    // Releasing resources
    free(x_n);
    rp_Release();

    return EXIT_SUCCESS;
}
