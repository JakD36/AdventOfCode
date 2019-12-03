#include <stdio.h>
#include <vector>

using namespace std;

int RunIntCode(int* intCode);

int main()
{
    // ops 
    // 1 Adds
    // 2 multiplies
    // 99 end

    // Pos 0 operations
    // Pos 1 input 1 position
    // Pos 2 input 1 position
    // Pos 3 output position

    vector<int> intCode;

    char filepath[] = "Day2/input.txt";
    FILE* file = fopen(filepath,"r");
    int val;

    while(fscanf(file,"%d",&val) != EOF)
    {            
        intCode.push_back(val);
        fscanf(file,",");
    }    

    
    int output = RunIntCode(intCode.data());
    printf("%d\n",output);
    return 0;
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