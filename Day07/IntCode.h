#ifndef IntCode
#define IntCode

#include <stdio.h>

#include <stdio.h>
#include <math.h>

#include <vector>

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
    THIRD_MODE = 0,
    SECOND_MODE = 1,
    FIRST_MODE = 2,
    INSTRUCTION = 3
};

int RunIntCode(int* intCode);
void SplitInstruction(int* array, const int instruction);
int Add(int* ram, int* instructArray, int instructionIndex);
int Mul(int* ram, int* instructArray, int instructionIndex);
int Input(int* ram, int* instructArray, int instructionIndex);
int Output(int* ram, int* instructArray, int instructionIndex);
int JumpIf1(int* ram, int* instructArray, int instructionIndex);
int JumpIf0(int* ram, int* instructArray, int instructionIndex);
int LessThan(int* ram, int* instructArray, int instructionIndex);
int Equals(int* ram, int* instructArray, int instructionIndex);

int RunIntCode(int* ram)
{
    int i = 0;
    int instructArray[4] = {0};
    SplitInstruction(instructArray,ram[i]);
    int mode = 0;

    while(instructArray[3] != END)
    {
        switch (instructArray[3])
        {
        case ADD:
            i = Add(ram,instructArray,i);
            break;
        case MUL:
            i = Mul(ram,instructArray,i);
            break;
        case IN:
            i = Input(ram,instructArray,i);
            break;
        case OUT:
            i = Output(ram,instructArray,i);
            break;
        case JUMP_IF_1:
            i = JumpIf1(ram,instructArray,i);
            break;
        case JUMP_IF_0:
            i = JumpIf0(ram,instructArray,i);
            break;
        case LESS_THAN:
            i = LessThan(ram,instructArray,i);
            break;
        case EQUALS:
            i = Equals(ram,instructArray,i);
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

int Add(int* ram, int* instructArray, int instructionIndex)
{
    int paramA;
    int paramB;
    int i = 0;
    switch(instructArray[FIRST_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[SECOND_MODE])
    {
        case POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[THIRD_MODE])
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
    switch(instructArray[FIRST_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[SECOND_MODE])
    {
        case POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[THIRD_MODE])
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
    switch(instructArray[FIRST_MODE])
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
    switch(instructArray[FIRST_MODE])
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
    switch(instructArray[FIRST_MODE])
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
    switch(instructArray[SECOND_MODE])
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
    switch(instructArray[FIRST_MODE])
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
    switch(instructArray[SECOND_MODE])
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
    switch(instructArray[FIRST_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[SECOND_MODE])
    {
        case POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[THIRD_MODE])
    {
        case POS:
            ram[ram[instructionIndex + 3]] = paramA < paramB ? 1 : 0;
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
    switch(instructArray[FIRST_MODE])
    {
        case POS:
            paramA = ram[ram[instructionIndex + 1]];
            break;
        case IM:
            paramA = ram[instructionIndex + 1];
            break;
    }

    i = 1;
    switch(instructArray[SECOND_MODE])
    {
        case POS:
            paramB = ram[ram[instructionIndex + 2]];
            break;
        case IM:
            paramB = ram[instructionIndex + 2];
            break;
    }
    
    switch(instructArray[THIRD_MODE])
    {
        case POS:
            ram[ram[instructionIndex + 3]] = paramA == paramB ? 1 : 0;
            break;
        case IM:
            ram[instructionIndex + 3] = paramA == paramB ? 1 : 0;
            break;
    }

    return instructionIndex + 4;
}
#endif