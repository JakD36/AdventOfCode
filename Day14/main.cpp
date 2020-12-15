#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <vector>

void Part1(FILE* file);
void Part2(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Part2(file);
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
    char buffer[255];
    std::unordered_map<long,long> mem;
    while(fgets(buffer,255,file) != nullptr)
    {
        char maskBuffer[36];
        unsigned long loc = 0, val = 0;
        if(sscanf(buffer,"mask = %s",maskBuffer) == 1) { }
        else if(sscanf(buffer,"mem[%ld] = %ld",&loc,&val) == 2)
        {
            for(int i = 35; i >= 0; --i)
            {
                if(maskBuffer[i] != 'X')
                {
                    if(maskBuffer[i] == '1')
                        val |= 1ul << (35-i);
                    else
                        val &= ~(1ul << (35-i));
                }
            }
            mem[loc] = val;
        }
    }
    unsigned long sum = 0;
    for(auto& x : mem)
    {
        sum += x.second;
    }

    printf("%ld\n",sum);
}

void Part2(FILE* file)
{
    char buffer[255];
    std::unordered_map<unsigned long,unsigned long> mem;
    while(fgets(buffer,255,file) != nullptr)
    {
        char maskBuffer[36];
        unsigned long loc = 0, val = 0;
        if(sscanf(buffer,"mask = %s",maskBuffer) == 1) { }
        else if(sscanf(buffer,"mem[%ld] = %ld",&loc,&val) == 2)
        {
            std::vector<unsigned long> addr = {loc};
            for(int i = 35; i >= 0; --i)
            {
                switch(maskBuffer[i])
                {
                    case 'X':
                    {
                        unsigned int count = addr.size();
                        addr.reserve(count * 2);
                        for(unsigned int j = 0; j < count; ++j)
                        {
                            unsigned long bitAnd = ~(1ul << (35-i));
                            unsigned long bitOr = 1ul << (35-i);
                            addr[j] |= bitOr;
                            addr.push_back(addr[j] & bitAnd);
                        }
                        break;
                    }
                    case '1':
                    {
                        unsigned long bit = 1ul << (35-i);
                        for(auto& x : addr)
                        {
                            x |= bit;
                        }
                        break;
                    }
                }
            }
            for(auto& x : addr)
            {
                mem[x] = val;
            }
        }
    }
    unsigned long sum = 0;
    for(auto& x : mem)
    {
        sum += x.second;
    }

    printf("%ld\n",sum);
}