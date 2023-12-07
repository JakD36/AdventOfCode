#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

int64_t Part1(const char* filepath);
int64_t Part2(const char* filepath);

int main()
{
    // char filepath[] = "testInput.txt";
    char filepath[] = "input.txt";

    {
        auto start = std::chrono::steady_clock::now();
        int64_t total = Part1(filepath);
        printf("Part 1 %ld\n", total);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }

    {
        auto start = std::chrono::steady_clock::now();
        int64_t total = Part2(filepath);
        printf("Part 2 %ld\n", total);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
    }

    return 0;
}

int64_t CountWaysToWin(int64_t time, int64_t distance)
{
    // roots of quadratic
    // -b is cancelled out across the board
    int64_t part1 = time * time - 4 * distance;
    if(part1 < 0) {
        return 0;
        // complex roots, no solution
    }
    else if(part1 == 0) {
        // 1 solution
        return 1;
    }
    else {
        double root1 = (time - sqrt((double)part1)) * 0.5f;
        double root2 = (time + sqrt((double)part1)) * 0.5f;

        int64_t canWin = floor(root2) - ceil(root1) + 1;
        // If we didn't add 1 to the desired distance we would need to check for the fract component == 0
        // if(root1 - floor(root1) < 0.001f)
        //     --canWin;
        // if(root2 - floor(root2) < 0.001f)
        //     --canWin;
        return canWin;
    }
}

int64_t Part1(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");
    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return -1;
    }

    const int size = 200;
    char contents[size];

    // Parse Times
    if(fgets(contents, size, file) == nullptr) {
        printf("Failed to read the first line\n");
        return -1;
    }

    const int raceCapacity = 200;
    int raceCount = 0;
    int times[raceCapacity];

    int offset = 0;
    sscanf(contents, "Time: %n", &offset);
    char* ptr = contents + offset;
    while(*ptr != '\n')
    {
        sscanf(ptr, "%i%n", &times[raceCount], &offset);
        ptr += offset;
        ++raceCount;
    }

    // Parse Distances
    if(fgets(contents, size, file) == nullptr) {
        printf("Failed to read the second line\n");
        return -1;
    }

    raceCount = 0;
    int distances[raceCapacity];
    sscanf(contents, "Distance: %n", &offset);
    ptr = contents + offset;
    while(*ptr != '\n')
    {
        sscanf(ptr, "%i%n", &distances[raceCount], &offset);
        ptr += offset;
        ++raceCount;
    }

    fclose(file);

    int total = 1;
    for(int i = 0; i < raceCount; ++i)
    {
        int time = times[i];
        int distance = distances[i] + 1;

        total *= CountWaysToWin(time, distance);
    }

    return total;
}


int64_t Part2(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");
    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return -1;
    }

    const int size = 200;
    char contents[size];

    // Parse Times
    if(fgets(contents, size, file) == nullptr) {
        printf("Failed to read the first line\n");
        return -1;
    }

    char concatTime[20];
    memset(concatTime, '\0', sizeof(char) * 20);
    char buffer[20];

    int offset = 0;
    sscanf(contents, "Time: %n", &offset);
    char* ptr = contents + offset;
    while(*ptr != '\n')
    {
        sscanf(ptr, "%s%n", &buffer, &offset);
        strcat(concatTime, buffer);
        ptr += offset;
    }

    // Parse Distances
    if(fgets(contents, size, file) == nullptr) {
        printf("Failed to read the second line\n");
        return -1;
    }

    char concatDistance[20];
    memset(concatDistance, '\0', sizeof(char) * 20);
    sscanf(contents, "Distance: %n", &offset);
    ptr = contents + offset;
    while(*ptr != '\n')
    {
        sscanf(ptr, "%s%n", &buffer, &offset);
        strcat(concatDistance, buffer);
        ptr += offset;
    }

    fclose(file);

    int64_t time = strtol(concatTime, nullptr, 10);
    int64_t distance = strtol(concatDistance, nullptr, 10);
    return CountWaysToWin(time, distance + 1);
}
