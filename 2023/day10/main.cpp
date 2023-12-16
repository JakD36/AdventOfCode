#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

const int k_size = 142;
struct int2 {
    int x;
    int y;
};

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

struct Node
{
    int2 Position;
    Direction ContinueIn;
};

enum class Result {
    NONE,
    FOUND_NEXT,
    FINISHED_LOOP
};

bool Contains(Node* array, int count, int2 vec);

void FindStart(char contents[k_size][k_size], int rows, int cols, int2& start);
Result TryFindNext(char contents[k_size][k_size], int2 current, Direction direction, Node& next);

int Part1(const char* filepath);
int Part2(const char* filepath);

int main()
{
    // const char* filepath = "testInput.txt";
    const char* filepath = "testInput3.txt";
    // const char* filepath = "input.txt";
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
        int count = Part2(filepath);
        printf("Part 2 %d\n", count);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("Part 2 %lld microseconds\n", duration);
    }
}

int Part1(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    char contents[k_size][k_size];

    int count = 0;
    while(fgets((char *)contents[count], k_size, file) != nullptr)
    {
        ++count;
    }
    fclose(file);

    int2 start = {-1,-1};
    int width = strlen(contents[0]);
    FindStart(contents, count, width, start);
    Direction tryDirections[4] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};

    Node nodes[15'000] = { {start, Direction::NONE} };
    for(int i = 0; i < 4; ++i) {
        if(TryFindNext(contents, start, tryDirections[i], nodes[1]) == Result::FOUND_NEXT)
        {
            nodes[0].ContinueIn = tryDirections[i];
            break;
        }
    }

    int loopLen = 2;
    while(TryFindNext(contents, nodes[loopLen - 1].Position, nodes[loopLen - 1].ContinueIn, nodes[loopLen]) != Result::FINISHED_LOOP) {
        ++loopLen;
    }

    return loopLen / 2;
}

int Part2(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    char contents[k_size][k_size];

    int rowCount = 0;
    while(fgets((char *)contents[rowCount], k_size, file) != nullptr)
    {
        ++rowCount;
    }
    fclose(file);


    int2 start = {-1,-1};
    int width = strlen(contents[0]);
    FindStart(contents, rowCount, width, start);
    int minX, minY, maxX, maxY;
    minX = start.x;
    minY = start.y;
    maxX = start.x;
    maxY = start.y;

    Direction tryDirections[4] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    Node nodes[15'000] = { {start, Direction::NONE} };
    for(int i = 0; i < 4; ++i) {
        if(TryFindNext(contents, start, tryDirections[i], nodes[1]) == Result::FOUND_NEXT)
        {
            minX = nodes[1].Position.x < minX ? nodes[1].Position.x : minX;
            minY = nodes[1].Position.y < minY ? nodes[1].Position.y : minY;
            maxX = nodes[1].Position.x > maxX ? nodes[1].Position.x : maxX;
            maxY = nodes[1].Position.y > maxY ? nodes[1].Position.y : maxY;
            nodes[0].ContinueIn = tryDirections[i];
            break;
        }
    }

    int loopLen = 2;
    while(TryFindNext(contents, nodes[loopLen - 1].Position, nodes[loopLen - 1].ContinueIn, nodes[loopLen]) != Result::FINISHED_LOOP) {
        minX = nodes[loopLen].Position.x < minX ? nodes[loopLen].Position.x : minX;
        minY = nodes[loopLen].Position.y < minY ? nodes[loopLen].Position.y : minY;
        maxX = nodes[loopLen].Position.x > maxX ? nodes[loopLen].Position.x : maxX;
        maxY = nodes[loopLen].Position.y > maxY ? nodes[loopLen].Position.y : maxY;
        ++loopLen;
    }

    int2 inPos = nodes[loopLen-1].Position;
    int2 outPos = nodes[1].Position;
    char& startChar = contents[start.y][start.x];


    int2 diff = {outPos.x - inPos.x, outPos.y - inPos.y};
    if(abs(diff.x) == 2) {
        startChar = '-';
    }
    else if(abs(diff.y) == 2) {
        startChar = '|';
    }
    else if(diff.x == 1 && diff.y == 1) {
        startChar = 'L';
    }
    else if(diff.x == 1 && diff.y == -1) {
        startChar = 'F';
    }
    else if(diff.x == -1 && diff.y == 1) {
        startChar = 'J';
    }
    else if(diff.x == -1 && diff.y == -1) {
        startChar = '7';
    }

    int count = 0;
    for(int y = 0; y < rowCount; ++y)
    {
        bool winding = false;
        for(int x = 1; x < width; ++x)
        {
            bool contains = Contains(nodes, loopLen, {x,y});
            bool leftContains = Contains(nodes, loopLen, {x-1,y});
            // if(contains != leftContains)
            // {
            //     winding = !winding;
            // }
            // else
            // {
            //     if(contents[y][x] == '|')
            //         winding = !winding;
            // }
            if(contains) {
                switch (contents[y][x])
                {
                    case '|':
                    case 'F':
                    case 'L':
                        winding = !winding;
                    break;
                    // case 'J':
                    // case '7':
                    //     if(leftContains == false)
                    break;;
                }
            }
            else if (contains == false && leftContains)
                winding = !winding;

            if(winding && contains == false)
            {
                ++count;
                contents[y][x] = 'I';
                continue;
            }
            contents[y][x] = '0';
        }
    }

    for(int y = 0; y < rowCount; ++y)
    {
        for(int x = 0; x < width; ++x)
        {
            printf("%c", contents[y][x]);
        }
        printf("\n");
    }

    return count;
}

void FindStart(char contents[k_size][k_size], int rows, int cols, int2& start)
{
    for(int i = 0; i < rows; ++i)
    {
        for(int j = 0; j < cols; ++j)
        {
            if(contents[i][j] == 'S')
            {
                start = {
                    .x = j,
                    .y = i
                };
            }
        }
    }
}

Result TryFindNext(char contents[k_size][k_size], int2 current, Direction direction, Node& next)
{
    switch (direction)
    {
        case Direction::UP:
            switch (contents[current.y - 1][current.x])
            {
                case '|': // Continue up
                    next = {{current.x, current.y - 1}, Direction::UP};
                    return Result::FOUND_NEXT;
                case '7': // move left
                    next = {{current.x, current.y - 1}, Direction::LEFT};
                    return Result::FOUND_NEXT;
                case 'F': // move right
                    next = {{current.x, current.y - 1}, Direction::RIGHT};
                    return Result::FOUND_NEXT;
                case 'S':
                    return Result::FINISHED_LOOP;
            }
            break;
        case Direction::DOWN:
            switch (contents[current.y + 1][current.x])
            {
                case '|': // continue down
                    next = {{current.x, current.y + 1}, Direction::DOWN};
                    return Result::FOUND_NEXT;
                case 'L': // move right
                    next = {{current.x, current.y + 1}, Direction::RIGHT};
                    return Result::FOUND_NEXT;
                case 'J': // move left
                    next = {{current.x, current.y + 1}, Direction::LEFT};
                    return Result::FOUND_NEXT;
                case 'S':
                    return Result::FINISHED_LOOP;
            }
            break;
        case Direction::LEFT:
            switch (contents[current.y][current.x - 1])
            {
                case '-': // Continue left
                    next = {{current.x - 1, current.y}, Direction::LEFT};
                    return Result::FOUND_NEXT;
                case 'L': // move up
                    next = {{current.x - 1, current.y}, Direction::UP};
                    return Result::FOUND_NEXT;
                case 'F': // move down
                    next = {{current.x - 1, current.y}, Direction::DOWN};
                    return Result::FOUND_NEXT;
                case 'S':
                    return Result::FINISHED_LOOP;
            }
            break;
        case Direction::RIGHT:
            switch (contents[current.y][current.x + 1])
            {
                case '-': // Continue right
                    next = {{current.x + 1, current.y}, Direction::RIGHT};
                    return Result::FOUND_NEXT;
                case 'J': // move up
                    next = {{current.x + 1, current.y}, Direction::UP};
                    return Result::FOUND_NEXT;
                case '7': // move down
                    next = {{current.x + 1, current.y}, Direction::DOWN};
                    return Result::FOUND_NEXT;
                case 'S':
                    return Result::FINISHED_LOOP;
            }
            break;
    }
    return Result::NONE;
}

bool Contains(Node* array, int count, int2 vec)
{
    for(int i = 0; i < count; ++i) {
        if(array[i].Position.x == vec.x && array[i].Position.y == vec.y)
            return true;
    }
    return false;
}