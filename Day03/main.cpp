#include <vector>
#include <algorithm>
#include <chrono>

void Part1(FILE* file);
void Part2(FILE* file);
std::vector<unsigned int> CreateMap(FILE* file);
int CountTreesOnSlope(std::vector<unsigned int>& map, int right, int down, int width = 31);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
//        Part1(file);
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

void Part1(FILE* file)
{
    std::vector<unsigned int> map = CreateMap(file);
    int treeCount = CountTreesOnSlope(map, 3, 1);
    printf("Tree Count %d\n",treeCount);
}

void Part2(FILE* file)
{
    std::vector<unsigned int> map = CreateMap(file);
    unsigned long total = CountTreesOnSlope(map, 1, 1);
    total *= CountTreesOnSlope(map, 3, 1);
    total *= CountTreesOnSlope(map, 5, 1);
    total *= CountTreesOnSlope(map, 7, 1);
    total *= CountTreesOnSlope(map, 1, 2);

    printf("Total %lu \n",total);
}

std::vector<unsigned int> CreateMap(FILE* file)
{
    char line[50];
    std::vector<unsigned int> map;
    // Store our tree map
    while(fscanf(file,"%s",line) != EOF)
    {
        unsigned int row = 0;
        int i = 0;
        while(line[i] != '\0')
        {
            if(i > sizeof(unsigned int) * 8)
                assert("We have exceeded the size of an unsigned int!");
            row |= ((line[i] == '#') << i);
            ++i;
        }
        map.push_back(row);
    }
    return map;
}

int CountTreesOnSlope(std::vector<unsigned int>& map, int right, int down, int width)
{
    // Find the number of trees on the path defined by down, right
    int treeCount = 0;
    int x = 0,y = 0;
    while(y < map.size())
    {
        treeCount += ( map[y] & (1 << (x % width)) ) > 0;
        x += right; y += down;
    }
    return treeCount;
}