#include<stdio.h>
#include<time.h>


void main()
{
	float i,j,s;
	int N, k;
	N = 10000;
	clock_t start,end;
	double cpu_time;
	for(k=1;k<N;k++)
	{
		start = clock();
		for(i=1.0;i<=k;i+=1.0)
		{
			for(j=1.0;j<=k;j+=1.0)
			{
				s=i*j;
			}
		}
		end = clock();
		cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;
		printf("%d %lf \n",k,cpu_time);
	}
	
}
