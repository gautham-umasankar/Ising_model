#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<time.h>
#include<inttypes.h>

#include "redpitaya/rp.h"

#define BUFFER_SIZE 16*1024

int N_spins = 4;
int p_step = 1000;
int N_noise = 100;
int buff_size = BUFFER_SIZE;
int buff_per_spin;
int main()
{
        float *x_out = (float *)malloc(buff_size*sizeof(float));
	int i;
	float n,value;
	if (rp_Init() != RP_OK)
	{
		fprintf(stderr, "Red Pitaya API init failed!\n");
		printf("Red Pitaya API init failed!\n");
		return EXIT_FAILURE;
	}
        srand(time(0));	
	rp_GenOutEnable(RP_CH_2);
	buff_per_spin = (int)BUFFER_SIZE/N_spins;

	for(i = 0;i<N_spins;i++)
	{
		n = rand()%N_noise/100.0;
		value = n;
		if(i==0)
			value = 0;
		printf("Value = %f\n",value);
		if(value>=1.0)
			value = 1.0;
		else if(value<=-1.0)
			value = -1.0;
		
		for(int j = i*buff_per_spin;j<(i+1)*buff_per_spin;j++)
			x_out[j] = value;
	}
	for(i = 0;i<BUFFER_SIZE;i+=p_step)
		printf("x_out[%d] = %f\n",i,x_out[i]);

	rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
	rp_GenArbWaveform(RP_CH_2,x_out,buff_size); //Does it start generating here?
	rp_GenMode(RP_CH_2,RP_GEN_MODE_BURST);
	//One waveform per burst
	rp_GenBurstCount(RP_CH_2,1);
	//Number of bursts
	rp_GenBurstRepetitions(RP_CH_2,1);
	
	//rp_GenBurstPeriod(RP_CH_2,130000);

	//rp_GenAmp(RP_CH_2,1.0);

	rp_GenFreq(RP_CH_2,7690.0);
	free(x_out);
	rp_Release();

	return EXIT_SUCCESS;

}




