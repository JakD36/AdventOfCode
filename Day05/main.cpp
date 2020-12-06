#include <vector>
#include <algorithm>
#include <chrono>

void Part1(FILE* file);
void Part2(FILE* file);
void Part2Smart(FILE* file);
void Visualise(bool seats[1024], int min, int max);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Part2Smart(file);
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

int GetSeatBinary(char *seat)
{
    unsigned int seatId = 0;
    for(int i = 0; i < 7; ++i)
    {
        seatId |= (seat[i] == 'B') << (9 - i) ;
    }
    for(int i = 7; i < 10; ++i)
    {
        seatId |= (seat[i] == 'R') << (9 - i);
    }
    return seatId;
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
    const int maxSeatId = 987;
    // Always make sure to prefill the array! Otherwise you get old data muddling your info
    // I didn't in my attempt, which meant I saw odd seats where it looked like there were assigned
    // boarding passes, when in reality the first row hadn't been encountered yet.
    bool seats[987] = {false};
    char tmp[12];

    while(fgets(tmp,12,file) != nullptr)
    {
        seats[GetSeatId(tmp)] = true;
    }
    for(int i = 1; i < maxSeatId; ++i)
    {
        if(seats[i-1] && !seats[i] && seats[i+1])
        {
            printf("%d\n",i);
            break;
        }
    }
}

void Part2Smart(FILE* file) // Wanted to test + compare andrews solution, significantly faster
{
    const int maxSeatId = 987;
    char tmp[12];
    int min = INT32_MAX, max = maxSeatId, sum = 0;
    while(fgets(tmp,12,file) != nullptr)
    {
        int id = GetSeatBinary(tmp);
        min = std::min(min,id);
        sum -= id;
    }
    for(int i = min; i < max + 1; ++i){sum += i;}
    printf("%d\n",sum);
}

void Visualise(bool seats[1024], int min, int max)
{
    for(int i = 0; i < 128; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            int id = i * 8 + j;
            if(id >= min && id <= max)
            {
                printf("\033[;32m%d \033[0m",seats[i * 8 + j]);
            }
            else
            {
                printf("%d ",seats[i * 8 + j]);
            }

        }
        printf("\n");
    }
}