#include <vector>
#include <algorithm>
#include <chrono>

void Part1(FILE* file);

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
        printf("Failed to open %s",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

bool Exists(char keys[8][4], char required[])
{
    for(int i = 0; i < 8 * 4; ++i)
    {
        if(strncmp(&keys[i][0],required,4) == 0)
            return true;
    }
    return false;
}

bool IsValidPassport(char keys[8][4])
{
    bool found = true;
    found &= Exists(keys,"ecl");
    found &= Exists(keys, "pid");
    found &= Exists(keys, "eyr");
    found &= Exists(keys, "hcl");
    found &= Exists(keys, "byr");
    found &= Exists(keys, "iyr");
    found &= Exists(keys, "hgt");
    return found;
}

void Part1(FILE* file)
{
    char line[255];
    char keys[8][4];
    char vals[8][100];

    int count = 0;

    // Read all lines in block to find entries for passport
    int i = 0;
    while(fgets(line,100, file) != nullptr)
    {
        if(line[0] != '\n')
        {
//            printf("%s\t",line);
            int offset = 0, n = 0;
            while(sscanf(&line[offset],"%[^:]:%s %n",keys[i],vals[i],&n) != EOF)
            {
                offset += n;
                ++i;
            }
        }
        else
        {
            //evaluate
            count += IsValidPassport(keys);

            // Reset, Clear the keys, vals
            i = 0;
            for(int i = 0; i < 8; ++i)
            {
                keys[i][0] = '\0'; vals[i][0] = '\0';
            }
            continue;
        }
    }

    printf("Valid Count = %d\n",count);
}