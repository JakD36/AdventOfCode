#include <bitset>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

int Part1(const char* filepath);
// int Part2(const char* filepath);

int main()
{
    const char* filepath = "testInput.txt";
    // const char* filepath = "input.txt";
    {
        auto start = std::chrono::steady_clock::now();
        int sum = Part1(filepath);
        printf("Part 1 %d\n",sum);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 1 %lld microseconds\n", duration);
    }

    // {
    //     auto start = std::chrono::steady_clock::now();
    //     int count = Part2(filepath);
    //     printf("Part 2 %d\n", count);
    //     auto end = std::chrono::steady_clock::now();
    //     auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //     printf("Part 2 %lld microseconds\n", duration);
    // }
}


struct Buffer2D
{
    uint32_t Contents[32];
    int Width;
    int Height;
};

int TryFindVerticalReflection(Buffer2D& board);
int TryFindHorizontalReflection(Buffer2D& board);
uint32_t ReverseBits(uint32_t n, int count);

inline int ProcessVerticalReflection2(Buffer2D& board);

void PrintRowDebug(Buffer2D& board, int y, uint32_t leftMask, uint32_t rightMask, int maskWidth, uint32_t left, uint32_t right);
void PrintMaskComparison(uint32_t leftMask, uint32_t rightMask, int width);

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    const int k_size = 500;
    char contents[k_size];

    Buffer2D board {.Width = -1, .Height = 0};

    int sum = 0;
    while(fgets(contents, k_size, file) != nullptr)
    {
        if(contents[0] == '\n') // Completed board
        {
            int vertLine = TryFindVerticalReflection(board);
            if(vertLine >= 0)
            {
                sum += vertLine;
            }
            else
            {
                int horizontalLine = TryFindHorizontalReflection(board);
                if(horizontalLine < 0)
                {
                    printf("Error no reflection line found at all\n");
                    return -1;
                }
                sum += 100 * horizontalLine;
            }


            // Reset board for next
            board.Width = -1;
            board.Height = 0;
            memset(board.Contents, 0, sizeof(uint32_t) * 32);
        }
        else
        {
            // Continue adding to current board
            if(board.Width == -1)
            {
                board.Width = strlen(contents) - 1; // -1 removes the \n from the board
            }
            for(int i = 0; i < board.Width; ++i)
                if(contents[i] == '#')
                    board.Contents[board.Height] |= 1 << i;
            ++board.Height;
        }
    }
    fclose(file);


    // Off by one complete the final board
    int vertLine = TryFindVerticalReflection(board);
    if(vertLine >= 0)
    {
        sum += vertLine;
    }
    else
    {
        int horizontalLine = TryFindHorizontalReflection(board);
        if(horizontalLine < 0)
        {
            printf("Error no reflection line found at all\n");
            return -1;
        }
        sum += 100 * horizontalLine;
    }

    return sum;
}

inline int TryFindVerticalReflection(Buffer2D& board)
{
    for(int i = 1; i < board.Width; ++i)
    {
        bool found = true;
        int maskWidth = 0;
        uint32_t leftMask = 0;
        uint32_t rightMask = 0;
        int leftOffset = 0;
        if(i <= board.Width / 2)
        {
            maskWidth = i;
            leftMask = (1 << maskWidth) - 1;
            rightMask = leftMask << maskWidth;
        }
        else
        {
            maskWidth = board.Width - i;
            leftMask = ((1 << maskWidth) - 1) << (i - maskWidth);
            rightMask = leftMask << maskWidth;
            leftOffset = i - maskWidth;
        }

        // PrintMaskComparison(leftMask, rightMask, board.Width);

        for(int y = 0; y < board.Height; ++y)
        {
            uint32_t left = (board.Contents[y] & leftMask) >> leftOffset;
            uint32_t right = (board.Contents[y] & rightMask) >> (maskWidth + leftOffset);

            // PrintRowDebug(board, y, leftMask, rightMask, maskWidth, left, right);

            if(left != ReverseBits(right, maskWidth))
            {
                found = false;
                break;
            }
        }
        if(found)
            return i;
    }
    return -1;
}

inline int TryFindHorizontalReflection(Buffer2D& board)
{
    for(int i = 1; i < board.Height; ++i)
    {
        bool found = true;
        int maskWidth = 0;
        if(i <= board.Height / 2)
        {
            maskWidth = i;
        }
        else
        {
            maskWidth = board.Height - i;
        }

        for(int j = 0; j < maskWidth; ++j)
        {
            if(board.Contents[i - maskWidth + j] != board.Contents[i + maskWidth - 1 - j])
            {
                found = false;
                break;
            }
        }
        if(found)
            return i;
    }
    return -1;
}


uint32_t ReverseBits(uint32_t val, int count)
{
    uint32_t rev = 0;
    for(int i = 0; i < count; ++i)
    {
        rev |= ((val & 1 << i) >> i) << (count - 1 - i);
    }
    return rev;
}

void PrintRowDebug(Buffer2D& board, int y, uint32_t leftMask, uint32_t rightMask, int maskWidth, uint32_t left, uint32_t right)
{
    printf("Board At =======================\n");
    for(int j = 0; j < board.Width; ++j)
    {
        if((leftMask & 1 << j) != 0)
            printf("%d", ((leftMask & 1 << j) & board.Contents[y]) != 0 ? 1 : 0);
        else
            printf(".");
    }
    printf("\n");
    for(int j = 0; j < board.Width; ++j)
    {
        if((rightMask & 1 << j) != 0)
            printf("%d", ((rightMask & 1 << j) & board.Contents[y]) != 0 ? 1 : 0);
        else
            printf(".");
    }
    printf("\n");
    printf("================================\n");

    printf("Comparing ======================\n");
    printf("As Bits\n");
    for(int j = 0; j < maskWidth; ++j)
    {
        printf("%d", (1 << j & left) != 0 ? 1 : 0);

    }
    printf("\n");
    for(int j = 0; j < maskWidth; ++j)
    {
        printf("%d", (1 << j & right) != 0 ? 1 : 0);
    }
    printf("\n");
    for(int j = 0; j < maskWidth; ++j)
    {
        printf("%d", (1 << j & ReverseBits(right, maskWidth)) != 0 ? 1 : 0);
    }
    printf("\n");
    printf("As Ints\n");
    printf("%d %d\n", left, right);
    printf("Reversed\n");
    printf("%d %d\n", left, ReverseBits(right, maskWidth));
    printf("================================\n");

    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n");
}

void PrintMaskComparison(uint32_t leftMask, uint32_t rightMask, int width)
{
    printf("================================\n");
    for(int j = 0; j < width; ++j)
    {
        if((leftMask & 1 << j) != 0)
            printf("#");
        else
            printf(".");
    }
    printf("\n");
    for(int j = 0; j < width; ++j)
    {
        if((rightMask & 1 << j) != 0)
            printf("#");
        else
            printf(".");
    }
    printf("\n");
    printf("================================\n");
}
