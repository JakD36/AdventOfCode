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

void ForkIntCode(FILE* IO[], int level, int max, int* ram);

int main()
{
    char filepath[] = "Day07/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector<int> ram;
   // Gather Inputs
    int val;
    while(fscanf(file,"%d",&val) != EOF)
    {            
        ram.push_back(val);
        fscanf(file,",");
    }    
    
    int pipeFd[4][2];

    for(int i = 0; i < 6; ++i)
    {
        if (pipe(pipeFd[i]) == -1) 
        { 
            fprintf(stderr, "Pipe Failed" ); 
            return 1; 
        }
    }
    
    FILE* phase = fopen("Day07/phase.txt","r");
    
    FILE* init = fopen("Day07/init.txt","r");
    FILE* IO[5 * 3] = 
    {
        phase, init, fdopen(pipeFd[0][1],"w"),
        phase, fdopen(pipeFd[0][0],"r"), fdopen(pipeFd[1][1],"w"),
        phase, fdopen(pipeFd[1][0],"r"), fdopen(pipeFd[2][1],"w"),
        phase, fdopen(pipeFd[2][0],"r"), fdopen(pipeFd[3][1],"w"),
        phase, fdopen(pipeFd[3][0],"r"), stdout,
    };
    ForkIntCode(IO,0,5,ram.data());
    
    return 0;
}
  
void ForkIntCode(FILE* IO[], int level, int max, int* ram)
{
    pid_t p = fork();
    if(p > 0)
    {
        if(level < max)
        {
            wait(NULL);;
            ForkIntCode(IO, level + 1, max, ram);
        }
    }
    else if (p == 0)
    {
        int arrayStart = level * 3;
        int input1 = arrayStart;
        int input2 = arrayStart + 1;
        int output = arrayStart + 2;
        RunIntCode(IO[input1],IO[input2],IO[output],ram);
        fclose(IO[input2]);
        fclose(IO[output]);
    }
}
