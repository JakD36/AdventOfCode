#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

int Part1(const char* filepath);
uint64_t Part2(const char* filepath);

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

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t count = Part2(filepath);
        printf("Part 2 %lld\n", count);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
    }
}

struct int2
{
    int x;
    int y;
};

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_maxRows = 140, k_maxCols = 142;
    char contents[k_maxCols];

    bool colContains[k_maxCols], rowContains[k_maxRows];
    memset(colContains, false, sizeof(bool) * k_maxCols);
    memset(rowContains, false, sizeof(bool) * k_maxRows);

    int2 galaxies[500];
    int count = 0;

    if(fgets(contents, k_maxCols, file) == nullptr)
    {
        printf("Failed to read the first line.");
        return -1;
    }
    int width = strlen(contents) - 1;

    for(int i = 0; i < width; ++i)
    {
        if(contents[i] == '#')
        {
            rowContains[0] = true;
            colContains[i] = true;
            galaxies[count] = {i, 0};
            ++count;
        }
    }

    int rowCount = 1;
    while(fgets(contents, k_maxCols, file) != nullptr)
    {
        for(int i = 0; i < width; ++i)
        {
            if(contents[i] == '#')
            {
                rowContains[rowCount] = true;
                colContains[i] = true;
                galaxies[count] = {i, rowCount};
                ++count;
            }
        }
        ++rowCount;
    }
    fclose(file);

    int sum = 0;
    // int pairs = 0;
    for(int i = 0; i < count - 1; ++i)
    {
        for(int j = i + 1; j < count; ++j)
        {
            // ++pairs;
            int2& a = galaxies[i];
            int2& b = galaxies[j];
            int minX, minY, maxX, maxY;
            if(a.x < b.x)
            {
                minX = a.x;
                maxX = b.x;
            }
            else
            {
                minX = b.x;
                maxX = a.x;
            }
            if(a.y < b.y)
            {
                minY = a.y;
                maxY = b.y;
            }
            else
            {
                minY = b.y;
                maxY = a.y;
            }

            int diff = maxX - minX + maxY - minY;

            for(int k = minX + 1; k < maxX; ++k)
                if(colContains[k] == false)
                    ++diff;
            for(int k = minY + 1; k < maxY; ++k)
                if(rowContains[k] == false)
                    ++diff;
            sum += diff;
        }
    }

    return sum;
}


uint64_t Part2(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_maxRows = 140, k_maxCols = 142;
    char contents[k_maxCols];

    bool colContains[k_maxCols], rowContains[k_maxRows];
    memset(colContains, false, sizeof(bool) * k_maxCols);
    memset(rowContains, false, sizeof(bool) * k_maxRows);

    int2 galaxies[500];
    int count = 0;

    if(fgets(contents, k_maxCols, file) == nullptr)
    {
        printf("Failed to read the first line.");
        return -1;
    }
    int width = strlen(contents) - 1;

    for(int i = 0; i < width; ++i)
    {
        if(contents[i] == '#')
        {
            rowContains[0] = true;
            colContains[i] = true;
            galaxies[count] = {i, 0};
            ++count;
        }
    }

    int rowCount = 1;
    while(fgets(contents, k_maxCols, file) != nullptr)
    {
        for(int i = 0; i < width; ++i)
        {
            if(contents[i] == '#')
            {
                rowContains[rowCount] = true;
                colContains[i] = true;
                galaxies[count] = {i, rowCount};
                ++count;
            }
        }
        ++rowCount;
    }
    fclose(file);

    uint64_t sum = 0;
    // int pairs = 0;
    const int k_expandBy = 1'000'000 - 1;
    for(int i = 0; i < count - 1; ++i)
    {
        for(int j = i + 1; j < count; ++j)
        {
            // ++pairs;
            int2& a = galaxies[i];
            int2& b = galaxies[j];
            int minX, minY, maxX, maxY;
            if(a.x < b.x)
            {
                minX = a.x;
                maxX = b.x;
            }
            else
            {
                minX = b.x;
                maxX = a.x;
            }
            if(a.y < b.y)
            {
                minY = a.y;
                maxY = b.y;
            }
            else
            {
                minY = b.y;
                maxY = a.y;
            }

            uint64_t diff = maxX - minX + maxY - minY;

            for(int k = minX + 1; k < maxX; ++k)
                if(colContains[k] == false)
                    diff += k_expandBy;
            for(int k = minY + 1; k < maxY; ++k)
                if(rowContains[k] == false)
                    diff += k_expandBy;
            sum += diff;
        }
    }

    return sum;
}
