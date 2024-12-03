#include <cstdio>
#include <cmath>
#include <chrono>

void Part1(const char* filepath);

enum class PatternMatchMode
{
    CHAR,
    INTEGER,
    ERROR
};

int ExtractIntegersFromNextPattern(const char* buffer, const int bufferSize,const char*& endPtr, const char* pattern, int* values, const int maxValues);

int main() {
    const char* filepath = "input.txt";
    // const char* filepath = "testInput.txt";

    Part1(filepath);
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
        const char* endPtr;
        int values[2];
        const char* ptr = buffer;
        while (ExtractIntegersFromNextPattern(ptr, bufferSize, endPtr, "mul(%d,%d)", values, 2) > 0)
        {
            sum += values[0] * values[1];
            ptr = endPtr;
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

int ExtractIntegersFromNextPattern(const char* buffer, const int bufferSize,const char*& endPtr, const char* pattern, int* values, const int maxValues)
{
    const int patternSize = strlen(pattern);
    int patternIndex = 0;
    PatternMatchMode mode = ExtractMode(pattern, patternSize, patternIndex);

    int valuesSaved = 0;

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
                        endPtr = &buffer[i + 1];
                        return valuesSaved;
                    }

                    mode = ExtractMode(pattern, patternSize, patternIndex);
                }
                else
                {
                    valuesSaved = 0;
                    patternIndex = 0;
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
                        return valuesSaved;
                    }
                    mode = ExtractMode(pattern, patternSize, patternIndex);

                    values[valuesSaved] = (int)val;
                    ++valuesSaved;
                    i += (int)digitsRead - 1; // want to compensate for the extra number of characters read
                }
                else
                {
                    valuesSaved = 0;
                    patternIndex = 0; // reset failed to match pattern
                    mode = ExtractMode(pattern, patternSize, patternIndex);
                }
                break;
        }
    }

    endPtr = &buffer[bufferSize];
    return 0;
}