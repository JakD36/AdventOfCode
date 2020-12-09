#include <chrono>
#include <cstdio>
#include <vector>

void Part1(FILE* file);
void Part2(FILE* file);
bool CheckValid(int preamble[], int preambleLen, int val);

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
    const int preambleLen = 25;

    int preamble[preambleLen]; // ringbuffer
    int val, count = 0, index = -1;
    while(fscanf(file,"%d",&val) != EOF)
    {
        if(count >= preambleLen)
        {
            if(CheckValid(preamble, preambleLen, val) == false)
            {
                printf("Invalid %d\n", val);
            }
        }
        index = (index + 1) % preambleLen;
        preamble[index] = val;
        ++count;
    }
}

bool CheckValid(int preamble[], int preambleLen, int val)
{
    for(int i = 0; i < preambleLen; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            if(preamble[i] + preamble[j] == val)
            {
                return true;
            }
        }
        for(int j = i + 1; j < preambleLen; ++j)
        {
            if(preamble[i] + preamble[j] == val)
            {
                return true;
            }
        }
    }
    return false;
}

void Part2(FILE* file)
{
    const int invalidNumber = 393911906;

    // Cache all values
    std::vector<int> values;
    values.reserve(500);
    int buf;
    while(fscanf(file,"%d",&buf) != EOF)
    {
        values.push_back(buf);
    }

    int count = values.size();
    int start = 0, end = 1;
    int sum = values[start] + values[end];
    while(end != count && start != count-1)
    {
        if(sum < invalidNumber)
        {
            ++end;
            sum += values[end];
        }
        else if (sum > invalidNumber)
        {
            sum -= values[start];
            ++start;
        }
        else
        {
            int min = INT32_MAX, max = INT32_MIN;
            for(int i = start; i < end + 1; ++i)
            {
                min = values[i] < min ? values[i] : min;
                max = values[i] > max ? values[i] : max;
            }
            printf("Result = %d\n",min+max);
            break;
        }
//        printf("%3d %3d\t %6d == %6d\n",start,end,sum,invalidNumber);
    }

}