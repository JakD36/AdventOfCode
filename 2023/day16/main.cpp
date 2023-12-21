#include <assert.h>
#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <unordered_set>

int Part1(const char* filepath);
int Part2(const char* filepath);


enum class Dir : uint8_t
{
    UP,
    LEFT,
    DOWN,
    RIGHT
};

struct Ray
{
    int Idx;
    Dir Dir;
};

inline size_t Hash(Ray ray)
{
    size_t h1 = std::hash<int>{}(ray.Idx);
    size_t h2 = std::hash<int>{}((int)ray.Dir);
    return h1 ^ (h2 << 1);
}
int SolveForRay(const char* board, int width, int height, Ray ray);

const int k_size = 120;

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

bool TryMoveRay(Ray& ray, int width, int height);

void PrintBoard(const char* board, int width, int height);

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    char contents[k_size * k_size + 2];

    int width = k_size;
    int rowIdx = 0;
    char* ptr = contents;
    while(fgets(ptr, k_size + 1, file) != nullptr)
    {
        if(rowIdx == 0)
            width = strlen(contents) - 1;
        ptr += width;
        ++rowIdx;
    }
    fclose(file);

    return SolveForRay(contents, width, rowIdx, {0, Dir::RIGHT});
}

int Part2(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    char contents[k_size * k_size + 2];

    int width = k_size;
    int height = 0;
    char* ptr = contents;
    while(fgets(ptr, k_size + 1, file) != nullptr)
    {
        if(height == 0)
            width = strlen(contents) - 1;
        ptr += width;
        ++height;
    }
    fclose(file);

    int max = -1;
    for(int y = 0; y < height; ++y)
    {
        int tmp = SolveForRay(contents, width, height, {y * width, Dir::RIGHT});
        max = tmp > max ? tmp : max;
        tmp = SolveForRay(contents, width, height, {(y+1) * width - 1, Dir::LEFT});
        max = tmp > max ? tmp : max;
    }
    for(int x = 0; x < width; ++x)
    {
        int tmp = SolveForRay(contents, width, height, {x, Dir::DOWN});
        max = tmp > max ? tmp : max;
        tmp = SolveForRay(contents, width, height, {height * width - 1 - x, Dir::UP});
        max = tmp > max ? tmp : max;
    }
    return max;
}

void PrintBoard(const char* board, int width, int height)
{
    for(int y = 0; y < height; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            printf("%c", board[y * width + x]);
        }
        printf("\n");
    }
}

inline bool TryMoveRay(Ray& ray, int width, int height)
{
    switch (ray.Dir)
    {
        case Dir::UP:
            if(ray.Idx / width != 0)
            {
                ray.Idx -= width;
            }
            else
            {
                return false;
            }
        break;
        case Dir::LEFT:
            if(ray.Idx % width != 0)
            {
                ray.Idx -= 1;
            }
            else
            {
                return false;
            }
        break;
        case Dir::DOWN:
            if(ray.Idx / width != height - 1)
            {
                ray.Idx += width;
            }
            else
            {
                return false;
            }
        break;
        case Dir::RIGHT:
            if((ray.Idx + 1) % width != 0)
            {
                ray.Idx += 1;
            }
            else
            {
                return false;
            }
        break;
    }
    return true;
}

int SolveForRay(const char* board, int width, int height, Ray ray)
{
    // Use ringbuffer as we will be adding, removing
    int rayStart = 0;
    int rayCount = 1;
    const int rayCapacity = 500;
    Ray rays[rayCapacity] = {ray};
    std::unordered_set<size_t> visited;

    std::bitset<k_size*k_size> energised;
    while(rayCount > 0)
    {
        Ray ray = rays[rayStart];
        rayStart = (rayStart + 1) % rayCapacity;
        --rayCount;
        bool endOfTravel = false;
        while(endOfTravel == false)
        {
            if(visited.contains(Hash(ray)))
            {
                break;
            }

            energised.set(ray.Idx, true);

            switch (board[ray.Idx])
            {
                case '/':
                    visited.insert(Hash(ray));
                    switch (ray.Dir)
                    {
                        case Dir::UP:
                            ray.Dir = Dir::RIGHT;
                        break;
                        case Dir::LEFT:
                            ray.Dir = Dir::DOWN;
                        break;
                        case Dir::DOWN:
                            ray.Dir = Dir::LEFT;
                        break;
                        case Dir::RIGHT:
                            ray.Dir = Dir::UP;
                        break;
                    }
                break;
                case '\\':
                    visited.insert(Hash(ray));
                    switch (ray.Dir)
                    {
                        case Dir::UP:
                            ray.Dir = Dir::LEFT;
                        break;
                        case Dir::LEFT:
                            ray.Dir = Dir::UP;
                        break;
                        case Dir::DOWN:
                            ray.Dir = Dir::RIGHT;
                        break;
                        case Dir::RIGHT:
                            ray.Dir = Dir::DOWN;
                        break;
                    }
                break;
                case '|':
                    visited.insert(Hash(ray));
                    switch (ray.Dir)
                    {
                        case Dir::LEFT:
                            ray.Dir = Dir::DOWN;
                            if(ray.Idx / width != 0)
                                rays[(rayStart + rayCount++) % rayCapacity] = {ray.Idx - width, Dir::UP};
                            break;
                        case Dir::RIGHT:
                            ray.Dir = Dir::UP;
                            if(ray.Idx / width != height - 1)
                                rays[(rayStart + rayCount++) % rayCapacity] = {ray.Idx + width, Dir::DOWN};
                            break;
                    }
                break;
                case '-':
                    visited.insert(Hash(ray));
                    switch (ray.Dir)
                    {
                        case Dir::UP:
                            ray.Dir = Dir::LEFT;
                            if(ray.Idx + 1 % width != 0)
                                rays[(rayStart + rayCount++) % rayCapacity] = {ray.Idx + 1, Dir::RIGHT};
                            break;
                        case Dir::DOWN:
                            ray.Dir = Dir::RIGHT;
                            if(ray.Idx % width != 0)
                                rays[(rayStart + rayCount++) % rayCapacity] = {ray.Idx - 1, Dir::LEFT};
                            break;
                    }
                break;
            }

            // Try Move the ray
            endOfTravel = !TryMoveRay(ray, width, height);
        }
    }
    return energised.count();
}