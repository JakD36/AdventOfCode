#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <unordered_map>

int Part1(const char* filepath);
int Part2(const char* filepath);

void PrintBoard(char* board, int width, int height);

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

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_rows = 100, k_cols = 100;
    char contents[k_rows * k_cols + 2];

    int rowIdx = 0;
    int width = k_cols;
    while(fgets(contents + rowIdx * width, k_cols + 2, file) != nullptr)
    {
        if(rowIdx == 0)
            width = strlen(contents) - 1;
        ++rowIdx;
    }
    fclose(file);

    int sum = 0;
    for(int i = 0; i < rowIdx * width; ++i)
    {
        if(contents[i] == 'O')
        {
            int step = 0;
            while(i - (step + 1) * width >= 0 && contents[i - (step + 1) * width] == '.')
            {
                ++step;
            }
            if(step > 0)
            {
                contents[i] = '.';
                contents[i - step * width] = 'O';
            }

            sum += rowIdx - ((i - step * width) / width);
        }
    }

    return sum;
}

void PrintBoard(char* board, int width, int height)
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

void MoveAllNorth(char* board, int width, int height)
{
    for(int i = 0; i < height * width; ++i)
    {
        if(board[i] == 'O')
        {
            int step = 0;
            while(i - (step + 1) * width >= 0 && board[i - (step + 1) * width] == '.')
            {
                ++step;
            }
            if(step > 0)
            {
                board[i] = '.';
                board[i - step * width] = 'O';
            }
        }
    }
}

void MoveAllSouth(char* board, int width, int height)
{
    for(int i = width * height - 1; i >= 0; --i)
    {
        if(board[i] == 'O')
        {
            int step = 0;
            while(i + (step + 1) * width < width * height && board[i + (step + 1) * width] == '.')
            {
                ++step;
            }
            if(step > 0)
            {
                board[i] = '.';
                board[i + step * width] = 'O';
            }
        }
    }
}

void MoveAllWest(char* board, int width, int height)
{
    for(int i = 0; i < height * width; ++i)
    {
        if(board[i] == 'O')
        {
            int step = 0;

            while((i - (step + 1)) >= (i / width) * width && board[i - (step + 1)] == '.')
            {
                ++step;
            }
            if(step > 0)
            {
                board[i] = '.';
                board[i - step] = 'O';
            }
        }
    }
}

void MoveAllEast(char* board, int width, int height)
{
    for(int i = width * height - 1; i >= 0; --i)
    {
        if(board[i] == 'O')
        {
            int step = 0;
            while((i + (step + 1)) < ((i / width)+1) * width && board[i + (step + 1)] == '.')
            {
                ++step;
            }
            if(step > 0)
            {
                board[i] = '.';
                board[i + step] = 'O';
            }
        }
    }
}

int Part2(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_rows = 100, k_cols = 100;
    char contents[k_rows * k_cols + 2];

    int rowIdx = 0;
    int width = k_cols;
    while(fgets(contents + rowIdx * width, k_cols + 2, file) != nullptr)
    {
        if(rowIdx == 0)
            width = strlen(contents) - 1;
        ++rowIdx;
    }
    fclose(file);

    std::unordered_map<int, int> map;
    const int ringBufferLen = 128;
    int vals[ringBufferLen];
    for(int i = 0; i < 1'000'000'000; ++i)
    {
        MoveAllNorth(contents, width, rowIdx);
        MoveAllWest(contents, width, rowIdx);
        MoveAllSouth(contents, width, rowIdx);
        MoveAllEast(contents, width, rowIdx);

        int hash = 0, sum = 0;
        for(int i = 0; i < width * rowIdx; ++i)
        {
            if(contents[i] == 'O')
            {
                hash += (i / width) * 1000 + i;
                sum += rowIdx - (i / width);
            }
        }
        vals[i % ringBufferLen] = sum;
        // printf("%6d %d\n", i, sum);

        if(map.contains(hash))
        {
            int cycleLen = i - map[hash];
            int mod = (1'000'000'000 - map[hash]) % cycleLen;
            int idx = i - cycleLen + mod - 1;
            return vals[(idx % ringBufferLen + ringBufferLen)%ringBufferLen];
        }
        else
        {
            map[hash] = i;
        }
    }

    int sum = 0;
    for(int i = 0; i < width * rowIdx; ++i)
    {
        if(contents[i] == 'O')
            sum += rowIdx - (i / width);
    }
    return sum;
}