#include <stdio.h>
#include <vector>
#include <set>

using namespace std;

int RunIntCode(int* intCode);

int main()
{
    vector<int> intCode;
   
    char filepath[] = "Day2/input.txt";
    FILE* file = fopen(filepath,"r");
    int val;

    while(fscanf(file,"%d",&val) != EOF)
    {            
        intCode.push_back(val);
        fscanf(file,",");
    }    

    // Build new opcode program
    set<int> potentialUnknowns;
    potentialUnknowns.insert(1);
    potentialUnknowns.insert(2);
    // Collect old ops
    int i = 0;
    while(intCode[i] != 99)
    {
        potentialUnknowns.insert(intCode[i+3]);
        i+=4;
    }

    vector<int> unknownOps;
    potentialUnknowns.erase(0);
    vector<int> newOps;
    for(int n = i-4; n >= 0; n-=4)
    {
        // Get the reverse op + -> - * -> /
        newOps.push_back(intCode[n] + 2); // Convert + to - and * to /
        if(intCode[n] == 1)
        {
            printf("%d + %d -> %d\t",intCode[n+1],intCode[n+2],intCode[n+3]);
        }
        else if(intCode[n] == 2)
        {
            printf("%d * %d -> %d\t",intCode[n+1],intCode[n+2],intCode[n+3]);    
        }
    
        int unknowns = 0;
        bool firstKnown = potentialUnknowns.find(intCode[n+1]) == potentialUnknowns.end();
        bool secondKnown = potentialUnknowns.find(intCode[n+2]) == potentialUnknowns.end();
        bool resultKnown = potentialUnknowns.find(intCode[n+3]) == potentialUnknowns.end();
        
        if(firstKnown && !secondKnown && resultKnown)
        {
            newOps.push_back(intCode[n+3]);
            newOps.push_back(intCode[n+1]);
            newOps.push_back(intCode[n+2]);
            potentialUnknowns.erase(intCode[n+3]);
            if(intCode[n] + 2 ==3)
            {
                printf("%d - %d -> %d\n",intCode[n+3],intCode[n+1],intCode[n+2]);
            }
            else if(intCode[n] + 2 == 4)
            {
                printf("%d / %d -> %d\n",intCode[n+3],intCode[n+1],intCode[n+2]);    
            }
            // printf("Erase %d\n",intCode[n+2]);
        }
        else if(!firstKnown && secondKnown && resultKnown)
        {
            newOps.push_back(intCode[n+3]);
            newOps.push_back(intCode[n+2]);
            newOps.push_back(intCode[n+1]);
            potentialUnknowns.erase(intCode[n+3]);
            if(intCode[n] + 2 == 3)
            {
                printf("%d - %d -> %d\n",intCode[n+3],intCode[n+2],intCode[n+1]);
            }
            else if(intCode[n] + 2 == 4)
            {
                printf("%d / %d -> %d\n",intCode[n+3],intCode[n+2],intCode[n+1]);    
            }
            // printf("Erase %d\n",intCode[n+1]);
        }
        else if( (!firstKnown && !secondKnown))
        {
            printf("\n");
            printf("Two Unknowns %d %d\n",intCode[n+1],intCode[n+2]);
            newOps.pop_back();
            // unknownOps.push_back(intCode[n]);
            // unknownOps.push_back(intCode[n+1]);
            // unknownOps.push_back(intCode[n+2]);
            // unknownOps.push_back(intCode[n+3]);
        }
        else if( (!firstKnown && !resultKnown) )
        {
            printf("\n");
            printf("Two Unknowns %d %d\n",intCode[n+1],intCode[n+3]);
            newOps.pop_back();
            // unknownOps.push_back(intCode[n]);
            // unknownOps.push_back(intCode[n+1]);
            // unknownOps.push_back(intCode[n+2]);
            // unknownOps.push_back(intCode[n+3]);
        }
        else if(  (!secondKnown && !resultKnown))
        {
            printf("\n");
            printf("Two Unknowns %d %d\n",intCode[n+2],intCode[n+3]);
            newOps.pop_back();
            // unknownOps.push_back(intCode[n]);
            // unknownOps.push_back(intCode[n+1]);
            // unknownOps.push_back(intCode[n+2]);
            // unknownOps.push_back(intCode[n+3]);
        }
        else
        {
            potentialUnknowns.erase(intCode[n+3]);
            printf("\n");
            printf("Help\n");
        }
    }

    for(set<int>::iterator it = potentialUnknowns.begin(); it != potentialUnknowns.end(); ++it)
    {
        printf("%d ",*it);
    }printf("\n");

    for(int n = 0; n < newOps.size(); n++)
    {
        printf("%d ", newOps[n]);
    }
    printf("\n");

    switch(newOps[0])
    {
        case 3:
            
            intCode[newOps[3]] = 19690720 - intCode[newOps[2]];
            printf("%d - %d -> %d\t%d - %d = %d\n",newOps[1],newOps[2],newOps[3],19690720,intCode[newOps[2]],intCode[newOps[3]]);
            break;
        case 4:
            intCode[newOps[3]] = 19690720 / intCode[newOps[2]];
            printf("%d / %d -> %d\t%d / %d = %d\n",newOps[1],newOps[2],newOps[3],19690720,intCode[newOps[2]],intCode[newOps[3]]);
            break;
    }
    for(int n = 4; n < newOps.size(); n+=4)
    {
        switch(newOps[n])
        {
            case 3:
                intCode[newOps[n+3]] = intCode[newOps[n+1]] - intCode[newOps[n+2]];
                printf("%d - %d -> %d\t%d - %d = %d\n",newOps[n+1],newOps[n+2],newOps[n+3],intCode[newOps[n+1]],intCode[newOps[n+2]],intCode[newOps[n+3]]);
                break;
            case 4:
                intCode[newOps[n+3]] = intCode[newOps[n+1]] / intCode[newOps[n+2]];
                printf("%d / %d -> %d\t%d / %d = %d\n",newOps[n+1],newOps[n+2],newOps[n+3],intCode[newOps[n+1]],intCode[newOps[n+2]],intCode[newOps[n+3]]);
                break;
        }
    }

    for(int n = 0; n < unknownOps.size(); n++)
    {
        printf("%d ", unknownOps[n]);
    }
    printf("\n");
    
    printf("%d %d",intCode[1],intCode[2]);
    
    // int output = RunIntCode(intCode.data());
    // printf("%d\n",output);
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
        case 3:
            intCode[intCode[i+3]] = intCode[intCode[i + 1]] - intCode[intCode[i + 2]];
            break;
        case 4:
            intCode[intCode[i+3]] = intCode[intCode[i + 1]] / intCode[intCode[i + 2]];
            break;
        }
        i+=4;
    }
    return intCode[0];
}