#include <chrono>
#include <cstdio>

struct Results
{
    int Part1;
    int Part2;
};

Results Part1(const char* filepath);

int main()
{
    // char filepath[] = "testInput.txt";
    // char filepath[] = "testInput2.txt";
    char filepath[] = "input.txt";

    {
        auto start = std::chrono::steady_clock::now();
        Results results = Part1(filepath);
        printf("Answers %i %i\n", results.Part1, results.Part2);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("%lld microseconds\n", duration);
    }

    return 0;
}

Results Part1(const char* filepath)
{
    FILE* file;
    file = fopen(filepath,"r");

    if(file == nullptr)
    {
        printf("Failed to open %s\n",filepath);
        return {-1, -1};
    }

    int idSum = 0;
    int cubePower = 0;

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
            return {-1, -1};
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

        cubePower += maxRed * maxGreen * maxBlue;
    }

    fclose(file);
    return {idSum, cubePower};
}
