#include <stdio.h>
#include <stdlib.h>

#define N_spins 100
float **J;

void read_J()
{
    FILE *j_file = fopen("./Maxcut_instances/pw05_100.3.txt","r");
    int row,column;
    float value;
    char line[20];
    fgets(line, 20, j_file);
    puts(line);
    printf("\nReading file for J...\n");
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

int main(int argc, char **argv)
{
    J = (float **)malloc(N_spins * sizeof(float *));
    for(int k = 0;k < N_spins;k++)
    {
        J[k] = (float *)malloc(N_spins * sizeof(float));
    }

    read_J();
    for(int i = 0;i< N_spins;i++)
        for(int j = 0;j<N_spins;j++)
            if(J[i][j]>0)
                printf("J[%d][%d]=%f\n",i,j,J[i][j]);
    
    return 0;
}