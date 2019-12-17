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
void Add(int* ram, int* modes, int instructionIndex);
void Mul(int* ram, int* modes, int instructionIndex);
void Input(int* ram, int* modes, int instructionIndex);
void Output(int* ram, int* modes, int instructionIndex);
// void WriteRamToFile(int* ram, FILE* file);

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
        switch (instructArray[3])
        {
        case Op::ADD:
            Add(ram,instructArray,i);
            i+=4;
            break;
        case Op::MUL:
            Mul(ram,instructArray,i);
            i+=4;
            break;
        case Op::IN:
            Input(ram,instructArray,i);
            i+=2;
            break;
        case Op::OUT:
            Output(ram,instructArray,i);
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

void Add(int* ram, int* modes, int instructionIndex)
{
    int paramA;
    int paramB;
    int i = 0;
    switch(modes[2])
    {
        case Param::POS:
            paramA = ram[ram[instructionIndex + i + 1]];
            break;
        case Param::IM:
            paramA = ram[instructionIndex + i + 1];
            break;
    }

    i = 1;
    switch(modes[1])
    {
        case Param::POS:
            paramB = ram[ram[instructionIndex + i + 1]];
            break;
        case Param::IM:
            paramB = ram[instructionIndex + i + 1];
            break;
    }
    
    switch(modes[0])
    {
        case Param::POS:
            ram[ram[instructionIndex + 3]] = paramA + paramB;
            break;
        case Param::IM:
            ram[instructionIndex + 3] = paramA + paramB;
            break;
    }
}

void Mul(int* ram, int* modes, int instructionIndex)
{
    int paramA;
    int paramB;
    int i = 0;
    switch(modes[2])
    {
        case Param::POS:
            paramA = ram[ram[instructionIndex + i + 1]];
            break;
        case Param::IM:
            paramA = ram[instructionIndex + i + 1];
            break;
    }

    i = 1;
    switch(modes[1])
    {
        case Param::POS:
            paramB = ram[ram[instructionIndex + i + 1]];
            break;
        case Param::IM:
            paramB = ram[instructionIndex + i + 1];
            break;
    }
    
    switch(modes[0])
    {
        case Param::POS:
            ram[ram[instructionIndex + 3]] = paramA * paramB;
            break;
        case Param::IM:
            ram[instructionIndex + 3] = paramA * paramB;
            break;
    }
}

void Input(int* ram, int* modes, int instructionIndex)
{
    printf("<< ");
    switch(modes[2])
    {
        case Param::POS:
            scanf("%d",&ram[ram[instructionIndex+1]]);
            break;
        case Param::IM:
            scanf("%d",&ram[instructionIndex+1]); 
            break;
    }
}

void Output(int* ram, int* modes, int instructionIndex)
{
    switch(modes[2])
    {
        case Param::POS:
            printf("%d : %d >> %d\n",instructionIndex+1,ram[instructionIndex+1],ram[ram[instructionIndex+1]]);
            break;
        case Param::IM:
            printf("%d : %d >> %d\n",instructionIndex+1,instructionIndex+1,ram[instructionIndex+1]);
            break;
    }
}

// void WriteRamToFile(int* ram, FILE* file)
// {
//     int size = 625;
//     for(int i = 0; i < size; ++i)
//     {
//         fprintf(file,"%d: %d,\t",i,ram[i]);
//     }
//     fprintf(file,"\n");
// }