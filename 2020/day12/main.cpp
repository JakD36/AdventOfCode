#include <chrono>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <valarray>

void Part1(FILE* file);
void Part2(FILE* file);
std::valarray<int> Rotate(std::valarray<int> pos, int degrees);

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
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Part1(FILE* file)
{
    int dir = 1;
    int pos[4] = {0,0,0,0};
    // N, E, S, W

    char op;
    int val;
    while(fscanf(file,"%c%d\n",&op,&val) != EOF)
    {
        switch (op) {
            case 'F':
                pos[dir] += val;
                break;
            case 'L':

                dir = (dir + 4 - (val / 90)) % 4;
                break;
            case 'R':
                dir = (dir + (val / 90)) % 4;
                break;
            case 'N':
                pos[1] += val;
                break;
            case 'E':
                pos[0] += val;
                break;
            case 'S':
                pos[3] += val;
                break;
            case 'W':
                pos[2] += val;
                break;
        }
    }
    printf("North %d East %d\n",pos[0]-pos[2],pos[1]-pos[3]);
    int manhattenDistance = abs(pos[0] - pos[2]) + abs(pos[1]-pos[3]);
    printf("%d\n",manhattenDistance);
}

std::valarray<int> Rotate(std::valarray<int> pos, int degrees)
{
    float radians = (float)degrees * (M_PI / 180.f);
    float c = cos(radians);
    float s = sin(radians);
    float x = pos[0] * c - pos[1] * s;
    float y = pos[0] * s + pos[1] * c;
    return {(int)round(x), (int)round(y)};
}

void Part2(FILE* file)
{
    std::valarray<int> pos = {0,0};
    std::valarray<int> way = {10,1};

    char op;
    int val;
    while(fscanf(file,"%c%d\n",&op,&val) != EOF)
    {
        switch (op) {
            case 'F':
                pos += way * val;
                break;
            case 'L':
                way = Rotate(way,val);
                break;
            case 'R':
                val *= -1;
                way = Rotate(way,val);
                break;
            case 'N':
                way[1] += val;
                break;
            case 'E':
                way[0] += val;
                break;
            case 'S':
                way[1] -= val;
                break;
            case 'W':
                way[0] -= val;
                break;
        }
    }
    printf("North %d East %d\n",pos[0],pos[1]);
    int manhattenDistance = abs(pos[0]) + abs(pos[1]);
    printf("%d\n",manhattenDistance);
}