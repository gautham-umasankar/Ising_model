#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "redpitaya/rp.h"

int main(int argc, char **argv){

/* Print error, if rp_Init() function failed */
if(rp_Init() != RP_OK){
fprintf(stderr, "Rp api init failed!\n");
}
uint32_t buff_size = 16384;
float *buff = (float *)malloc(buff_size * sizeof(float));
float c1 = 0.6;
float *x_n = (float *)malloc(buff_size *sizeof(float));
float c2 = -0.6;
float c3 = 0.2;
int i = 0;
x_n[i++] = 0;
for(;i<buff_size/3;i++)
{
	x_n[i] = c1;
}
for(;i<buff_size*2/3;i++)
{
	x_n[i] = c2;
} 
	
for(;i<buff_size;i++)
{
	x_n[i] = c3;
} 

/*GENERATE*/
rp_GenReset();
rp_GenWaveform(RP_CH_2, RP_WAVEFORM_ARBITRARY);
rp_GenArbWaveform(RP_CH_2, x_n, buff_size);
//rp_GenFreq(RP_CH_2, 1000);
//rp_GenAmp(RP_CH_2, 1);
rp_GenMode(RP_CH_2, RP_GEN_MODE_BURST);
rp_GenBurstCount(RP_CH_2, 1);
//usleep(100);

rp_GenOutEnable(RP_CH_2);
rp_AcqReset();
rp_AcqSetDecimation(RP_DEC_8);
//rp_AcqSetTriggerLevel(RP_CH_1,0.1);
rp_AcqSetTriggerDelay(0);

rp_AcqStart();

/*If you have trigger delay 0, after acquisition is started some pause is
needed in order to acquire fresh samples in to buffer. Here we have used
time delay of one second but you can calculate exact value taking in
to account buffer length and smaling rate.
If you set TRIGGER DELAY to 8192 sample THEN you don’t need to have pause,
because time delay of 8192 will give you a full buffer (16384 samples) of new data.
I.e all samples will be AFTER the trigger event. With trigger delay 0,
half of a buffer are data before and half are data after trigger event.*/
//sleep(1);

rp_AcqSetTriggerSrc(RP_TRIG_SRC_NOW);
//rp_GenTrigger(2);
rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;

while(1){
rp_AcqGetTriggerState(&state);
if(state == RP_TRIG_STATE_TRIGGERED){
break;
}
}
//usleep(10000);
rp_AcqGetLatestDataV(RP_CH_1, &buff_size, buff);


for(i = 0; i < buff_size; i++){
printf("%d %f\n", i, buff[i]);
}

rp_GenOutDisable(RP_CH_2);
/* Releasing resources */
free(buff);
rp_Release();
rp_AcqStop();
return 0;
}
