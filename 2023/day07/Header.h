//
// Created by Jack Davidson on 08/12/2023.
//

#ifndef HEADER_H
#define HEADER_H
#include <cstdint>

uint64_t Part1(const char* filepath);
uint64_t Part2(const char* filepath);

enum class Type {
    HIGH_CARD = 0,
    ONE_PAIR = 1,
    TWO_PAIR = 2,
    THREE_OF_A_KIND = 3,
    FULL_HOUSE = 4,
    FOUR_OF_A_KIND = 5,
    FIVE_OF_A_KIND = 6,
};

struct Hand
{
    char Cards[5];
    Type Type;
    int Bid;
};

inline void Count(int* indices, int* count)
{
    for(int i = 0; i < 5; ++i)
    {
        ++count[indices[i]];
    }
}

#endif //HEADER_H
