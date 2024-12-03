#include <cstdio>
#include <cmath>
#include <chrono>

void Part1(const char* filepath);
void Part2(const char* filepath);

enum class PatternMatchMode
{
    CHAR,
    INTEGER,
    ERROR
};

bool TryMatchPattern(const char* buffer, const char* bufferEnd,const char*& endPtr, const char* pattern, int* values, const int maxValues);

int main() {
    const char* filepath = "input.txt";
    // const char* filepath = "testInput.txt";

    Part1(filepath);
    Part2(filepath);
    return 0;
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

    const int bufferSize = 2048 * 5;
    char buffer[bufferSize];

    int sum = 0;

    while (fgets(buffer, bufferSize, f) != nullptr)
    {
        size_t len = strlen(buffer);
        const char* bufferEnd = buffer + len;
        const char* endPtr;
        int values[2];
        const char* ptr = buffer;
        while (*ptr != '\0' && ptr < bufferEnd)
        {
            if (TryMatchPattern(ptr, bufferEnd, endPtr, "mul(%d,%d)", values, 2))
            {
                sum += values[0] * values[1];
            }
            ptr = endPtr + 1;
        }
    }
    fclose(f);

    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf_s("Part 1 %d in %llu us\n", sum, elapsed);
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

    const int bufferSize = 2048 * 5;
    char buffer[bufferSize];

    int sum = 0;

    bool enabled = true;

    while (fgets(buffer, bufferSize, f) != nullptr)
    {
        size_t len = strlen(buffer);
        const char* bufferEnd = buffer + len;

        const char* endPtr;
        int values[2];
        const char* ptr = buffer;
        while (*ptr != '\0' && ptr < bufferEnd)
        {
            if (TryMatchPattern(ptr, bufferEnd, endPtr, "mul(%d,%d)", values, 2))
            {
                if (enabled)
                    sum += values[0] * values[1];
            }
            else if (TryMatchPattern(ptr, bufferEnd, endPtr, "do()", values, 2))
            {
                enabled = true;
            }
            else if (TryMatchPattern(ptr, bufferEnd, endPtr, "don't()", values, 2))
            {
                enabled = false;
            }
            ptr = endPtr + 1;
        }
    }
    fclose(f);

    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf_s("Part 1 %d in %llu us\n", sum, elapsed);
}


PatternMatchMode ExtractMode(const char* pattern, const int patternSize, int& index)
{
    if (index >= 0 && index < patternSize)
    {
        if (pattern[index] == '%')
        {
            if (index + 1 >= patternSize)
            {
                return PatternMatchMode::ERROR;
            }

            switch (pattern[index + 1])
            {
                case 'd':
                    ++index;
                    return PatternMatchMode::INTEGER;
                default:
                    return PatternMatchMode::ERROR;
            }
        }
        return PatternMatchMode::CHAR;
    }
    return PatternMatchMode::ERROR;
}

bool TryMatchPattern(const char* buffer, const char* bufferEnd,const char*& endPtr, const char* pattern, int* values, const int maxValues)
{
    const int patternSize = strlen(pattern);
    int patternIndex = 0;
    PatternMatchMode mode = ExtractMode(pattern, patternSize, patternIndex);

    int valuesSaved = 0;

    int bufferSize = bufferEnd - buffer;
    for (int i = 0; i < bufferSize && buffer[i] != '\0'; ++i)
    {
        switch (mode)
        {
            case PatternMatchMode::CHAR:
                if (buffer[i] == pattern[patternIndex])
                {
                    ++patternIndex;
                    if (patternIndex == patternSize)
                    {
                        endPtr = &buffer[i];
                        return true;
                    }

                    mode = ExtractMode(pattern, patternSize, patternIndex);
                }
                else
                {
                    endPtr = buffer;
                    return false;
                }
                break;
            case PatternMatchMode::INTEGER:
                char* toLongEndPtr;
                long val = strtol(&buffer[i], &toLongEndPtr, 10);
                size_t digitsRead = toLongEndPtr - &buffer[i];
                if (digitsRead > 0)
                {
                    ++patternIndex;
                    if (patternIndex == patternSize)
                    {
                        endPtr = &buffer[i + 1];
                        return true;
                    }
                    mode = ExtractMode(pattern, patternSize, patternIndex);

                    if (valuesSaved == maxValues)
                    {
                        endPtr = buffer;
                        return false;
                    }
                    values[valuesSaved] = (int)val;
                    ++valuesSaved;
                    i += (int)digitsRead - 1; // want to compensate for the extra number of characters read
                }
                else
                {
                    endPtr = buffer;
                    return false;
                }
                break;
        }
    }

    endPtr = buffer;
    return false;
}