#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

uint64_t Part1(const char* filepath);
uint64_t Part2BruteForce(const char* filepath);
uint64_t Part2(const char* filepath);

int main()
{
    // char filepath[] = "testInput.txt";
    char filepath[] = "input.txt";

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t min = Part1(filepath);
        printf("Part 1 %lld\n", min);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }

    // { // Just for testing the answers of part 2, against test input
    //     auto start = std::chrono::steady_clock::now();
    //     uint64_t min = Part2BruteForce(filepath);
    //     printf("Part 2 Brute %lld\n", min);
    //     auto end = std::chrono::steady_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //     printf("Part 2 brute %lld microseconds\n", duration);
    // }

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t min = Part2(filepath);
        printf("Part 2 %lld\n", min);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
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

struct SeedRange
{
    uint64_t Start;
    uint64_t Len;
};

struct SeedSet
{
    uint64_t Start;
    uint64_t End;
    int Born;
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
        uint64_t& seed = seeds[i];
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
        // printf("Seed %lld: %lld\n", i, seed);
        minLoc = seed < minLoc ? seed : minLoc;
    }

    return minLoc;
}

uint64_t Part2BruteForce(const char* filepath)
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

    const int seedCapacity = 10'000;
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
            uint64_t start = 0, range = 0;
            sscanf(ptr, "%lld %lld%n", &start, &range, &offset);
            ptr += offset;
            for(int j = 0; j < range; ++j, ++i) {
                seeds[i] = start + j;
            }
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
        uint64_t& seed = seeds[i];
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
        minLoc = seed < minLoc ? seed : minLoc;
    }

    // std::vector<uint64_t> vec;
    // for(int i = 0; i < seedCount; ++i) {
    //     vec.push_back(seeds[i]);
    // }
    // std::sort(std::begin(vec), std::end(vec));
    // for(int i = 0; i < vec.size(); ++i) {
    //     printf("%lld ", vec[i]);
    // }
    // printf("\n");

    return minLoc;
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

    const int setCapacity = 20;
    const int mapCapacity = 1000;
    MapSet sets[setCapacity];
    Map maps[mapCapacity];
    memset(sets, 0, sizeof(MapSet) * setCapacity);
    memset(maps, 0, sizeof(Map) * mapCapacity);
    int nextMap = 0;

    int setCount = 0;

    const int seedCapacity = 2000;
    SeedSet seeds[seedCapacity];
    int seedCount = 0;
    memset(seeds, 0, sizeof(SeedSet) * seedCapacity);

    const int size = 300;
    char contents[size];
    int offset = 0;

    if(fgets(contents, size, file) != nullptr)
    {
        sscanf(contents, "seeds: %n", &offset);
        char* ptr = contents + offset;
        int i = 0;
        while(*ptr != '\n') {
            uint64_t start, len;
            sscanf(ptr, "%lld %lld%n", &start, &len, &offset);
            seeds[i].Start = start;
            seeds[i].End = start + len - 1;
            seeds[i].Born = 0;
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
        SeedSet& seed = seeds[i];
        for(int j = seed.Born; j < setCount; ++j)
        {
            for(int k = 0; k < sets[j].Count; ++k)
            {
                Map& map = sets[j].Map[k];
                if(seed.Start < map.Src && seed.End >= map.Src)
                {
                    SeedSet newSeed =
                    {
                        .Start = map.Src,
                        .End = seed.End,
                        .Born = j
                    };
                    seeds[seedCount] = newSeed;
                    ++seedCount;
                    seed.End = map.Src - 1;
                }

                if(seed.Start < map.Src+map.Range && seed.End >= map.Src+map.Range)
                {
                    SeedSet newSeed =
                    {
                        .Start = map.Src + map.Range,
                        .End = seed.End,
                        .Born = j
                    };
                    seeds[seedCount] = newSeed;
                    ++seedCount;
                    seed.End = (map.Src + map.Range) - 1;
                }

                if(seed.Start >= map.Src && seed.Start < (map.Src + map.Range))
                {
                    seed.Start = seed.Start - map.Src + map.Dest;
                    seed.End = seed.End - map.Src + map.Dest;
                    break;
                }
            }
        }
        minLoc = seed.Start < minLoc ? seed.Start : minLoc;
    }

    // I leave this here in shame, I forgot it was here and wondered why my code was taking so long to complete...
    // std::vector<uint64_t> vec;
    // for(int i = 0; i < seedCount; ++i) {
    //     for(int j = 0; j <= seeds[i].End; ++j)
    //     {
    //         vec.push_back(seeds[i].Start + j);
    //     }
    // }
    // std::sort(std::begin(vec), std::end(vec));
    // for(int i = 0; i < vec.size(); ++i) {
    //     printf("%lld ", vec[i]);
    // }
    // printf("\n");

    return minLoc;
}
