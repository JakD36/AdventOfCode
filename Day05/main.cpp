#include <stdio.h>
#include <vector>

using namespace std;

enum Op
{
    ADD = 1,
    MUL = 2,



    END = 99
};

enum Param
{
    POS = 0,
    IM = 1
};

int RunIntCode(int* intCode);

int main()
{
    char filepath[] = "Day05/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector<int> rawMemory;
   
    int val;
    while(fscanf(file,"%d",&val) != EOF)
    {            
        rawMemory.push_back(val);
        fscanf(file,",");
    }    

    vector<int> memory(rawMemory.size());
    int i, j;
    for(i = 0; i < 100; ++i)
    {
        for(j = 0; j < 100; ++j)
        {
            memory = rawMemory;
            memory[1] = i;
            memory[2] = j;
            int result = RunIntCode(memory.data());
            if(result == 19690720)
            {
                printf("Result found!\n");
                int answer = 100 * i + j;
                printf("%d\n",answer);
                return 0;
            }
        }   
    }
    return 2;
}
  

int RunIntCode(int* intCode)
{
    int i = 0;
    while(intCode[i] != 99)
    {
        switch (intCode[i])
        {
        case 1:
            intCode[intCode[i+3]] = intCode[intCode[i + 1]] + intCode[intCode[i + 2]];
            break;
        case 2:
            intCode[intCode[i+3]] = intCode[intCode[i + 1]] * intCode[intCode[i + 2]];
            break;
        }
        i+=4;
    }
    return intCode[0];
}

int* Split
