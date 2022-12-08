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

    JUMP_IF_1 = 5,
    JUMP_IF_0 = 6,

    LESS_THAN = 7,
    EQUALS = 8,

    END = 99
};

enum Mode
{
    POS = 0,
    IM = 1
};

enum Param
{
    OUTPUT_MODE = 0,
    INPUTB_MODE = 1,
    INPUTA_MODE = 2,
    INSTRUCTION = 3
};

int RunIntCode(int* intCode);
void SplitInstruction(int* array, const int instruction);
int Add(int* ram, int* modes, int instructionIndex);
int Mul(int* ram, int* modes, int instructionIndex);
int Input(int* ram, int* modes, int instructionIndex);
int Output(int* ram, int* modes, int instructionIndex);
int JumpIf1(int* ram, int* modes, int instructionIndex);
int JumpIf0(int* ram, int* modes, int instructionIndex);
int LessThan(int* ram, int* modes, int instructionIndex);
int Equals(int* ram, int* modes, int instructionIndex);

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
            i = Add(ram,instructArray,i);
            break;
        case Op::MUL:
            i = Mul(ram,instructArray,i);
            break;
        case Op::IN:
            i = Input(ram,instructArray,i);
            break;
        case Op::OUT:
            i = Output(ram,instructArray,i);
            break;
        case Op::JUMP_IF_1:
            i = JumpIf1(ram,instructArray,i);
            break;
        case Op::JUMP_IF_0:
            i = JumpIf0(ram,instructArray,i);
            break;
        case Op::LESS_THAN:
            i = LessThan(ram,instructArray,i);
            break;
        case Op::EQUALS:
            i = Equals(ram,instructArray,i);
            break;
        }
        SplitInstruction(instructArray,ram[i]);
        // printf("i = %d\n",i);
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

int Add(int* ram, int* instructArray, int instructionIndex)
{
    int paramA;
    int paramB;
    int i = 0;
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[INPUTB_MODE])
    {
        case POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[OUTPUT_MODE])
    {
        case POS:
            ram[ram[instructionIndex + 3]] = paramA + paramB;
            break;
        case IM:
            ram[instructionIndex + 3] = paramA + paramB;
            break;
    }

    return instructionIndex + 4;
}

int Mul(int* ram, int* instructArray, int instructionIndex)
{
    int paramA;
    int paramB;
    int i = 0;
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[INPUTB_MODE])
    {
        case POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[OUTPUT_MODE])
    {
        case POS:
            ram[ram[instructionIndex + 3]] = paramA * paramB;
            break;
        case IM:
            ram[instructionIndex + 3] = paramA * paramB;
            break;
    }
    return instructionIndex + 4;
}

int Input(int* ram, int* instructArray, int instructionIndex)
{
    printf("<< ");
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            scanf("%d",&ram[ram[instructionIndex+1]]);
            break;
        case IM:
            scanf("%d",&ram[instructionIndex+1]); 
            break;
    }
    return instructionIndex + 2;
}

int Output(int* ram, int* instructArray, int instructionIndex)
{
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            printf("%d : %d >> %d\n",instructionIndex+1,ram[instructionIndex+1],ram[ram[instructionIndex+1]]);
            break;
        case IM:
            printf("%d : %d >> %d\n",instructionIndex+1,instructionIndex+1,ram[instructionIndex+1]);
            break;
    }
    return instructionIndex + 2;
}

int JumpIf1(int* ram, int* instructArray, int instructionIndex)
{
    int condition;
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            condition = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            condition = ram[instructionIndex + 1];
            break;
        default:
            printf("Error: Unexpected Instruction param mode");
            break;
    }

    int output;
    switch(instructArray[INPUTB_MODE])
    {
        case POS:
            output = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            output = ram[instructionIndex + 2];
            break;
        default:
            printf("Error: Unexpected Instruction param mode");
            break;
    }
    
    if(condition)
    {
        return output;
    }

    return instructionIndex + 3;
}

int JumpIf0(int* ram, int* instructArray, int instructionIndex)
{
    int condition;
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            condition = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            condition = ram[instructionIndex + 1];
            break;
        default:
            printf("Error: Unexpected Instruction param mode");
            break;
    }

    int output;
    switch(instructArray[INPUTB_MODE])
    {
        case POS:
            output = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            output = ram[instructionIndex + 2];
            break;
        default:
            printf("Error: Unexpected Instruction param mode");
            break;
    }
    
    if(!condition)
    {
        return output;
    }

    return instructionIndex + 3;
}

int LessThan(int* ram, int* instructArray, int instructionIndex)
{
    int paramA;
    int paramB;
    int i = 0;
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[INPUTB_MODE])
    {
        case POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[OUTPUT_MODE])
    {
        case POS:
            ram[ram[instructionIndex + 3]] = paramA < paramB ? 1 : 0;
            // if(ram[instructionIndex + 3] == instructionIndex)
            //     return instructionIndex;
            break;
        case IM:
            ram[instructionIndex + 3] = paramA < paramB ? 1 : 0;
            break;
    }

    return instructionIndex + 4;
}

int Equals(int* ram, int* instructArray, int instructionIndex)
{
    int paramA;
    int paramB;
    int i = 0;
    switch(instructArray[INPUTA_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[INPUTB_MODE])
    {
        case Mode::POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case Mode::IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[OUTPUT_MODE])
    {
        case POS:
            ram[ram[instructionIndex + 3]] = paramA == paramB ? 1 : 0;
            // if(ram[instructionIndex+3] == instructionIndex)
            //     return instructionIndex;
            break;
        case IM:
            ram[instructionIndex + 3] = paramA == paramB ? 1 : 0;
            break;
    }

    return instructionIndex + 4;
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