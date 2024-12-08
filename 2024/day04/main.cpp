#include <cstdio>
#include <chrono>

void Part1(const char* filepath);

int main() {
    const char* filepath = "input.txt";
    // const char* filepath = "testInput.txt";

    Part1(filepath);
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
    char print[bufferSize];

    memset(print, '.', 140 * 140);

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
                        int col = j + offset.X;

                        int index = idx + offset.X + offset.Y * len;

                        if(index < 0 || index > lineCount * len
                            || col < 0 || col > len
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
