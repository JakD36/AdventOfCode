#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

uint64_t Part1(const char* filepath);
uint64_t Part2(const char* filepath);

int main()
{
    // char filepath[] = "testInput.txt";
    char filepath[] = "input.txt";

    {

        {
            auto start = std::chrono::steady_clock::now();
            uint64_t sum = Part1(filepath);
            printf("Part 1 %lld\n", sum);
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            printf("Part 1 %lld microseconds\n", duration);
        }

        {
            auto start = std::chrono::steady_clock::now();
            uint64_t sum = Part2(filepath);
            printf("Part 2 %lld\n", sum);
            auto end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            printf("Part 2 %lld microseconds\n", duration);
        }
    }

    return 0;
}

uint64_t Part1(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");
    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return -1;
    }

    int line = 0;
    int winningNumbers[30];
    int myNumbers[30];

    uint64_t sum = 0;
    const int size = 200;
    char contents[size];

    while(fgets(contents, size, file) != nullptr)
    {
        int offset;
        sscanf(contents, "Card %i: %n", &line, &offset);
        char* ptr = contents + offset;
        int totalWinningNumbers = 0;
        int totalMyNumbers = 0;

        while(sscanf(ptr, "%i%n", &winningNumbers[totalWinningNumbers], &offset) == 1)
        {
            ++totalWinningNumbers;
            ptr += offset;
        }
        ptr += 3; // jump over " | "
        while(sscanf(ptr, "%i%n", &myNumbers[totalMyNumbers], &offset) == 1)
        {
            ++totalMyNumbers;
            ptr += offset;
        }

        uint64_t winMask[2] = {0, 0};
        uint64_t testMask[2] = {0, 0};
        uint64_t checkMask[2] = {0, 0};

        for(int i = 0; i < totalWinningNumbers; ++i) {
            if(winningNumbers[i] >= 64)
                winMask[1] |= 1llU << (winningNumbers[i] - 64);
            else
                winMask[0] |= 1llU << winningNumbers[i];
        }

        for(int i = 0; i < totalMyNumbers; ++i) {
            if(myNumbers[i] >= 64)
                testMask[1] |= 1llU << (myNumbers[i] - 64);
            else
                testMask[0] |= 1llU << myNumbers[i];
        }

        int count = 0;
        // for(int i = 0; i < 64; ++i) {
        //     count += (winMask[0] & 1llU << i) != 0;
        // }
        // for(int i = 0; i < 64; ++i) {
        //     count += (winMask[1] & 1llU << i) != 0;
        // }
        // printf("Win Mask %i\n", count);
        //
        // count = 0;
        // for(int i = 0; i < 64; ++i) {
        //     count += (testMask[0] & 1llU << i) != 0;
        // }
        // for(int i = 0; i < 64; ++i) {
        //     count += (testMask[1] & 1llU << i) != 0;
        // }
        // printf("Test MAsk %i\n", count);
        // count = 0;

        checkMask[0] = winMask[0] & testMask[0];
        checkMask[1] = winMask[1] & testMask[1];
        for(int i = 0; i < 64; ++i) {
            count += (checkMask[0] & 1llU << i) != 0;
        }
        for(int i = 0; i < 64; ++i) {
            count += (checkMask[1] & 1llU << i) != 0;
        }

        if(count > 0)
            sum += pow(2, count-1);
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
        printf("Failed to open %s\n",filepath);
        return -1;
    }

    int line = 0;
    int winningNumbers[30];
    int myNumbers[30];


    uint64_t sum = 0;
    const int size = 200;
    char contents[size];

    int bonusCards[255];
    memset(bonusCards, 0, sizeof(int) * 255);

    while(fgets(contents, size, file) != nullptr)
    {
        int offset;
        sscanf(contents, "Card %i: %n", &line, &offset);
        char* ptr = contents + offset;
        int totalWinningNumbers = 0;
        int totalMyNumbers = 0;

        while(sscanf(ptr, "%i%n", &winningNumbers[totalWinningNumbers], &offset) == 1)
        {
            ++totalWinningNumbers;
            ptr += offset;
        }
        ptr += 3; // jump over " | "
        while(sscanf(ptr, "%i%n", &myNumbers[totalMyNumbers], &offset) == 1)
        {
            ++totalMyNumbers;
            ptr += offset;
        }

        uint64_t winMask[2] = {0, 0};
        uint64_t testMask[2] = {0, 0};
        uint64_t checkMask[2] = {0, 0};

        for(int i = 0; i < totalWinningNumbers; ++i) {
            if(winningNumbers[i] >= 64)
                winMask[1] |= 1llU << (winningNumbers[i] - 64);
            else
                winMask[0] |= 1llU << winningNumbers[i];
        }

        for(int i = 0; i < totalMyNumbers; ++i) {
            if(myNumbers[i] >= 64)
                testMask[1] |= 1llU << (myNumbers[i] - 64);
            else
                testMask[0] |= 1llU << myNumbers[i];
        }

        int count = 0;

        checkMask[0] = winMask[0] & testMask[0];
        checkMask[1] = winMask[1] & testMask[1];
        for(int i = 0; i < 64; ++i) {
            count += (checkMask[0] & 1llU << i) != 0;
        }
        for(int i = 0; i < 64; ++i) {
            count += (checkMask[1] & 1llU << i) != 0;
        }

         for(int i = line; i < line + count; ++i) {
             bonusCards[i] += (bonusCards[line-1] + 1);
         }
    }

    for(int i = 0; i < line; ++i)
    {
        sum += 1 + bonusCards[i];
    }

    fclose(file);
    return sum;
}
