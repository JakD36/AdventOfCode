#include <cstdio>
#include <cmath>
#include <chrono>

void Part1(const char* filepath);
void Part2(const char* filepath);

int main() {
    const char* filepath = "input.txt";
    // const char* filepath = "testInput.txt";

    Part1(filepath);
    Part2(filepath);
    return 0;
}

bool EvalLine(int* line, int lineCount)
{
    bool isPositive;
    bool isSafe = true;
    for (int i = 1; i < lineCount; ++i)
    {
        int diff = line[i] - line[i - 1];
        int absValue = abs(diff);
        if (absValue == 0 || absValue > 3)
        {
            isSafe = false;
            break;
        }

        if (i == 1)
        {
            isPositive = diff > 0;
        }
        else
        {
            bool currentSign = diff > 0;
            if (isPositive != currentSign)
            {
                isSafe = false;
                break;
            }
        }
    }
    return isSafe;
}

void Part1(const char* filepath)
{
    auto start = std::chrono::high_resolution_clock::now();

    FILE* f;
    if (fopen_s(&f, filepath, "r") != 0)
    {
        printf_s("Failed to read filepath: %s", filepath);
        return;
    };

    int safeReportCount = 0;
    const int valueCount = 32;
    int values[valueCount];
    const int bufferSize = 256;
    char buffer[bufferSize];

    while (fgets(buffer, bufferSize, f) != nullptr)
    {
        int read;
        int count = 0;
        char* ptr = buffer;
        while (count < valueCount && (sscanf_s(ptr, "%d%n", &values[count], &read) ) > 0)
        {
            ptr += read;
            ++count;
        }

        if (EvalLine(values, count))
            ++safeReportCount;
    }
    fclose(f);

    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf_s("Part 1 %d in %llu us\n", safeReportCount, elapsed);
}

void Part2(const char* filepath)
{
    auto start = std::chrono::high_resolution_clock::now();

    FILE* f;
    if (fopen_s(&f, filepath, "r") != 0)
    {
        printf_s("Failed to read filepath: %s", filepath);
        return;
    };

    int safeReportCount = 0;
    const int valueCount = 32;
    int values[valueCount];

    int secondaryValues[valueCount - 1];

    const int bufferSize = 256;
    char buffer[bufferSize];

    while (fgets(buffer, bufferSize, f) != nullptr)
    {
        int read;
        int count = 0;
        char* ptr = buffer;
        while (count < valueCount && (sscanf_s(ptr, "%d%n", &values[count], &read) ) > 0)
        {
            ptr += read;
            ++count;
        }

        if (EvalLine(values, count))
        {
            ++safeReportCount;
        }
        else
        {
            for (int j = 0; j < count; ++j)
            {
                for (int k = 0, l = 0; k < count; ++k)
                {
                    if (k == j)
                        continue;
                    secondaryValues[l] = values[k];
                    ++l;
                }
                if (EvalLine(secondaryValues, count - 1))
                {
                    ++safeReportCount;
                    break;
                }
            }
        }
    }
    fclose(f);

    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf_s("Part 2 %d in %llu us\n", safeReportCount, elapsed);
}