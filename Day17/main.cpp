#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <numeric>

void Part1(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    FILE* file = fopen("testInput.txt","r");

    if(file)
    {
        Part1(file);
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
    unsigned long read[64][64], write[64][64];
    std::fill((unsigned long*)read, (unsigned long*)read + 64 * 64, 0lu);
    std::fill((unsigned long*)write, (unsigned long*)write + 64 * 64, 0lu);
    for(int i = 0; i < 64; ++i)
    {
        for(int j = 0; j < 64; ++j)
        {
            read[i][j] = 0;
            write[i][j] = 0;
        }
    }

    char buf[255];
    int z = 0;
    while(fgets(buf,255,file) != nullptr)
    {
        int i = 0;
        while(buf[i] != '\0' && buf[i] != '\n')
        {
            if(buf[i] == '#')
            {
                read[32][28 + i] |= (1lu << (28lu + z));
                write[32][28 + i] |= (1lu << (28lu + z));
            }
            ++i;
        }
        ++z;
    }

    int offset = 1;
    for(int rounds = 0; rounds < 6; ++rounds)
    {
        for(int y = offset; y < 64-offset; ++y)
        {
            for(int x = offset; x < 64-offset; ++x)
            {
                for(unsigned long z2 = offset; z2 < 64 - offset; ++z2)
                {

                    int sum = 0;
                    for(int i = y-1; i <= y+1; ++i)
                    {
                        for(int j = x-1; j <= x+1; ++j)
                        {
                            for(unsigned int k = z2 - 1; k <= z2 + 1; ++k)
                            {
                                sum += (read[i][j] & (1lu << k)) > 0lu;
                            }
                        }
                    }

                    bool active = (read[y][x] & (1lu << z2)) > 0lu;
                    sum -= active;

                    if(active && !(sum == 2 || sum == 3))
                    {
                        write[y][x] &= ~(1lu << z2);
                    }
                    else if(!active && sum == 3)
                    {
                        write[y][x] |= (1u << z2);
                    }

                }
            }
        }

        memcpy((unsigned long*)read,(unsigned long*)write,64*64 * sizeof(unsigned long));
    }

    int total = 0;
    for(int y = 0; y < 64; ++y)
    {
        for (int x = 0; x < 64; ++x)
        {
            for (unsigned long z = 0; z < 64; ++z)
            {
                total += (read[y][x] & (1lu << z)) > 0;
            }
        }
    }
    printf("%d\n",total);
}