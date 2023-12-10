#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <unordered_map>

bool AreAllEqual(int* array, int count);
int CalcEndValues(int* array, int* endValues, int count);
int CalcFrontValues(int* array, int* frontValues, int count);

int Part1(const char* filepath);
int Part2(const char* filepath);

int main()
{
    // const char* filepath = "testInput.txt";
    const char* filepath = "input.txt";
    {
        auto start = std::chrono::steady_clock::now();
        int sum = Part1(filepath);
        printf("Part 1 %d\n",sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }
    {
        auto start = std::chrono::steady_clock::now();
        int sum = Part2(filepath);
        printf("Part 2 %d\n",sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
    }

}

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int size = 500;
    char contents[size];

    int sum = 0;
    while(fgets(contents, 500, file) != nullptr)
    {
        int sequence[100];
        int count = 0;
        char* ptr = contents;
        int offset = 0;
        while(sscanf(ptr, "%d%n", &sequence[count], &offset) == 1) {
            // printf("%i ", sequence[count]);
            ptr += offset;
            ++count;
        }
        int finalVals[100];
        int order = CalcEndValues(sequence, finalVals, count);


        for(int i = order - 2; i >= 0; --i) {
            finalVals[i] += finalVals[i+1];
        }

        sum += finalVals[0];
    }
    fclose(file);

    return sum;
}

int Part2(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int size = 500;
    char contents[size];

    int sum = 0;
    while(fgets(contents, 500, file) != nullptr)
    {
        int sequence[100];
        int count = 0;
        char* ptr = contents;
        int offset = 0;
        while(sscanf(ptr, "%d%n", &sequence[count], &offset) == 1) {
            // printf("%i ", sequence[count]);
            ptr += offset;
            ++count;
        }
        int frontVals[100];
        int order = CalcFrontValues(sequence, frontVals, count);


        for(int i = order - 2; i >= 0; --i) {
            frontVals[i] -= frontVals[i+1];
        }

        sum += frontVals[0];
    }
    fclose(file);

    return sum;
}

int CalcEndValues(int* array, int* endValues, int count)
{
    int buffers[2][100];
    int currentIdx = 0;
    memcpy(buffers[0], array, sizeof(int) * count);
    endValues[0] = array[count - 1];
    int order = 0;
    while(AreAllEqual(buffers[currentIdx], count) == false)
    {
        int nextIdx = (currentIdx + 1) % 2;
        for(int i = 0; i < count - 1; ++i)
        {
            buffers[nextIdx][i] = buffers[currentIdx][i+1] - buffers[currentIdx][i];
        }
        --count;
        currentIdx = nextIdx;
        ++order;
        endValues[order] = buffers[nextIdx][count - 1];
    }
    return order + 1;
}

int CalcFrontValues(int* array, int* frontValues, int count)
{
    int buffers[2][100];
    int currentIdx = 0;
    memcpy(buffers[0], array, sizeof(int) * count);
    frontValues[0] = array[0];
    int order = 0;
    while(AreAllEqual(buffers[currentIdx], count) == false)
    {
        int nextIdx = (currentIdx + 1) % 2;
        for(int i = 0; i < count - 1; ++i)
        {
            buffers[nextIdx][i] = buffers[currentIdx][i+1] - buffers[currentIdx][i];
        }
        --count;
        currentIdx = nextIdx;
        ++order;
        frontValues[order] = buffers[nextIdx][0];
    }
    return order + 1;
}

bool AreAllEqual(int* array, int count)
{
    for(int i = 1; i < count; ++i) {
        if(array[i] != array[i - 1])
            return false;
    }
    return true;
}