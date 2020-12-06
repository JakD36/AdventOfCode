#include <vector>
#include <algorithm>
#include <chrono>

void Part1(FILE* file);
void Part2(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Part2(file);
    }
    else
    {
        printf("Failed to open %s",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

int GetSeatId(char seat[11])
{
    int minRow = 0, maxRow = 127, minCol = 0, maxCol = 7;
    for(int i = 0; i < 7; ++i)
    {
        if(seat[i] == 'F')
        {
            maxRow = minRow + (maxRow - minRow) / 2;
        }
        else if(seat[i] == 'B')
        {
            minRow += (maxRow - minRow) / 2 + 1;
        }
    }
    for(int i = 7; i < 10; ++i)
    {
        if(seat[i] == 'L')
        {
            maxCol = minCol + (maxCol - minCol) / 2;
        }
        else if(seat[i] == 'R')
        {
            minCol += (maxCol - minCol) / 2 + 1;
        }
    }
    return minRow * 8 + minCol;
}

void Part1(FILE* file)
{
    char tmp[12];
    int max = INT32_MIN;
    while(fgets(tmp,12,file) != nullptr)
    {
        int val = GetSeatId(tmp);
        if(val > max)
            max = val;
    }
    printf("%d\n",max);
//    printf("%d\n",GetSeatId("BBFFBBFRLL"));
}

void Part2(FILE* file)
{
    bool seats[988];
    char tmp[12];
    while(fgets(tmp,12,file) != nullptr)
    {
        seats[GetSeatId(tmp)] = true;
    }
    for(int i = 1; i < 997; ++i)
    {
        if(seats[i-1] && !seats[i] && seats[i+1])
        {
            printf("%d\n",i);
            break;
        }
    }
}