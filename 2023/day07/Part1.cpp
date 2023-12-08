//
// Created by Jack Davidson on 08/12/2023.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Header.h"

void ProcessHand(const char* hand, int* indices)
{
    for(int i = 0; i < 5; ++i)
    {
        int idx = 0;
        switch (hand[i])
        {
            case 'T':
                idx = 8;
            break;
            case 'J':
                idx = 9;
            break;
            case 'Q':
                idx = 10;
            break;
            case 'K':
                idx = 11;
                break;
            case 'A':
                idx = 12;
                break;
            default:
                idx = hand[i] - '2';
            break;
        }
        indices[i] = idx;
    }
}

Type DetermineType(int* counts) {
    int twoPairs = 0;
    int threeOfAKind = 0;
    for(int i = 0; i < 13; ++i) {
        switch (counts[i]) {
            case 5:
                return Type::FIVE_OF_A_KIND;
            case 4:
                return Type::FOUR_OF_A_KIND;
            case 3:
                ++threeOfAKind;
                break;
            case 2:
                ++twoPairs;
                break;
        }
    }
    if(twoPairs == 2)
        return Type::TWO_PAIR;
    if(twoPairs == 1 && threeOfAKind == 1)
        return Type::FULL_HOUSE;
    if(threeOfAKind == 1)
        return Type::THREE_OF_A_KIND;
    if(twoPairs == 1)
        return Type::ONE_PAIR;

    return Type::HIGH_CARD;
}

int CompareHands (const void *a, const void *b)
{
    const Hand* handA = ((const Hand *) a);
    const Hand* handB = ((const Hand *) b);

    if(handA->Type != handB->Type)
        return (int)handA->Type - (int)handB->Type;

    int indicesA[13];
    int indicesB[13];
    memset(indicesA, 0, sizeof(int) * 13);
    memset(indicesB, 0, sizeof(int) * 13);
    ProcessHand(handA->Cards, indicesA);
    ProcessHand(handB->Cards, indicesB);

    for(int i = 0; i < 5; ++i) {
        if(indicesA[i] != indicesB[i])
            return indicesA[i] - indicesB[i];
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

    const int handCapacity = 1000;
    Hand hands[handCapacity];

    const int size = 20;
    char contents[size];
    int handCount = 0;
    int offset = 0;
    while(fgets(contents, size, file) != nullptr)
    {
        // if there are more than 5 cards we will get a buffer overflow, not sure if sscanf_s is portable, for an AOC problem this is fine
        sscanf(contents, "%s %i%n", &hands[handCount].Cards, &hands[handCount].Bid, &offset);

        int indices[5];
        memset(indices, 0, sizeof(int) * 5);
        ProcessHand(hands[handCount].Cards, indices);
        int count[13];
        memset(count, 0, sizeof(int) * 13);
        Count(indices, count);
        hands[handCount].Type = DetermineType(count);

        ++handCount;
    }
    fclose(file);

    qsort(hands, handCount, sizeof(Hand), CompareHands);

    uint64_t total = 0;
    for(int i = 0; i < handCount; ++i) {
        total += (uint64_t)hands[i].Bid * (uint64_t)(i + 1);
        // printf("%s %d   %4d x %4d    %6lld\n", hands[i].Cards, hands[i].Type, hands[i].Bid, i + 1, total);
    }
    // printf("\n");
    return total;
}