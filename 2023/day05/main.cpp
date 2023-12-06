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
        auto start = std::chrono::steady_clock::now();
        uint64_t sum = Part1(filepath);
        printf("Part 1 %lld\n", sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }

    return 0;
}

struct Map
{
    uint64_t Dest;
    uint64_t Src;
    uint64_t Range;
};

struct MapSet
{
    Map* Map;
    int Count;
};

uint64_t Part1(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");
    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return -1;
    }

    const int setCapacity = 20;
    const int mapCapacity = 1000;
    MapSet sets[setCapacity];
    Map maps[mapCapacity];
    memset(sets, 0, sizeof(MapSet) * setCapacity);
    memset(maps, 0, sizeof(Map) * mapCapacity);
    int nextMap = 0;

    int setCount = 0;

    const int seedCapacity = 50;
    uint64_t seeds[seedCapacity];
    int seedCount = 0;
    memset(seeds, 0, sizeof(uint64_t) * seedCapacity);

    const int size = 300;
    char contents[size];
    int offset = 0;

    if(fgets(contents, size, file) != nullptr)
    {
        sscanf(contents, "seeds: %n", &offset);
        char* ptr = contents + offset;
        int i = 0;
        while(*ptr != '\n') {
            sscanf(ptr, "%lld%n", &seeds[i], &offset);
            ptr += offset;
            ++i;
        }
        seedCount = i;
    }
    else {
        printf("Failed to read first line.");
        return -1;
    }

    setCount = 0;
    while(fgets(contents, size, file) != nullptr)
    {
        if(contents[0] == '\n')
        {
            continue;
        }
        else if(contents[0] >= '0' && contents[0] <= '9')
        {
            ++sets[setCount-1].Count;
            sscanf(contents, "%lld %lld %lld", &maps[nextMap].Dest, &maps[nextMap].Src, &maps[nextMap].Range);
            ++nextMap;
        }
        else
        {
            Map& map = maps[nextMap];
            sets[setCount].Map = &map;
            ++setCount;
        }
    }
    fclose(file);

    uint64_t minLoc = UINT64_MAX;
    for(int i = 0; i < seedCount; ++i)
    {
        uint64_t seed = seeds[i];
        for(int j = 0; j < setCount; ++j)
        {
            for(int k = 0; k < sets[j].Count; ++k)
            {
                Map& map = sets[j].Map[k];
                if(seed >= map.Src && seed < (map.Src + map.Range))
                {
                    seed = seed - map.Src + map.Dest;
                    break;
                }
            }
        }
        printf("Seed %lld: %lld\n", i, seed);
        minLoc = seed < minLoc ? seed : minLoc;
    }

    return minLoc;
}
