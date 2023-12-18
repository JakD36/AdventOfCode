#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

int Part1(const char* filepath);
// uint64_t Part2(const char* filepath);

int main()
{
    const char* filepath = "testInput.txt";
    // const char* filepath = "input.txt";
    {
        auto start = std::chrono::steady_clock::now();
        int sum = Part1(filepath);
        printf("Part 1 %d\n",sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }
    //
    // {
    //     auto start = std::chrono::steady_clock::now();
    //     uint64_t count = Part2(filepath);
    //     printf("Part 2 %lld\n", count);
    //     auto end = std::chrono::steady_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //     printf("Part 2 %lld microseconds\n", duration);
    // }
}

struct Island
{
    char* ptr;
    int Len;
};

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_size = 500;
    char contents[k_size];

    char input[k_size];
    int numbers[100];

    Island islands[100];


    int sum = 0;
    while(fgets(contents, k_size, file) != nullptr)
    {
        int islandCount = 0;
        int readOffset = 0;
        int writeOffset = 0;
        char* ptr = contents;
        // Find the islands
        while(true)
        {
            if(*ptr == '.')
            {
                sscanf(ptr, "%[.]%n", input + writeOffset, &readOffset);
            }
            else
            {
                if(sscanf(ptr, "%[^. 1-9]%n", input + writeOffset, &readOffset) != 1)
                {
                    break;
                }
                writeOffset += readOffset;

                islands[islandCount] = {input + writeOffset, readOffset};
                ++islandCount;
            }
            ptr += readOffset;
        }


        // Parse the arrangements
        int count = 0;
        while(sscanf(ptr, "%d%n", &numbers[count], &readOffset) == 1)
        {
            ptr += readOffset;
            ++count;
            if(*ptr != '\n')
                ptr += 1;
            else
                break;
        }

        bool islandsTaken[100];
        bool arrangementsTaken[100];
        memset(islandsTaken, false, sizeof(bool) * 100);
        memset(arrangementsTaken, false, sizeof(bool) * 100);

        int pairsRemoved = 0;
        //
        // // eliminate anything we know can't shift
        // for(int i = 0; i < count; ++i)
        // {
        //     int num = numbers[i];
        //     for(int j = 0; j < islandCount; ++j)
        //     {
        //         if(islandsTaken[j] == false && islands[j].Len == num)
        //         {
        //             islandsTaken[j] = true;
        //             arrangementsTaken[i] = true;
        //             ++pairsRemoved;
        //             break;
        //         }
        //     }
        // }
        //
        // if(pairsRemoved == islandCount)
        // {
        //     sum += 0;
        //     continue;
        // }
        //
        //
        // for(int i = 0; i < count; ++i)
        // {
        //     if(arrangementsTaken[i] == false)
        //     numbers
        // }

    }

    fclose(file);


    return sum;
}

