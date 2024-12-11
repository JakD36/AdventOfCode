#include <cstdio>
#include <cstring>
#include <cmath>
#include <chrono>

void Part1(const char* filepath);
void Part2(const char* filepath);

int main()
{
    // const char* filepath = "testInput.txt";
    const char* filepath = "input.txt";

    Part1(filepath);
    Part2(filepath);
    return 0;
}

void Part1(const char* filepath)
{
    auto start = std::chrono::high_resolution_clock::now();

    FILE* f = fopen(filepath, "r");
    if (f == nullptr)
    {
        printf("Failed to read filepath: %s", filepath);
        return;
    };

    const int bufferSize = 256;
    char buffer[bufferSize];

    uint64_t sum = 0;

    while (fgets(buffer, bufferSize, f) != nullptr)
    {
        int len = strlen(buffer);
        uint64_t result;
        int offset;
        const int maxVars = 32;
        uint64_t vars[maxVars];
        
        char* ptr = buffer;
        sscanf(ptr, "%llu:%n", &result, &offset);
        ptr += offset;

        int varCount = 0;
        while(varCount < maxVars && sscanf(ptr, "%llu%n", &vars[varCount], &offset) > 0)
        {
            ptr += offset;
            ++varCount;
        }

        int combinations = pow(2, varCount - 1);

        for(int i = 0; i < combinations; ++i)
        {
            uint64_t currentResult = result;
            bool checkFinalResult = true;
            for(int j = varCount - 1; j > 0; --j)
            {
                // Can go backwards to take advantage of / resulting in floating point numbers to early return
                if((i & (1 << (varCount - 1 - j))) != 0) // binary representative of our combinations represents which operation we should use
                {
                    if(currentResult % vars[j] > 0) // gives us a chance to early return as we only want whole numbers!
                    {
                        checkFinalResult = false;
                        break; // Failed to find the answer   
                    }
                    currentResult /= vars[j];
                }
                else
                {
                    currentResult -= vars[j];
                }
            }
            if(checkFinalResult && currentResult == vars[0])
            {
                sum += result;
                break;
            }
        }
    }
    fclose(f);

    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf("Part 1 %llu in %llu us\n", sum, elapsed);
}


void Part2(const char* filepath)
{
    auto start = std::chrono::high_resolution_clock::now();
    FILE* f = fopen(filepath, "r");
    if (f == nullptr)
    {
        printf("Failed to read filepath: %s", filepath);
        return;
    };

    const int bufferSize = 256;
    char buffer[bufferSize];

    uint64_t sum = 0;

    while (fgets(buffer, bufferSize, f) != nullptr)
    {
        int len = strlen(buffer);
        uint64_t result;
        int offset;
        const int maxVars = 32;
        uint64_t vars[maxVars];
        uint64_t tmpVars[maxVars];
        
        char* ptr = buffer;
        sscanf(ptr, "%llu:%n", &result, &offset);
        ptr += offset;

        int varCount = 0;
        while(varCount < maxVars && sscanf(ptr, "%llu%n", &vars[varCount], &offset) > 0)
        {
            ptr += offset;
            ++varCount;
        }

        int combinations = pow(4, varCount - 1);

        for(int i = 0; i < combinations; ++i)
        {
            // save me running this loop 4 times for each operation, when I only have 3 types of operations to check
            // if (i != 0 && i%3 == 0)
                // continue;

            memcpy(tmpVars, vars, sizeof(uint64_t) * varCount);
            
            uint64_t currentResult = tmpVars[0];
            for(int j = 1; j < varCount; ++j)
            {
                int mask1 = (1 << (2 * (j-1)));
                int mask2 = (2 << (2 * (j-1)));
                if((i & mask2) == mask2)
                {
                    currentResult *= tmpVars[j];
                }
                else if((i & mask1) == mask1)
                {
                    // Combine the two values
                    uint64_t mul = pow(10.0, floor(log10((double)tmpVars[j])+1));
                    currentResult = (currentResult * mul) + tmpVars[j];
                }
                else
                {
                    currentResult += tmpVars[j];
                }
            }
            if(currentResult == result)
            {
                sum += result;
                break;
            }
        }
    }
    fclose(f);

    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf("Part 2 %llu in %llu us\n", sum, elapsed);
}