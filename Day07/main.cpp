#include <stdio.h>
#include <stdio.h>
#include <vector>

#include "IntCode.h"

using namespace std;

int main()
{
    char filepath[] = "Day05/input.txt";
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

    for(int i = 0; i < 5; ++i)
    {
        RunIntCode(ram[i].data());
    }
    return 0;
}
  

