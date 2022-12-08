#include <chrono>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <numeric>

void Part1(FILE* file);
void Part2(FILE* ticketRanges, FILE* validTickets);
void Part2_RemoveDuds(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    FILE* f1 = fopen("ticketRanges.txt","r");
    FILE* f2 = fopen("validTickets.txt","r");

    if(f1 && f2)
    {
        Part2(f1,f2);
    }
    else
    {
        printf("Failed to open files\n");
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    fclose(f1);
    fclose(f2);
    return 0;
}

struct Field
{
public:
    char m_name[255];
    int x1,x2,x3,x4;

    unsigned int matchCount;
    unsigned int match;

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

void Part2_RemoveDuds(FILE* file)
{
    std::vector<int> invalidVals;
    std::vector<Field> fields;
    char buf[255];
    InputType type = InputType::NONE;

    std::vector<int> ticketNumbers;

    FILE* writeFile = fopen("validTickets.txt","w");

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
                bool failed = false;
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
                        failed = true;
                        break;
                    }
                    offset += bytesRead;
                }
                if(!failed)
                    fprintf(writeFile,"%s", buf);
            }
        }
    }
    fclose(writeFile);
}

void Part2(FILE* ticketRanges, FILE* validTickets)
{
    unsigned int all = 0;
    for(unsigned int i = 0; i < 20; ++i)
        all |= 1U << i;

    std::vector<Field> fields;
    char key[255];
    int x1 = 0,x2 = 0,x3 = 0,x4 = 0;
    while(fscanf(ticketRanges,"%[^:]: %d-%d or %d-%d\n",key,&x1,&x2,&x3,&x4) != EOF)
    {
        Field tmp;
        strcpy(tmp.m_name,key);
        tmp.x1 = x1; tmp.x2 = x2; tmp.x3 = x3; tmp.x4 = x4;
        tmp.match = all;
        tmp.matchCount = 0;
        fields.push_back(tmp);
    }

    char buf[255];
    while(fgets(buf,255,validTickets) != nullptr)
    {
        int offset = 0;
        int val = 0, bytesRead = 0;
        int end = std::find(buf,buf+255,'\0') - buf;
        unsigned int index = 0;
        while(offset < end && sscanf(&buf[offset],"%d,%n",&val,&bytesRead) > 0)
        {
            for(int i = 0, count = fields.size(); i < count; ++i)
            {
                fields[i].match &= ~(!fields[i].IsValid(val) << index);
            }
            ++index;
            offset += bytesRead;
        }
    }

    // Count the number of valid values in each, so we can sort by what has the least
    for(unsigned int i = 0; i < 20; ++i)
    {
        for(unsigned int j = 0; j < 20; ++j)
        {
            fields[i].matchCount += (fields[i].match & (1U << j)) > 0;
        }
    }

    std::sort(begin(fields),end(fields),[&](auto& a, auto& b){return b.matchCount>a.matchCount;});

    int yourTicket[] = {67,107,59,79,53,131,61,101,71,73,137,109,157,113,173,103,83,167,149,163};
    unsigned long total = 1;
    // Now our list is sorted we can start eliminating possibilities
    unsigned int found = 0;
    for(int i = 0; i < 20; ++i)
    {
//        for(int j = 0; j < 20; ++j)
//        {
//            if ((fields[i].match & (1 << j)) > 0)
//            {
//                printf("1 ");
//            } else
//                printf("  ");
//        }
//        printf("\n");
        fields[i].match &= ~found;
//        for(int j = 0; j < 20; ++j)
//        {
//            if ((fields[i].match & (1 << j)) > 0)
//            {
//                printf("1 ");
//            } else
//                printf("  ");
//        }
//        printf("\n");
        found |= fields[i].match;
        for(int j = 0; j < 20; ++j)
        {
            if ((fields[i].match & (1 << j)) > 0)
            {
                if(strncmp(fields[i].m_name,"departure",9) == 0)
                {
                    total *= yourTicket[j];
                }
                printf("%s %d\n",fields[i].m_name,j);
            }
        }
//        printf("\n\n");
    }
    printf("Total %ld\n",total);
}