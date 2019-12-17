#include <stdio.h>
#include <math.h>

#include <vector>

using namespace std;

// The opcode is the rightmost two digits of the first value of an instruction
// Param modes are single digits, one per param read right to left from the opcode

// The first parameter's mode is in the hundreds digit, the second in the thousands
// Third in the 10s of thousands and so on 
// Any missing modes are 0

// 1002,4,3,4,33

enum Op
{
    ADD = 1,
    MUL = 2,

    IN = 3,
    OUT = 4,

    END = 99
};

enum Param
{
    POS = 0,
    IM = 1
};

int RunIntCode(int* intCode);
void SplitInstruction(int* array, const int instruction);
void Add(int* ram, int modes, int instructionIndex);
void Mul(int* ram, int modes, int instructionIndex);
void Input(int* ram, int instructionIndex);
void Output(int* ram, int instructionIndex);

int main()
{
    char filepath[] = "Day05/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector<int> ram;
   
    int val;
    while(fscanf(file,"%d",&val) != EOF)
    {            
        ram.push_back(val);
        fscanf(file,",");
    }    
    
    RunIntCode(ram.data());
    return 0;
}
  
int RunIntCode(int* ram)
{
    int i = 0;
    int instructArray[4] = {0};
    SplitInstruction(instructArray,ram[i]);
    int mode = 0;
    while(instructArray[3] != Op::END)
    {
        mode = instructArray[2] << 0;
        mode |= instructArray[1] << 1;
        mode |= instructArray[0] << 2;

        switch (instructArray[3])
        {
        case Op::ADD:
            Add(ram,mode,i);
            i+=4;
            break;
        case Op::MUL:
            Mul(ram,mode,i);
            i+=4;
            break;
        case Op::IN:
            Input(ram,i);
            i+=2;
            break;
        case Op::OUT:
            Output(ram,i);
            i+=2;
            break;
        }
        SplitInstruction(instructArray,ram[i]);
    }
    return ram[0];
}

void SplitInstruction(int* array, const int instruction)
{
    int sum = 0;
    const int arrayLength = 5;
    int tmpArray[arrayLength] = {0};
    for(int n = 0; n < arrayLength; ++n)
    {
        tmpArray[n] = (instruction - sum) / pow(10,arrayLength-1-n) ;
        sum += tmpArray[n] * pow(10,arrayLength-1-n);
    }
    array[0] = tmpArray[0];
    array[1] = tmpArray[1];
    array[2] = tmpArray[2];
    array[3] = tmpArray[3] * 10 + tmpArray[4];
}

void Add(int* ram, int modes, int instructionIndex)
{
    const int numInputs = 2;
    int array[numInputs] = {0};
    for(int i = 0; i < numInputs; ++i)
    {
        int bit = modes & 1 << i;
        switch(bit)
        {
            case Param::POS:
                array[i] = ram[ram[instructionIndex + i + 1]];
                break;
            case Param::IM:
                array[i] = ram[instructionIndex + i + 1];
                break;
        }
    }
    ram[ram[instructionIndex + 3]] = array[0] + array[1];
}

void Mul(int* ram, int modes, int instructionIndex)
{
    const int numInputs = 3;
    int array[numInputs];
    for(int i = 0; i < numInputs - 1; ++i)
    {
        int bit = modes & 1 << i;
        switch(bit)
        {
            case Param::POS:
                array[i] = ram[ram[instructionIndex + i + 1]];
                break;
            case Param::IM:
                array[i] = ram[instructionIndex + i + 1];
                break;
        }
    }
    ram[ram[instructionIndex + 3]] = array[0] * array[1];
}

void Input(int* ram, int instructionIndex)
{
    printf(">> ");
    scanf("%d",&ram[ram[instructionIndex+1]]);
}

void Output(int* ram, int instructionIndex)
{
    printf("%d\n",ram[ram[instructionIndex+1]]);
}