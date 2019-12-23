#include <stdio.h>
#include <vector>
#include <unistd.h>  

#include "IntCode.h"



#include<stdlib.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

using namespace std;

int InputAuto(int* ram, int* instructArray, int instructionIndex)
{
    // printf("<< ");
    switch(instructArray[FIRST_MODE])
    {
        case POS:
            fscanf(stdout,"%d",&ram[ram[instructionIndex+1]]);
            break;
        case IM:
            fscanf(stdout,"%d",&ram[instructionIndex+1]); 
            break;
    }
    return instructionIndex + 2;
}

int OutputAuto(int* ram, int* instructArray, int instructionIndex)
{
    switch(instructArray[FIRST_MODE])
    {
        case POS:
            fprintf(stdout,"%d\n",ram[ram[instructionIndex+1]]);
            break;
        case IM:
            fprintf(stdout,"%d\n",ram[instructionIndex+1]);
            break;
    }
    return instructionIndex + 2;
}

int main()
{
    char filepath[] = "Day07/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector<int> ram[5];
   // Gather Inputs
    int val;
    while(fscanf(file,"%d",&val) != EOF)
    {            
        ram[0].push_back(val);
        fscanf(file,",");
    }    
    for(int i = 1; i < 5; ++i)
    {
        ram[i] = ram[0]; // Copy the initial memory to each of the computers
    }


    int pipeFd[6][2];
    pid_t p; 

    for(int i = 0; i < 6; ++i)
    {
        if (pipe(pipeFd[i]) == -1) 
        { 
            fprintf(stderr, "Pipe Failed" ); 
            return 1; 
        }
    }
    FILE* phase = fopen("Day07/phase.txt","r");
    pid_t p1 = fork();
    
    if(p1 > 0)
    {
        close(pipeFd[0][1]);
        FILE* input2 = fdopen(pipeFd[0][0],"r");
        printf("Waiting\n");
        wait(NULL);
        printf("Stopped Waiting\n");
        pid_t p2 = fork();
        if(p2 == 0)
        {
            printf("Child 2\n");
            FILE* input1 = phase;
            FILE* output = stdout;
            RunIntCode(input1,input2,output,ram[1].data());
            fclose(input2);
            
            // close(pipeFd[1][1]);
        }
    }
    else if(p1 == 0)
    {
        printf("Child 1\n");
        FILE* input2 = stdin;
        FILE* input1 = phase;
        FILE* output = fdopen(pipeFd[0][1],"w");
        close(pipeFd[0][0]);
        RunIntCode(input1,input2,output,ram[0].data());
        fclose(output);
        close(pipeFd[0][1]);
    }
    
    // for(int i = 0; i < 5; ++i)
    // {
        
    // }
    return 0;
}
  

