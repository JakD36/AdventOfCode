#include <vector>
#include <algorithm>
#include <chrono>

void Part1(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Part1(file);
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
    int down = 1, right = 3;

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

    // Find the number of trees on the path defined by down, right
    int treeCount = 0;
    int x = 0,y = 0;
    while(y < map.size())
    {
        treeCount += ( map[y] & (1 << (x % 31)) ) > 0;
        x += right; y += down;
    }
    printf("Tree Count %d\n",treeCount);
}
