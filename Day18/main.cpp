#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <numeric>

void Part1(FILE* file);
void Part2(FILE* file);

int main(int argc, char *argv[]) {
    auto start = std::chrono::steady_clock::now();

    if(argc < 3)
    {
        printf("Not enough args!\nArgs should be part number (1 and 2) and then the input file\n");
        exit(2);
    }
    else if(argc > 3)
    {
        printf("Too many args!\nArgs should be part number (1 and 2) and then the input file\n");
        exit(2);
    }

    FILE* file = fopen(argv[2],"r");

    if(file)
    {
        if(*argv[1] == '1')
        {
            Part1(file);
        }
        else if(*argv[1] == '2')
        {
            Part2(file);
        }
    }
    else
    {
        printf("Failed to open files\n");
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    fclose(file);
    return 0;
}

struct Bracket
{
    int first;
    int second;
    int val;
};

enum class op
{
    NONE,
    ADD,
    MULT
};

void Part1(FILE* file)
{
    char buf[255];
    std::vector<Bracket> brackets;
    unsigned long sum = 0;
    while(fgets(buf,255,file) != nullptr)
    {
        // Find all brackets
        for(int i = 0; i < 255 && buf[i] != '\n' && buf[i] != '\0'; ++i)
        {
            switch (buf[i])
            {
                case '(':
                    brackets.push_back({i,-1,0});
                    break;
                case ')':
                    for(int j = brackets.size() - 1; j >= 0; --j)
                    {
                        if(brackets[j].second == -1)
                        {
                            brackets[j].second = i;
                            break;
                        }
                    }
                    break;
            }
        }

        // Calculate each of the brackets backwards
        for(int i = brackets.size()-1; i >= 0; --i)
        {
            int val1 = -1; op op; int internalBrack = 0;
            for(int j = brackets[i].first+1; j < brackets[i].second;)
            {
                if(buf[j] == '(')
                {
                    switch (op)
                    {
                        case op::NONE:
                            val1 = brackets[i + ++internalBrack].val;
                            break;
                        case op::ADD:
                            val1 += brackets[i + ++internalBrack].val;
                            op = op::NONE;
                            break;
                        case op::MULT:
                            val1 *= brackets[i + ++internalBrack].val;
                            op = op::NONE;
                            break;
                    }
                    j = brackets[i + internalBrack].second;
                }
                else if(buf[j] == ')')
                {
                    ++j;
                    continue;
                }
                else if(buf[j] == '*')
                {
                    ++j;
                    op = op::MULT;
                }
                else if(buf[j] == '+')
                {
                    ++j;
                    op = op::ADD;
                }
                else if(buf[j] == ' ')
                {
                    ++j;
                    continue;
                }
                else
                {
                    int tmp, bytesRead;
                    sscanf(&buf[j],"%d%n",&tmp,&bytesRead);
                    j += bytesRead;
                    switch (op)
                    {
                        case op::NONE:
                            val1 = tmp;
                            break;
                        case op::ADD:
                            val1 += tmp;
                            op = op::NONE;
                            break;
                        case op::MULT:
                            val1 *= tmp;
                            op = op::NONE;
                            break;
                    }
                }
            }
            brackets[i].val = val1;
        }

        // Calculate line
        unsigned long val1 = 0; op op;
        for(int i = 0; i < 255 && buf[i] != '\n' && buf[i] != '\0';)
        {
            if(buf[i] == '(')
            {
                auto brack = std::find_if(begin(brackets),end(brackets),[&](auto& x){return x.first == i;});
                switch(op)
                {
                    case op::NONE:
                        val1 = brack->val;
                        break;
                    case op::ADD:
                        val1 += brack->val;
                        op = op::NONE;
                        break;
                    case op::MULT:
                        val1 *= brack->val;
                        op = op::NONE;
                        break;
                }
                i = brack->second+1;
            }
            else if(buf[i] == ')')
            {
                ++i;
                continue;
            }
            else if(buf[i] == '*')
            {
                ++i;
                op = op::MULT;
            }
            else if(buf[i] == '+')
            {
                ++i;
                op = op::ADD;
            }
            else if(buf[i] == ' ')
            {
                ++i;
                continue;
            }
            else
            {
                unsigned long tmp; int bytesRead;
                sscanf(&buf[i],"%lu%n",&tmp,&bytesRead);
                i += bytesRead;
                switch (op)
                {
                    case op::NONE:
                        val1 = tmp;
                        break;
                    case op::ADD:
                        val1 += tmp;
                        op = op::NONE;
                        break;
                    case op::MULT:
                        val1 *= tmp;
                        op = op::NONE;
                        break;
                }
            }
        }
//        printf("Total %d\n",val1);
        sum += val1;
        printf("Sum = %lu\n",sum);
        brackets.clear();
    }
    printf("Sum = %lu\n",sum);
}

void Part2(FILE* file)
{
    printf("Not implemented!\n");
    exit(2);
}