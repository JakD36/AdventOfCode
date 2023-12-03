#include <chrono>
#include <cstdio>

struct Results
{
    int Part1;
    int Part2;
};

int Part1(const char* filepath);

int main()
{
    // char filepath[] = "testInput.txt";
    // char filepath[] = "testInput2.txt";
    char filepath[] = "input.txt";

    {
        auto start = std::chrono::steady_clock::now();
        int sum = Part1(filepath);
        printf("Part 1 %i\n", sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("%lld microseconds\n", duration);
    }

    return 0;
}

struct SymbolPosition
{
    int X;
    int Y;
};

struct PartNumber
{
    int Num;
    int X;
    int Y;
    int End;
};


int Part1(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");

    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return -1;
    }

    const int size = 256;
    char contents[size];

    int line = 0;
    int symbolCount = 0;
    int numCount = 0;
    // Yes I kept running the input until I found a buffer size big enough :)
    SymbolPosition symbols[1024 * 2];
    PartNumber numbers[1024 * 2];

    while(fgets(contents, size, file) != nullptr)
    {
        int i = 0;
        while(contents[i] != '\n' && contents[i] != '\0')
        {
            if(contents[i] == '.')
            {
                ++i;
                // continue;
            }
            else if(contents[i] >= '0' && contents[i] <= '9')
            {
                int offset;
                int num;
                int countRead = sscanf(&contents[i], "%i%n", &num, &offset);
                if(countRead != 1)
                {
                    printf("Failed to read number.");
                    return -1;
                }

                numbers[numCount++] = PartNumber
                {
                    .Num = num,
                    .X = i,
                    .Y = line,
                    .End = i + offset - 1
                };
                i += offset;
                // continue;
            }
            else // Symbol found
            {
                symbols[symbolCount++] = SymbolPosition
                {
                    .X = i,
                    .Y = line
                };
                ++i;
                // continue;
            }
        }
        ++line;
    }
    fclose(file);

    int sum = 0;
    for(int i = 0; i < symbolCount; ++i)
    {
        SymbolPosition& symbol = symbols[i];
        for(int j = 0; j < numCount; j++)
        {
            PartNumber& number = numbers[j];
            if(abs(symbol.Y - number.Y) < 2)
            {
                if(abs(symbol.X - number.X) < 2 || abs(symbol.X - number.End) < 2)
                {
                    sum += number.Num;
                }
            }
            else if(symbol.Y - number.Y <= -2) // small speed increase with early return 1 ms to 0.7 ms
            {
                break; // we can early return as all following numbers are also going to be out of bounds
            }
        }
    }

    // printf("%i %i", symbolCount, numCount);

    return sum;
}
