#include <assert.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

int Part1(const char* filepath);
int Part2(const char* filepath);

uint32_t Hash(const char* str, int len);

int main()
{
    // const char* filepath = "testInput.txt";
    const char* filepath = "input.txt";
    {
        auto start = std::chrono::steady_clock::now();
        int sum = Part1(filepath);
        printf("Part 1 %d\n",sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }

    {
        auto start = std::chrono::steady_clock::now();
        int count = Part2(filepath);
        printf("Part 2 %d\n", count);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
    }
}

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_size = 23'000;
    char contents[k_size];

    fgets(contents, k_size, file);
    fclose(file);

    uint32_t sum = 0;

    char* next;
    char* token = strtok_s(contents, ",\n", &next);
    while(token != nullptr)
    {
        sum += Hash(token, strlen(token));
        // printf("token %12s %d\n", token, sum);
        token = strtok_s(NULL, ",\n", &next); // This is truly cursed
    }

    return sum;
}

uint32_t Hash(const char* str, int len)
{
    uint32_t val = 0;
    for(int i = 0; i < len; ++i)
    {
        val += str[i];
        val *= 17;
        val %= 256;
    }
    assert(val < 256);
    return val;
}

struct Lens
{
    char Label[16];
    uint8_t FocalLength;
    char Len;
};

struct Box
{
    std::vector<Lens> Lenses;

    int FindIndex(const char* label, int len)
    {
        for(int i = 0, size = Lenses.size(); i < size; ++i)
        {
            if(Lenses[i].Len == len && strncmp(Lenses[i].Label, label, len) == 0)
                return i;
        }
        return -1;
    }
};

int Part2(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_size = 23'000;
    char contents[k_size];

    fgets(contents, k_size, file);
    fclose(file);

    Box boxes[256];

    char* next;
    char* token = strtok_s(contents, ",\n", &next);
    while(token != nullptr)
    {
        char* ptr = strpbrk(token, "-=");
        uint32_t box = Hash(token, ptr - token);
        int lensIdx = boxes[box].FindIndex(token, ptr-token);
        if(*ptr == '=')
        {
            uint8_t focalLen = strtol(ptr+1, NULL, 10);
            if(lensIdx >= 0)
            {
                boxes[box].Lenses[lensIdx].FocalLength = focalLen;
                // printf("Swapped lens with token %s\n", token);
            }
            else
            {
                // printf("Adding lens with token %s to box %d\n", token, box);
                Lens lens = Lens{.FocalLength = focalLen};
                lens.Len = ptr - token;
                strncpy(lens.Label, token, lens.Len);
                boxes[box].Lenses.push_back(lens);
            }
        }
        else
        {
            if(lensIdx >= 0)
            {
                boxes[box].Lenses.erase(begin(boxes[box].Lenses) + lensIdx);
                // printf("Removed lens matching token %s\n", token);
            }
            // else
            // {
            //     printf("No Lens matching token found %s\n", token);
            // }
        }

        token = strtok_s(NULL, ",\n", &next); // This is truly cursed
    }

    int sum = 0;
    int count = 0;
    for(int i = 0; i < 256; ++i)
    {
        for(int j = 0, size = boxes[i].Lenses.size(); j < size; ++j)
        {
            ++count;
            sum += (i + 1) * (j+1) * boxes[i].Lenses[j].FocalLength;
            // printf("%3d Box %3d %8s %d %d\n", count, i + 1, boxes[i].Lenses[j].Label, j+1, boxes[i].Lenses[j].FocalLength);
        }
    }

    return sum;
}

