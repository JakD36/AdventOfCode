#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <numeric>

void Part1(FILE* file);
void Part2(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    FILE* file = fopen("input.txt","r");

    if(file)
    {
        Part2(file);
    }
    else
    {
        printf("Failed to open files\n");
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    fclose(file);
    return 0;
}

void Part1(FILE* file)
{
    const int width = 32;
    const int halfWidth = width / 2;

    bool read[width][width][width], write[width][width][width];

    std::fill((bool*)read, (bool*)read + width * width * width, false);
    std::fill((bool*)write, (bool*)write + width * width * width, false);

    char buf[255];
    int j = 0;
    while(fgets(buf,255,file) != nullptr)
    {
        int i = 0;
        while(buf[i] != '\0' && buf[i] != '\n')
        {
            if(buf[i] == '#')
            {
                read[halfWidth][halfWidth + i][halfWidth+j] = true;
                write[halfWidth][halfWidth + i][halfWidth+j] = true;
            }
            ++i;
        }
        ++j;
    }

    int offset = 1;
    for(int rounds = 0; rounds < 6; ++rounds)
    {
        for(int y = offset; y < width-offset; ++y)
        {
            for(int x = offset; x < width-offset; ++x)
            {
                for(int z = offset; z < width - offset; ++z)
                {

                    int sum = 0;
                    for(int i = y-1; i <= y+1; ++i)
                    {
                        for(int j = x-1; j <= x+1; ++j)
                        {
                            for(unsigned int k = z - 1; k <= z + 1; ++k)
                            {
                                sum += read[i][j][k];
                            }
                        }
                    }

                    bool active = read[y][x][z];
                    sum -= active;

                    if(active && !(sum == 2 || sum == 3))
                    {
                        write[y][x][z] = false;
                    }
                    else if(!active && sum == 3)
                    {
                        write[y][x][z] = true;
                    }

                }
            }
        }

        memcpy((bool*)read,(bool*)write,width * width * width * sizeof(bool));
    }

    int total = 0;
    for(int y = 0; y < width; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            for (unsigned long z = 0; z < width; ++z)
            {
                total += read[y][x][z];
            }
        }
    }
    printf("%d\n",total);
}

void Part2(FILE* file)
{
    const int width = 32;
    const int halfWidth = width / 2;

    bool read[width][width][width][width], write[width][width][width][width];

    std::fill((bool*)read, (bool*)read + width * width * width * width, false);
    std::fill((bool*)write, (bool*)write + width * width * width * width, false);

    char buf[255];
    int j = 0;
    while(fgets(buf,255,file) != nullptr)
    {
        int i = 0;
        while(buf[i] != '\0' && buf[i] != '\n')
        {
            if(buf[i] == '#')
            {
                read[halfWidth][halfWidth][halfWidth + i][halfWidth+j] = true;
                write[halfWidth][halfWidth][halfWidth + i][halfWidth+j] = true;
            }
            ++i;
        }
        ++j;
    }

    int offset = 1;
    for(int rounds = 0; rounds < 6; ++rounds)
    {
        for(int y = offset; y < width-offset; ++y)
        {
            for(int x = offset; x < width-offset; ++x)
            {
                for(int z = offset; z < width - offset; ++z)
                {
                    for(int w = offset; w < width - offset; ++w)
                    {

                        int sum = 0;
                        for(int i = y-1; i <= y+1; ++i)
                        {
                            for(int j = x-1; j <= x+1; ++j)
                            {
                                for(int k = z - 1; k <= z + 1; ++k)
                                {
                                    for(int l = w - 1; l <= w + 1; ++l)
                                    {
                                        sum += read[i][j][k][l];
                                    }
                                }
                            }
                        }

                        bool active = read[y][x][z][w];
                        sum -= active;

                        if(active && !(sum == 2 || sum == 3))
                        {
                            write[y][x][z][w] = false;
                        }
                        else if(!active && sum == 3)
                        {
                            write[y][x][z][w] = true;
                        }

                    }
                }
            }
        }

        memcpy((bool*)read,(bool*)write,width * width * width * width * sizeof(bool));
    }

    int total = 0;
    for(int y = 0; y < width; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            for (int z = 0; z < width; ++z)
            {
                for (int w = 0; w < width; ++w)
                {
                    total += read[y][x][z][w];
                }
            }
        }
    }
    printf("%d\n",total);
}