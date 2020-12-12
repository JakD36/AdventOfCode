#include <chrono>
#include <cstdio>
#include <vector>
#include <algorithm>

void Part1(FILE* file);
void Part2(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "testInput2.txt";
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
    int freq[4] = {0,0,0,0};
    std::vector<int> adapters = {0};
    int val;

    while(fscanf(file,"%d",&val) != EOF)
    {
        adapters.push_back(val);
    }
    std::sort(begin(adapters),end(adapters));

    for(int i = 1, count = adapters.size(); i < count; ++i)
    {
        ++freq[adapters[i] - adapters[i-1]];
    }
    ++freq[3];

    printf("0=%d 1=%d 2=%d 3=%d\tTotal=%d\n",freq[0],freq[1],freq[2],freq[3],freq[1]*freq[3]);
}

void Part2(FILE* file)
{
    std::vector<int> adapters = {0};
    int val;

    while(fscanf(file,"%d",&val) != EOF)
    {
        adapters.push_back(val);
    }

    std::sort(begin(adapters),end(adapters));

    int count = adapters.size();
    adapters.push_back(adapters[count++-1]+3);

    int* freq = new int[count];

    for(int i = 0; i < count; ++i)
    {
        for(int j = i+1; j < count; ++j)
        {
            if(adapters[j]-adapters[i] <= 3)
            {
                ++freq[i];
            }
            else
            {
                break;
            }
        }
        printf("%d\n",freq[i]);
    }

    int branches = 1;
//
//    for(int i = 0; i < count;)
//    {
//        if(i == 1){++i;}
//        if(i == 2)
//        {
//
//        }
//    }

    printf("Branches %d\n",branches);


}
