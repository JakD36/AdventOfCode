#include <chrono>
#include <cstdio>
#include <vector>
#include <algorithm>

void Part1(FILE* file);
void Part2(FILE* file);

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
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Part1(FILE* file)
{
    int time;
    char buffer[255];
    fscanf(file,"%d\n",&time);
    fgets(buffer,255,file);

    // Get all ids
    std::vector<int> ids;
    int offset = 0, result;
    do
    {
        int id;
        int bytesRead;
        result = sscanf(&buffer[offset], "%d,%n",&id,&bytesRead);
        if(result == 0)
            result = sscanf(&buffer[offset], "%c,%n",&id,&bytesRead);
        else
            ids.push_back(id);
        offset += bytesRead;
    } while(result != EOF);


    int min = INT32_MAX;
    int id = -1;
    for(auto& x : ids)
    {
        int remain = x - time % x;
        if(remain < min)
        {
            id = x;
            min = remain;
        }
    }

    printf("id = %d, wait = %d\n",id,min);
    printf("Answer = %d\n",id*min);
}


void Part2(FILE* file)
{
    char buffer[255];
    fscanf(file,"%*d\n");
    fgets(buffer,255,file);

    // Get all ids
    std::vector<std::tuple<int,int>> ids;
    int i = 0;
    int offset = 0, result;
    do
    {
        int id;
        int bytesRead;
        result = sscanf(&buffer[offset], "%d,%n",&id,&bytesRead);
        if(result == 0)
            result = sscanf(&buffer[offset], "%c,%n",&id,&bytesRead);
        else
            ids.push_back({id,i});
        offset += bytesRead;
        ++i;
    } while(result != EOF);

    int min = INT32_MAX;
    int id = -1;
    for(auto& x : ids)
    {
        int remain = x - time % x;
        if(remain < min)
        {
            id = x;
            min = remain;
        }
    }

    printf("id = %d, wait = %d\n",id,min);
    printf("Answer = %d\n",id*min);
}
