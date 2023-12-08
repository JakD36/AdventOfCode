#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "Header.h"

int main()
{
    // char filepath[] = "testInput.txt";
    char filepath[] = "input.txt";

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t total = Part1(filepath);
        printf("Part 1 %lld\n", total);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }

    {
        auto start = std::chrono::steady_clock::now();
        uint64_t total = Part2(filepath);
        printf("Part 2 %lld\n", total);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
    }

    return 0;
}
