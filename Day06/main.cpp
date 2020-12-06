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
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Part1(FILE* file)
{
    unsigned int answers = 0, count = 0;
    char buffer[28];
    while(fgets(buffer,28,file) != nullptr)
    {
        if(buffer[0] != '\n')
        {
            int i = 0;
            while(buffer[i] != '\0' && buffer[i] != '\n')
            {
                answers |= 1U << (buffer[i] - 'a');
                ++i;
            }
        }
        else
        {
            int groupCount = 0;
            for(unsigned int i = 0; i < 26; ++i)
            {
                groupCount += (answers & (1U << i)) > 0;
            }
            printf("Group Count %d\n",groupCount);
            count += groupCount;
            answers = 0;
        }
    }

    int groupCount = 0;
    for(unsigned int i = 0; i < 26; ++i)
    {
        groupCount += (answers & (1U << i)) > 0;
    }
    printf("Group Count %d\n",groupCount);
    count += groupCount;
    printf("%d\n",count);
}
