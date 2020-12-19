#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <numeric>

void Part1(FILE* file);
void Part2(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Part1(file);
    }
    else
    {
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    fclose(file);
    return 0;
}

struct Field
{
public:
    char m_name[255];
    int x1,x2,x3,x4;

    bool IsValid(int val)
    {
        return (x1 <= val && val <= x2) || (x3 <= val && val <= x4);
    }
};

enum class InputType
{
    NONE,
    YOUR_TICKET,
    NEARBY,
};

void Part1(FILE* file) // between 28998 - 29022
{
    std::vector<int> invalidVals;
    std::vector<Field> fields;
    char buf[255];
    InputType type = InputType::NONE;

    std::vector<int> ticketNumbers;

    int sum = 0;

    while(fgets(buf,255,file) != nullptr)
    {
        char key[255], key2[255];
        int x1 = 0,x2 = 0,x3 = 0,x4 = 0;
        if(sscanf(buf,"%[^:]: %d-%d or %d-%d",key,&x1,&x2,&x3,&x4) == 5)
        {
            Field tmp;
            strcpy(tmp.m_name,key);
            tmp.x1 = x1; tmp.x2 = x2; tmp.x3 = x3; tmp.x4 = x4;
            fields.push_back(tmp);
        }
        else if(sscanf(buf,"%s %s",key,key2) == 2)
        {
            if(strcmp("your",key) == 0 && strcmp("ticket:",key2) == 0)
            {
                type = InputType::YOUR_TICKET;
            }
            if(strcmp("nearby",key) == 0 && strcmp("tickets:",key2) == 0)
            {
                type = InputType::NEARBY;
            }
        }
        else if (buf[0] != '\n')
        {
            if(type == InputType::YOUR_TICKET)
            {
                int offset = 0;
                int val = 0, bytesRead = 0;
                while(sscanf(&buf[offset],"%d,%n",&val,&bytesRead) > 0)
                {
                    ticketNumbers.push_back(val);
                    offset += bytesRead;
                }
            }
            else if (type == InputType::NEARBY)
            {
                int offset = 0;
                int val = 0, bytesRead = 0;
                int end = std::find(buf,buf+255,'\0') - buf;
                while(offset < end && sscanf(&buf[offset],"%d,%n",&val,&bytesRead) > 0)
                {
                    bool valid = false;
                    for(int i = 0, count = fields.size(); i < count; ++i)
                    {
                        if(fields[i].IsValid(val))
                        {
                            valid = true;
                            break;
                        }
                    }
                    if(!valid)
                    {
                        sum += val;
                        printf("%d\n",val);
                        invalidVals.push_back(val);
                    }
                    offset += bytesRead;
                }
            }
        }
    }

    printf("acc %d\n",std::accumulate(begin(invalidVals),end(invalidVals),0));
    printf("sum %d\n",sum);
}

void Part2(FILE* file) // between 28998 - 29022
{
    std::vector<int> invalidVals;
    std::vector<Field> fields;
    char buf[255];
    InputType type = InputType::NONE;

    std::vector<int> ticketNumbers;

    int sum = 0;

    while(fgets(buf,255,file) != nullptr)
    {
        char key[255], key2[255];
        int x1 = 0,x2 = 0,x3 = 0,x4 = 0;
        if(sscanf(buf,"%[^:]: %d-%d or %d-%d",key,&x1,&x2,&x3,&x4) == 5)
        {
            Field tmp;
            strcpy(tmp.m_name,key);
            tmp.x1 = x1; tmp.x2 = x2; tmp.x3 = x3; tmp.x4 = x4;
            fields.push_back(tmp);
        }
        else if(sscanf(buf,"%s %s",key,key2) == 2)
        {
            if(strcmp("your",key) == 0 && strcmp("ticket:",key2) == 0)
            {
                type = InputType::YOUR_TICKET;
            }
            if(strcmp("nearby",key) == 0 && strcmp("tickets:",key2) == 0)
            {
                type = InputType::NEARBY;
            }
        }
        else if (buf[0] != '\n')
        {
            if(type == InputType::YOUR_TICKET)
            {
                int offset = 0;
                int val = 0, bytesRead = 0;
                while(sscanf(&buf[offset],"%d,%n",&val,&bytesRead) > 0)
                {
                    ticketNumbers.push_back(val);
                    offset += bytesRead;
                }
            }
            else if (type == InputType::NEARBY)
            {
                int offset = 0;
                int val = 0, bytesRead = 0;
                int end = std::find(buf,buf+255,'\0') - buf;
                while(offset < end && sscanf(&buf[offset],"%d,%n",&val,&bytesRead) > 0)
                {
                    bool valid = false;
                    for(int i = 0, count = fields.size(); i < count; ++i)
                    {
                        if(fields[i].IsValid(val))
                        {
                            valid = true;
                            break;
                        }
                    }
                    if(!valid)
                    {
                        sum += val;
                        printf("%d\n",val);
                        invalidVals.push_back(val);
                    }
                    offset += bytesRead;
                }
            }
        }
    }

    printf("acc %d\n",std::accumulate(begin(invalidVals),end(invalidVals),0));
    printf("sum %d\n",sum);
}