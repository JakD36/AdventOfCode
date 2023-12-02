#include <chrono>
#include <cstdio>

int Part1(const char* filepath);
// uint64_t Part2(const char* filepath);

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


    // {
    //     auto start = std::chrono::steady_clock::now();
    //     uint64_t sum = Part2(filepath);
    //     printf("Part 2 %" PRIu64 "\n", sum);
    //     auto end = std::chrono::steady_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //     printf("%lld microseconds\n", duration);
    // }

    return 0;
}

int Part1(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");

    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return 1;
    }

    int idSum = 0;

    const int size = 256;
    char contents[size];

    while(fgets(contents, size, file) != nullptr)
    {
        int lineId = -1;
        int offset = 0;
        int successfullyRead = sscanf(contents, "Game %i: %n", &lineId, &offset);
        if(successfullyRead == 0)
        {
            printf("Failed to read line, unexpected format\n");
            return -1;
        }

        int maxRed = -1;
        int maxGreen = -1;
        int maxBlue = -1;

        bool endOfLineFound = false;
        while (endOfLineFound == false)
        {
            int count;
            char colour[] = "\0\0\0\0\0\0";
            int nextOffset = 0;
            sscanf(contents + offset, "%i %s%n", &count, &colour, &nextOffset);

            int len = 0;
            switch (colour[0])
            {
                case 'g':
                    maxGreen = maxGreen < count ? count : maxGreen;
                    len = 5;
                    break;
                case 'r':
                    maxRed = maxRed < count ? count : maxRed;
                    len = 3;
                    break;
                case 'b':
                    maxBlue = maxBlue < count ? count : maxBlue;
                    len = 4;
                    break;
            }

            char endChar = colour[len];
            switch (endChar)
            {
                case ';': // going to ignore ; and , as we should only need to know maxes for round 1
                    break;
                case ',':
                    break;
                default:
                    endOfLineFound = true;
                    break;
            }

            offset += nextOffset;
        }

        if(maxRed <= 12 && maxGreen <= 13 && maxBlue <= 14)
        {
            idSum += lineId;
        }
    }

    fclose(file);
    return idSum;
}

