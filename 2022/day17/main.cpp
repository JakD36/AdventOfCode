#include <chrono>
#include <cstdio>
#include <cstring>

int Part1(const char* filepath);

void Print(uint32_t mask);
void Print(uint32_t tetromino, uint32_t chunk);
void PrintRows(uint8_t* rows, int count);

int main()
{
    // char filepath[] = "testInput.txt";
    // char filepath[] = "testInput2.txt";
    char filepath[] = "input.txt";

    {
        auto start = std::chrono::steady_clock::now();
        int sum = Part1(filepath);
        printf("Part 1 %i\n", sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("%lld microseconds\n", duration);
    }

    return 0;
}

int Part1(const char* filepath)
{
    uint32_t tetrominos[] =
    {
        0b1111, // flat
        2 | 7 << 8 | 2 << 16, // cross
        7 | 1 << 10 | 1 << 18, // L
        0b1000000010000000100000001, // tall
        0b1100000011 // square
    };

    // Debug tetriminos
    // for(int i = 0; i < 5; ++i) {
        // Print(tetrominos[i]);
        // printf("\n");
    // }
    // return 0;

    int sizes[] =
    {
        1,
        3,
        3,
        4,
        2
    };

    FILE* file;
    file = fopen(filepath,"r");
    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return -1;
    }

    const int size = 1000;
    int bitarray[size];
    for(int i = 0; i < size; ++i)
        bitarray[i] = 0;

    int instructionCount = 0;
    bool end = false;
    do
    {
        char c = fgetc(file);
        // printf("%c",c);
        switch (c)
        {
            case (int)'<':
                ++instructionCount;
                break;
            case (int)'>':
                bitarray[instructionCount / 32] |= 1 << instructionCount % 32;
                ++instructionCount;
                break;
            case (int)'\n':
                end = true;
                break;
            case EOF:
                printf("Error in getting character");
                return -1;
                break;
        }
    } while (end == false);

    // for(int i = 0; i < instructionCount; ++i) {
        // printf("%c", (bitarray[i / 32] & 1 << i % 32) != 0 ? '>' : '<');
    // }
    // printf("\n");


    const uint32_t rowCount = 10'000;
    uint8_t rows[rowCount];
    for(int i = 0; i < rowCount; ++i)
        rows[i] = 1 << 7;

    int maxHeight = 0;
    const int startPos = 2;
    int jetIdx = 0;
    for(int i = 0; i < 2022; ++i) // Each rock is falling
    {
        int idx = i % 5; // select tetromino
        uint32_t templateTetrimino = tetrominos[idx];
        int tetroSize = sizes[idx];

        // old jet pos
        int x = startPos;
        uint32_t tetrimino = 0;
        for(int j = 0; j < 4; ++j)
        {
            uint32_t row = (templateTetrimino & 255 << 8 * j);
            tetrimino |= row << startPos;
        }

        // return -1;
        int y = maxHeight + 3;
        if(y + 3 > rowCount)
        {
            printf("Gone over the max row count!");
            return -1;
        }
        while(true)
        {
            // apply jet
            // if 0 = <, 1 = >
            bool moveRight = (bitarray[jetIdx / 32] & 1 << jetIdx % 32) != 0; // * 2 -1 to get it to -1 or +1
            ++jetIdx;
            jetIdx %= instructionCount;

            if(moveRight || x - 1 >= 0)
            {
                uint32_t movedTetro = 0;
                if(moveRight)
                {
                    for(int j = 0; j < 4; ++j)
                        movedTetro |= (tetrimino & (255 << (8 * j))) << 1;
                }
                else
                {
                    for(int j = 0; j < 4; ++j)
                        movedTetro |= (tetrimino & (255 << (8 * j))) >> 1;
                }

                uint32_t mask = *(uint32_t*)(&rows[y]);
                if((movedTetro & mask) == 0)
                {
                    tetrimino = movedTetro;
                    x += moveRight ? 1 : -1;
                }
            }

            // drop // potentially I can reuse the previous mask by shifting by 4 to push everything up 1 and just recalc the new layer
            // if(y > 0)
            // {
            //     Print(tetrimino, *(uint32_t*)&rows[y - 1]);
            // }

            if(y > 0 && (tetrimino & *(uint32_t*)&rows[y - 1]) == 0)
            {
                --y;
            }
            else
            {
                uint32_t& writeRows = *(uint32_t*)(&rows[y]);
                writeRows |= tetrimino;

                maxHeight = maxHeight < y + tetroSize ? y + tetroSize : maxHeight;
                break;
            }
        }
    }
    // PrintRows(rows, 400);
    return maxHeight;
}

void Print(uint32_t mask)
{
    printf("--------\n");
    for(int i = 3; i >= 0; --i)
    {
        uint8_t row = *((uint8_t*)&mask + i);
        for(int j = 0; j < 8; ++j)
        {
            if((row & 1 << j) != 0)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("--------\n");
}

void Print(uint32_t tetromino, uint32_t chunk)
{
    printf("--------\n");
    for(int i = 3; i >= 0; --i)
    {
        uint8_t tetroRow = *((uint8_t*)&tetromino + i);
        uint8_t boardRow = *((uint8_t*)&chunk + i);
        for(int j = 0; j < 8; ++j)
        {
            if((tetroRow & 1 << j) != 0 && (boardRow & 1 << j) != 0)
            {
                printf("x");
            }
            else if((tetroRow & 1 << j) != 0)
            {
                printf("0");
            }
            else if((boardRow & 1 << j) != 0)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("--------\n");
}


void PrintRows(uint8_t* rows, int count)
{
    printf("--------\n");
    for(int i = count; i >= 0; --i)
    {
        uint8_t& row = rows[i];
        for(int j = 0; j < 8; ++j)
        {
            if((row & 1 << j) != 0)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("--------\n");
}

