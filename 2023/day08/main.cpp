#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <unordered_map>

int Part1(const char* filepath);
uint64_t Part2(const char* filepath);

int main()
{
    // const char* filepath = "testInput3.txt";
    const char* filepath = "input.txt";
    {
        auto start = std::chrono::steady_clock::now();
        int stepCount = Part1(filepath);
        printf("Part 1 %d\n",stepCount);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t stepCount = Part2(filepath);
        printf("Part 2 %llu\n",stepCount);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
    }

}

uint16_t Pack(const char* code)
{
    uint16_t val = 0;
    for(int i = 0; i < 3; ++i) {
        val |= (code[i] - 'A') << 5 * (2 - i);
    }
    return val;
}

void Unpack(uint16_t val, char* code)
{
    for(int i = 0; i < 3; ++i) {
        code[i] = ((val & 0b11 << 5*(2-i)) >> 5 * (2 - i)) + 'A';
    }
}

struct Turns
{
    uint16_t Left;
    uint16_t Right;
};

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int size = 500;
    char contents[size];
    fscanf(file, "%s\n\n", contents);

    char origin[3], left[3], right[3];

    Turns map[UINT16_MAX / 2];
    // std::unordered_map<uint16_t, Turns> map;

    while(fscanf(file, "%3c = (%3c, %3c)\n", origin, left, right) != EOF)
    {
        uint16_t idx = Pack(origin);
        uint16_t leftIdx = Pack(left);
        uint16_t rightIdx = Pack(right);
        // printf("%s %s %s\n", origin, left, right);

        map[idx] = Turns{.Left = leftIdx, .Right = rightIdx};
    }
    fclose(file);

    uint16_t idx = 0;
    const uint16_t target = Pack("ZZZ");

    int instructionCount = strlen(contents);
    int stepCount = 0;

    while(idx != target)
    {
        char instruction = contents[stepCount % instructionCount];
        ++stepCount;
        switch (instruction) {
            case 'L':
                idx = map[idx].Left;
                break;
            case 'R':
                idx = map[idx].Right;
                break;
        }
    }

    return stepCount;
}


// Recursive methods runs out of memory
uint64_t gcdRecursive(uint64_t a, uint64_t b)
{
    // Everything divides 0
    if (a == 0)
        return b;
    if (b == 0)
        return a;

    // base case
    if (a == b)
        return a;

    // a is greater
    if (a > b)
        return gcdRecursive(a - b, b);
    return gcdRecursive(a, b - a);
}

int64_t gcd(int64_t a, int64_t b)
{
    while (b != 0)
    {
        int64_t tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

uint64_t Part2(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int size = 500;
    char contents[size];
    fscanf(file, "%s\n\n", contents);

    char origin[3], left[3], right[3];
    Turns map[UINT16_MAX / 2];
    // std::unordered_map<uint16_t, Turns> map;

    uint16_t paths[100];
    int cycleSize[100];
    int offset[100];
    memset(cycleSize, -1, sizeof(int) * 100);
    memset(offset, -1, sizeof(int) * 100);
    int pathCount = 0;
    uint16_t line = 0;

    while(fscanf(file, "%3c = (%3c, %3c)\n", origin, left, right) != EOF)
    {
        uint16_t idx = Pack(origin);
        uint16_t leftIdx = Pack(left);
        uint16_t rightIdx = Pack(right);

        map[idx] = Turns{.Left = leftIdx, .Right = rightIdx};

        if((~idx & 0b111) == 0b111) {
            paths[pathCount] = idx;
            ++pathCount;
        }
        ++line;
    }
    fclose(file);

    int instructionCount = strlen(contents);
    int stepCount = 0;

    uint16_t targetMask = ('Z' - 'A');

    int targetReached = 0;
    while(pathCount != targetReached)
    {
        char instruction = contents[stepCount % instructionCount];
        ++stepCount;
        for(int i = 0; i < pathCount; ++i) {
            switch (instruction) {
                case 'L':
                    paths[i] = map[paths[i]].Left;
                break;
                case 'R':
                    paths[i] = map[paths[i]].Right;
                break;
            }

            if((paths[i] & targetMask) == targetMask)
            {
                if(offset[i] == -1)
                {
                    offset[i] = stepCount;
                }
                else if(cycleSize[i] == -1 )
                {
                    cycleSize[i] = stepCount - offset[i];
                    ++targetReached;
                }
            }

        }
    }

    int64_t m = cycleSize[0];
    for(int i = 1; i < pathCount; ++i)
    {
        uint64_t g = gcd(m, cycleSize[i]);
        m = m * (cycleSize[i] / g); // Least common multiple
    }
    return m;
}