#include <algorithm>
#include <chrono>
#include <vector>
#include <set>

enum class Type
{
    nop,
    acc,
    jmp
};

struct Instruction
{
public:
    Type m_type;
    int m_val;
};

struct Node
{
public:
    Type m_type;
    int m_val;
    std::vector<int> m_from;
};

void Part1(FILE* file);
void Part2(FILE* file);
void Part2Brute(FILE* file);
void Analyse(FILE* file);
void Part2Smart(FILE* file);

std::vector<Instruction> Parse(FILE* file);
std::vector<Node> ParseToNodes(FILE* file);
void ExecuteBootCode(std::vector<Instruction> &instructions);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Part2Smart(file);
    }
    else
    {
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Part1(FILE* file)
{
    std::set<int> visitedInstructions;

    auto instructions = Parse(file);
    int global = 0, i = 0;
    while(true)
    {
        if (visitedInstructions.emplace(i).second == false)
        {
            printf("global %d\n",global);
            break;
        }
        switch(instructions[i].m_type)
        {
            case Type::jmp:
                i += instructions[i].m_val;
                break;
            case Type::acc:
                global += instructions[i].m_val;
                ++i;
                break;
            case Type::nop:
                ++i;
                break;
        }
    }
}

void Part2(FILE* file) // WIP: Idea is to reduce the analyses to nodes that cause a loop
{
    auto instructions = Parse(file);
    int instructionCount = instructions.size();
    int* executionPath = new int[instructionCount];
    std::fill(executionPath,executionPath+instructionCount,-1);


    int global = 0, i = 0, n = -1;
    bool fixed = false;
    int executionCount = 0;
    while(i < instructionCount && executionCount < instructionCount)
    {
        auto find = std::find(executionPath,executionPath + instructionCount, i);
        if (find == (executionPath + instructionCount))
        {
            executionPath[++n] = i;
        }
        else
        {
            for(int j = 0; j < n; ++j)
            {
                printf("%d ",executionPath[j]);
            }
            printf("\n");
            break;
        }

        switch(instructions[i].m_type)
        {
            case Type::jmp:
                i += instructions[i].m_val;
                break;
            case Type::acc:
                global += instructions[i].m_val;
                ++i;
                break;
            case Type::nop:
                ++i;
                break;
        }
        ++executionCount;
    }
    if(executionCount == instructionCount)
    {
        printf("Error stuck in an infinite loop!\n");
        std::sort(executionPath,executionPath+executionCount);
        for(int j = 0; j < executionCount; ++j)
        {
            printf("%d ",executionPath[j]);
        }
        printf("\n");
    }
    else
    {
        printf("%d\n",global);
    }

}

void Part2Brute(FILE* file) // Instruction 363 is the problem (line 364)
{
    auto instructions = Parse(file);
    int instructionCount = instructions.size();

    for(int n = 0; n < instructionCount; ++n)
    {
        int global = 0, i = 0;
        int executionCount = 0;
        if(instructions[n].m_type == Type::acc)
            continue;

        instructions[n].m_type = instructions[n].m_type == Type::nop ? Type::jmp : Type::nop;

        while(i < instructionCount && executionCount < instructionCount)
        {
            switch(instructions[i].m_type)
            {
                case Type::jmp:
                    i += instructions[i].m_val;
                    break;
                case Type::acc:
                    global += instructions[i].m_val;
                    ++i;
                    break;
                case Type::nop:
                    ++i;
                    break;
            }
            ++executionCount;
        }
        if(executionCount == instructionCount)
        {
//            printf("Error %d\n",n);
            instructions[n].m_type = instructions[n].m_type == Type::nop ? Type::jmp : Type::nop;
        }
        else
        {
            printf("%d\n",global);
            break;
        }
    }


}

std::vector<Node> ParseToNodes(FILE* file)
{
    std::vector<Node> nodes(609); // 608 known lines in the file + 1

    char opStr[4];
    int val, i = 0;
    while(fscanf(file,"%s %d",opStr,&val) != EOF)
    {
        if(strcmp(opStr,"nop") == 0)
        {
            nodes[i].m_val = val;
            nodes[i].m_type = Type::nop;
            nodes[i+1].m_from.push_back(i);
        }
        else if(strcmp(opStr,"acc") == 0)
        {
            nodes[i].m_val = val;
            nodes[i].m_type = Type::acc;
            nodes[i+1].m_from.push_back(i);
        }
        else if(strcmp(opStr,"jmp") == 0)
        {
            nodes[i].m_val = val;
            nodes[i].m_type = Type::jmp;
            nodes[i+val].m_from.push_back(i);
        } else
            assert("Error: Unexpected Instruction type!");
        ++i;
    }
    return nodes;
}


void FindAllNodesThatPointToEnd(int i, std::vector<Node> &nodes, std::vector<int> &output)
{
    output.push_back(i);
    for(auto& from : nodes[i].m_from){
        FindAllNodesThatPointToEnd(from,nodes,output);
    }
}

void Part2Smart(FILE* file)
{
    auto nodes = ParseToNodes(file);

    std::vector<int> nodesToEnd;
    FindAllNodesThatPointToEnd(608,nodes,nodesToEnd);

    int global = 0, i = 0;
    int instructionCount = nodes.size();
    bool fixed = false;
    while(i < instructionCount)
    {
        switch(nodes[i].m_type)
        {
            case Type::jmp:
                if(!fixed && std::find(begin(nodesToEnd),end(nodesToEnd),i + 1) != end(nodesToEnd))
                {
                    nodes[i].m_type = Type::nop;
                    fixed = true;
                    continue;
                }
                i += nodes[i].m_val;
                break;
            case Type::acc:
                global += nodes[i].m_val;
                ++i;
                break;
            case Type::nop:
                if(!fixed && std::find(begin(nodesToEnd),end(nodesToEnd),i + nodes[i].m_val) != end(nodesToEnd))
                {
                    nodes[i].m_type = Type::jmp;
                    fixed = true;
                    continue;
                }
                ++i;
                break;
            default:
                assert("Unexpected instruction."); break;
        }
    }
    printf("Global %d\n",global);
}

// Generates a dot file that can be used to create a directed graph of the execution paths as they are
void Analyse(FILE* file)
{
    auto instructions = Parse(file);
    int instructionsCount = instructions.size();
    FILE* wf = fopen("graph","w");
    fprintf(wf,"digraph {\n");
    for(int i = instructionsCount - 1; i >= 1; --i)
    {
        switch(instructions[i].m_type)
        {
            case Type::nop:
                fprintf(wf,"\t%d [color=\"red\" shape=circle style=filled]",i);
                fprintf(wf,"\t%d -> %d\n",i,i+1);
                break;
            case Type::acc:
                fprintf(wf,"\t%d -> %d\n",i,i+1);
                break;
            case Type::jmp:
                fprintf(wf,"\t%d [color=\"lightblue\" shape=circle style=filled]\n",i);
                fprintf(wf,"\t%d -> %d\n",i,i+instructions[i].m_val);
                break;
        }
    }
    switch(instructions[0].m_type)
    {
        case Type::nop:
            fprintf(wf,"\t%d [color=\"green\" shape=circle style=filled]",0);
            fprintf(wf,"\t%d -> %d\n",0,1);
            break;
        case Type::acc:
            fprintf(wf,"\t%d -> %d\n",0,1);
            break;
        case Type::jmp:
            fprintf(wf,"\t%d [color=\"green\" shape=circle style=filled]\n",0);
            fprintf(wf,"\t%d -> %d\n",0,0+instructions[0].m_val);
            break;
    }
    fprintf(wf,"\t%d [color=\"magenta\" shape=circle style=filled]",instructionsCount);
    fprintf(wf,"}");
}


std::vector<Instruction> Parse(FILE* file)
{
    std::vector<Instruction> bootCode;
    bootCode.reserve(100);
    char opStr[4];
    int val;
    while(fscanf(file,"%s %d",opStr,&val) != EOF)
    {
        if(strcmp(opStr,"nop") == 0)
        {
            bootCode.push_back(Instruction{Type::nop, val});
        }
        else if(strcmp(opStr,"acc") == 0)
        {
            bootCode.push_back(Instruction{Type::acc, val});
        }
        else if(strcmp(opStr,"jmp") == 0)
        {
            bootCode.push_back(Instruction{Type::jmp, val});
        } else
            assert("Error: Unexpected Instruction type!");
    }
    return bootCode;
}

void ExecuteBootCode(std::vector<Instruction> &instructions)
{
    int global = 0, i = 0;
    int instructionCount = instructions.size();
    while(i < instructionCount)
    {
        switch(instructions[i].m_type)
        {
            case Type::jmp:
                i += instructions[i].m_val;
                break;
            case Type::acc:
                global += instructions[i].m_val;
                ++i;
                break;
            case Type::nop:
                ++i;
                break;
            default:
                assert("Unexpected instruction."); break;
        }
    }
    printf("Global %d\n",global);
}