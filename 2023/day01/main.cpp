#include <chrono>
#include <cinttypes>
#include <cstdio>
#include <cstring>

uint64_t Part1(const char* filepath);
uint64_t Part2(const char* filepath);
int8_t TryFindNumber(const char* contents, int index);

int main()
{
    // char filepath[] = "testInput.txt";
    // char filepath[] = "testInput2.txt";
    char filepath[] = "input.txt";

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t sum = Part1(filepath);
        printf("Part 1 %" PRIu64 "\n", sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("%lld microseconds\n", duration);
    }
    

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t sum = Part2(filepath);
        printf("Part 2 %" PRIu64 "\n", sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("%lld microseconds\n", duration);
    }

    return 0;
}

uint64_t Part1(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");

    if(file == nullptr)
    {
        printf("Failed to open %s",filepath);
        return 1;
    }

    uint64_t sum = 0;
    char* contents = new char[100];
    while(fgets(contents, 100, file) != nullptr)
    {
        int lhs = -1;
        int rhs = 0;
        for(int i = 0; i < 100; ++i)
        {
            if(contents[i] == '\0' || contents[i] == '\n')
                break;

            int8_t val = contents[i] - '0';
            if(val >= 0 && val < 10)
            {
                if(lhs < 0)
                {
                    lhs = val;
                }
                rhs = val;
            }
        }
        // printf("%d\n", lhs * 10 + rhs);
        sum += lhs * 10 + rhs;
    }

    fclose(file);
    return sum;
}

uint64_t Part2(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");

    if(file == nullptr)
    {
        printf("Failed to open %s",filepath);
        return 1;
    }

    uint64_t sum = 0;
    char* contents = new char[100];
    while(fgets(contents, 100, file) != nullptr)
    {
        int lhs = -1;
        int rhs = 0;
        for(int i = 0; i < 100; ++i)
        {
            if(contents[i] == '\0' || contents[i] == '\n')
                break;

            int8_t val = contents[i] - '0';
            if(val >= 0 && val < 10)
            {
                if(lhs < 0)
                {
                    lhs = val;
                }
                rhs = val;
            }
            else
            {
                val = TryFindNumber(contents, i);
                if(val > -1)
                {
                    if(lhs < 0)
                    {
                        lhs = val;
                    }
                    rhs = val;
                }
            }
        }
        // printf("%d\n", lhs * 10 + rhs);
        sum += lhs * 10 + rhs;
    }

    fclose(file);
    return sum;
}

int8_t TryFindNumber(const char* contents, int index)
{
    if(index >= 2)
    {
        const char* ptr = &contents[index-2];
        if(strncmp(ptr, "one", 3) == 0)
            return 1;
        if(strncmp(ptr, "two", 3) == 0)
            return 2;
        if(strncmp(ptr, "six", 3) == 0)
            return 6;
    }
    if(index >= 3)
    {
        const char* ptr = &contents[index-3];
        if(strncmp(ptr, "four", 4) == 0)
            return 4;
        if(strncmp(ptr, "five", 4) == 0)
            return 5;
        if(strncmp(ptr, "nine", 4) == 0)
            return 9;
    }
    if(index >= 4)
    {
        const char* ptr = &contents[index-4];
        if(strncmp(ptr, "three", 5) == 0)
            return 3;
        if(strncmp(ptr, "seven", 5) == 0)
            return 7;
        if(strncmp(ptr, "eight", 5) == 0)
            return 8;
    }
    return -1;
}