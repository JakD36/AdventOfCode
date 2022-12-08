#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <queue>

void Solve();

int main() {
    auto start = std::chrono::steady_clock::now();

    Solve();

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Solve()
{
    std::unordered_map<int,std::queue<int>> map = {
            {11,std::queue<int>({1})},
            {18,std::queue<int>({2})},
            {0,std::queue<int>({3})},
            {20,std::queue<int>({4})},
            {1,std::queue<int>({5})},
            {7,std::queue<int>({6})},
            {16,std::queue<int>({7})}
    };
    int lastNumberSpoken = 16;
    int startIndex = 8;

    // Test Input
//    std::unordered_map<int,std::queue<int>> map = {
//            {0,std::queue<int>({1})},
//            {3,std::queue<int>({2})},
//            {6,std::queue<int>({3})},
//    };
//    int lastNumberSpoken = 6;
//    int startIndex = 4;

    FILE* file = fopen("debug.txt","w");
    for(auto& x : map)
    {
        fprintf(file,"%d\n",x.first);
    }

    int part1Count = 2021;
    int part2Count = 30000001;

    for(int i = startIndex; i < part1Count; ++i)
    {
        if(map.find(lastNumberSpoken) != end(map))
        {
            if(map[lastNumberSpoken].size() == 2)
            {
                lastNumberSpoken = map[lastNumberSpoken].back() - map[lastNumberSpoken].front();
            }
            else
            {
                lastNumberSpoken = 0;
            }

            // add this lastNumberSpoken to its queue
            if(map[lastNumberSpoken].size() == 2)
            {
                map[lastNumberSpoken].pop();
                map[lastNumberSpoken].push(i);
            }
            else
            {
                map[lastNumberSpoken].push(i);
            }
        }
        fprintf(file,"%d\n",lastNumberSpoken);
    }
    fclose(file);
    printf("%d\n",lastNumberSpoken);

}