#include <stdio.h>
#include <vector>
#include <unistd.h>  

#include "IntCode.h"



#include<stdlib.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

using namespace std;

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
        wait(NULL); // Wait on Child process to complete
        
        pid_t p2 = fork(); 
        if(p2 > 0)
        {
            wait(NULL); // Wait on Child process to complete
            
            pid_t p3 = fork();
            if(p3 > 0)
            {
                wait(NULL); // Wait on Child process to complete
                
                pid_t p4 = fork();
                if(p4 > 0)
                {
                    wait(NULL); // Wait on Child process to complete
                    
                    pid_t p5 = fork();
                    if(p5 == 0)
                    {
                    
                        close(pipeFd[3][1]); // Close write end pipe 2
                        FILE* input2 = fdopen(pipeFd[3][0],"r"); // Open FILE* to read end pipe 2
                        FILE* input1 = phase;
                        FILE* output = stdout;//fdopen(pipeFd[4][1],"w");
                        RunIntCode(input1,input2,output,ram.data());
                        fclose(input2);
                        fclose(output);
                    }
                }
                else if(p4 == 0)
                {
                    close(pipeFd[2][1]); // Close write end pipe 2
                    FILE* input2 = fdopen(pipeFd[2][0],"r"); // Open FILE* to read end pipe 2
                    FILE* input1 = phase;
                    FILE* output = fdopen(pipeFd[3][1],"w");
                    RunIntCode(input1,input2,output,ram.data());
                    fclose(input2);
                    fclose(output);
                    close(pipeFd[3][1]);
                }
            }
            else if(p3 == 0)
            {
                close(pipeFd[2][0]);
                close(pipeFd[1][1]); // Close write end pipe 2
                FILE* input2 = fdopen(pipeFd[1][0],"r"); // Open FILE* to read end pipe 2
                FILE* input1 = phase;
                FILE* output = fdopen(pipeFd[2][1],"w");
                RunIntCode(input1,input2,output,ram.data());
                fclose(input2);
                fclose(output);
                close(pipeFd[2][1]);
            }
        }
        else if(p2 == 0)
        {   
            close(pipeFd[0][1]); // Close write end pipe 1
            close(pipeFd[1][0]); // Close read end pipe 2
            FILE* input2 = fdopen(pipeFd[0][0],"r"); // Open FILE* to read end pipe 1 
            
            FILE* input1 = phase;
            FILE* output = fdopen(pipeFd[1][1],"w"); // open write end of pipe 2 for output
            RunIntCode(input1,input2,output,ram.data());
            fclose(input2);
            fclose(output);
            close(pipeFd[1][1]);
            close(pipeFd[0][0]);
        }
    }
    else if(p1 == 0)
    {
        FILE* input2 = stdin;
        FILE* input1 = phase;
        FILE* output = fdopen(pipeFd[0][1],"w");
        close(pipeFd[0][0]); // Close read end
        RunIntCode(input1,input2,output,ram.data());
        fclose(output);
        close(pipeFd[0][1]); // Close write end
    }
    
    return 0;
}
  

