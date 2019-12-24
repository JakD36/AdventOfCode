#include <stdio.h>
#include <vector>
#include <unistd.h>  
#include<stdlib.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <signal.h>

#include "IntCode.h"

using namespace std;

int main()
{
    // FILE* phaseFile = fopen("Day07/phase.txt","w");
    // for(int i = 0; i < 44445; i++)
    // {
    //     int sum = 0;
    //     const int arrayLength = 5;
    //     int tmpArray[arrayLength] = {0};
    //     bool validArray = true;
    //     for(int n = 0; n < arrayLength; ++n)
    //     {
    //         tmpArray[n] = (i - sum) / pow(10,arrayLength-1-n) ;
    //         sum += tmpArray[n] * pow(10,arrayLength-1-n);
            
    //         for(int m = 0; m < n; ++m)
    //         {
    //             if(tmpArray[n] == tmpArray[m])
    //             {
    //                 validArray = false;
    //                 break;
    //             }
    //         }
    //         if(tmpArray[n] > 4)
    //         {
    //             validArray = false;
    //             break;
    //         }
    //     }
    //     if(validArray)
    //         fprintf(phaseFile,"%d %d %d %d %d\n",tmpArray[0],tmpArray[1],tmpArray[2],tmpArray[3],tmpArray[4]);
    // }
    // fclose(phaseFile);

    char filepath[] = "Day07/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector<int> initRam;
    vector<int> ram[5];
   // Gather Inputs
    int val;
    while(fscanf(file,"%d",&val) != EOF)
    {            
        initRam.push_back(val);
        fscanf(file,",");
    }    

    FILE* phase = fopen("Day07/phase.txt","r");
    FILE* init = fopen("Day07/init.txt","r");
    
    FILE* pipes[] = 
    {
        fopen("Day07/pipe1.txt","w+"),
        fopen("Day07/pipe2.txt","w+"),
        fopen("Day07/pipe3.txt","w+"),
        fopen("Day07/pipe4.txt","w+")
        
    };
    FILE* output = fopen("Day07/output.txt","w+");

    FILE* IO[5 * 3] = 
    {
        phase, init, pipes[0],
        phase, pipes[0], pipes[1],
        phase, pipes[1], pipes[2],
        phase, pipes[2], pipes[3],
        phase, pipes[3], output,
    };

    int max = 0;
    for(int i = 0; i < 120; ++i)
    {
        for(int k = 0; k < 5; ++k)
        {
            ram[k] = initRam;
        }

        fpos_t position;
        fgetpos (phase, &position);
        int phaseNum[5]; 
        fscanf(phase,"%d %d %d %d %d",&phaseNum[0],&phaseNum[1],&phaseNum[2],&phaseNum[3],&phaseNum[4]);
        fprintf(output, "%d %d %d %d %d\t",phaseNum[0],phaseNum[1],phaseNum[2],phaseNum[3],phaseNum[4]);
        fsetpos (phase, &position);
        
        fgetpos(output,&position);
        for(int j = 0; j < 5; ++j)
        {
            RunIntCode(IO[j * 3 + 0],IO[j * 3 + 1],IO[j * 3 + 2],ram[j].data());
            if(IO[j * 3 + 2] != output)
                rewind(pipes[j]);
        }
        fsetpos(output,&position);
        int val;
        fscanf(output,"%d",&val);
        if(val > max)
        {
            max = val;
        }

        for(int j = 0; j < 4; ++j)
        {
            rewind(pipes[j]);
        }
    }
    printf("Max value = %d",max);

    return 0;
}
  
