#include <cstdio>
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

struct vec2
{
    int X; int Y;
};

void Part1(const char* filepath)
{
    auto start = std::chrono::high_resolution_clock::now();

    FILE* f;
    if (fopen_s(&f, filepath, "r") != 0)
    {
        printf_s("Failed to read filepath: %s", filepath);
        return;
    };

    const int bufferSize = 140 * 141;
    char buffer[bufferSize];

    int sum = 0;
    int lineCount = 0;

    char* ptr = buffer;
    int len = 0;
    while (fgets(ptr, bufferSize, f) != nullptr)
    {
        len = strlen(ptr);
        ptr += len - 1;
        ++lineCount;
    }

    fclose(f);

    vec2 offsets[] = {
        {1,0},
        {1,1},
        {0, 1},
        {-1,1},

        {-1,0},
        {-1,-1},
        {0, -1},
        {1, -1}
    };
    const char match[] = "MAS";

    for(int i = 0; i < lineCount; ++i)
    {
        for(int j = 0; j < len; ++j)
        {
            int idx = i * len + j;
            if(buffer[idx] == 'X')
            {
                for(int k = 0; k < 8; ++k)
                {
                    bool found = true;
                    for(int l = 1; l < 4; ++l)
                    {
                        vec2 offset = offsets[k];
                        offset = {offset.X * l, offset.Y * l};
                        int row = i + offset.Y;
                        int col = j + offset.X;

                        int index = col + row * len;
                        if(row < 0 || row >= lineCount || col < 0 || col >= len
                            || buffer[index] != match[l - 1])
                        {
                            found = false;
                            break;
                        }
                    }
                    sum += found;
                }
            }
        }
    }

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

    const int bufferSize = 140 * 141;
    char buffer[bufferSize];

    int sum = 0;
    int lineCount = 0;

    char* ptr = buffer;
    int len = 0;
    while (fgets(ptr, bufferSize, f) != nullptr)
    {
        len = strlen(ptr);
        ptr += len - 1;
        ++lineCount;
    }

    fclose(f);

    vec2 offsets[] = {
        {1,1},
        {-1,-1},

        {-1,1},
        {1, -1}
    };


    uint64_t match = (1ULL << ('M' - 'A')) | (1ULL << ('S' - 'A'));
    match |= match << 32;

    for(int i = 0; i < lineCount; ++i)
    {
        for(int j = 0; j < len; ++j)
        {
            int idx = i * len + j;
            if(buffer[idx] == 'A')
            {
                uint64_t mask = 0;
                for(int k = 0; k < 4; ++k)
                {
                    vec2 offset = offsets[k];
                    int row = i + offset.Y;
                    int col = j + offset.X;

                    if(row < 0 || row >= lineCount || col < 0 || col >= len)
                    {
                        break;
                    }

                    int index = col + row * len;
                    mask |= (1ULL << (buffer[index] - 'A')) << (32 * (k / 2));
                }
                if(mask == match)
                {
                    sum += 1;
                }
            }
        }
    }
    
    uint64_t elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
    printf_s("Part 2 %d in %llu us\n", sum, elapsed);
}
