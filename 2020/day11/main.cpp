#include <chrono>
#include <cstdio>
#include <vector>
#include <algorithm>

void Core(FILE* file);

const int cols = 91;
const int rows = 98;
//const int cols = 10;
//const int rows = 10;

const char floor = -1;
const char empty = 0;
const char taken = 1;

bool SimulatePart1(char read[rows+2][cols+2], char write[rows+2][cols+2]);
bool SimulatePart2(char read[rows+2][cols+2], char write[rows+2][cols+2]);
int Count(char read[rows+2][cols+2]);
void PrintBuffer(char buf[rows+2][cols+2]);
bool CheckDirection(char read[rows+2][cols+2], int i, int j, char x, char y);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Core(file);
    }
    else
    {
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Core(FILE* file)
{
    char buf[2][rows+2][cols+2];
    int halfBufferSize = (rows + 2) * (cols + 2);
    std::fill((char*)buf, (char*)buf + halfBufferSize * 2, -1);

    // Initialise
    char readBuffer[cols];
    int row = 1;
    while(fscanf(file,"%s",readBuffer) != EOF)
    {
        for(int i = 0; i < cols; ++i)
        {
            switch (readBuffer[i])
            {
                case 'L':
                    buf[0][row][i+1] = empty;
                    buf[1][row][i+1] = empty;
                    break;
                case '.':
                    buf[0][row][i+1] = floor;
                    buf[1][row][i+1] = floor;
                    break;
            }
        }
        ++row;
    }


    bool change = true;
    char read = 0, write = 1;
    while(change)
    {
        PrintBuffer(buf[read]);
        change = SimulatePart2(buf[read],buf[write]);
        memcpy((char*)buf[read], (char*)buf[write], halfBufferSize);
    }
    printf("%d\n",Count(buf[read]));
}

bool SimulatePart1(char read[rows+2][cols+2], char write[rows+2][cols+2])
{
    bool change = false;
    for(int i = 1; i < rows+1; ++i)
    {
        for(int j = 1; j < cols+1; ++j)
        {
            if(read[i][j] == floor)
                continue;

            int sum = (
                    (read[i-1][j-1] > 0) +
                    (read[i-1][j] > 0) +
                    (read[i-1][j+1] > 0) +

                    (read[i][j-1] > 0) +
                    (read[i][j+1] > 0) +

                    (read[i+1][j-1] > 0) +
                    (read[i+1][j] > 0) +
                    (read[i+1][j+1] > 0)
                    );

            if(sum == 0 && read[i][j] == empty)
            {
                write[i][j] = taken;
                change = true;
            }
            else if (sum >= 4 && read[i][j] == taken)
            {
                write[i][j] = empty;
                change = true;
            }
        }
    }
    return change;
}

bool SimulatePart2(char read[rows+2][cols+2], char write[rows+2][cols+2])
{
    bool change = false;
    for(int i = 1; i < rows+1; ++i)
    {
        for(int j = 1; j < cols+1; ++j)
        {
            if(read[i][j] == floor)
                continue;

            int sum =
                    CheckDirection(read,i,j,-1,-1) +
                    CheckDirection(read,i,j,-1,0) +
                    CheckDirection(read,i,j,-1,1) +

                    CheckDirection(read,i,j,0,-1) +
                    CheckDirection(read,i,j,0,1) +

                    CheckDirection(read,i,j,1,-1) +
                    CheckDirection(read,i,j,1,0) +
                    CheckDirection(read,i,j,1,1);

            if(sum == 0 && read[i][j] == empty)
            {
                write[i][j] = taken;
                change = true;
            }
            else if (sum >= 5 && read[i][j] == taken)
            {
                write[i][j] = empty;
                change = true;
            }
        }
    }
    return change;
}

bool CheckDirection(char read[rows+2][cols+2], int i, int j, char x, char y)
{
    i+=x; j+=y;
    for(;
    i < rows + 1 && i > 0
            && j < cols + 1 && j > 0;
    i+=x, j+=y)
    {
        switch (read[i][j])
        {
            case empty:
                return false;
            case taken:
                return true;
            case floor:
                continue;
        }
    }
    return false;
}

int Count(char read[rows+2][cols+2])
{
    int count = 0;
    for(int i = 1; i < rows+1; ++i)
    {
        for(int j = 1; j < cols+1; ++j)
        {
            if(read[i][j] == taken)
                ++count;
        }
    }
    return count;
}

void PrintBuffer(char buf[rows+2][cols+2])
{
    for(int i = 0; i < rows+2; ++i)
    {
        for(int j = 0; j < cols+2; ++j)
        {
            printf("%2d ",buf[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}