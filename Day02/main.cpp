#include <vector>
#include <algorithm>
#include <chrono>



int CountManual(char* passwd, char required)
{
    int countManual = 0;
    int i = 0;
    while(passwd[i] != '\0')
    {
        if(passwd[i] == required)
            ++countManual;
        ++i;
    }
    return countManual;
}

void DebugPrint(int min, int max, int count, char required, char* passwd)
{
    if(min <= count && count <= max)
    {
        printf("\033[;32m%3d <= %3d <= %3d :: %c :: %s\033[0m\n",min,count,max,required,passwd);
    }
    else if(count > max)
    {
        printf("\033[;31m%3d <= %3d <= %3d :: %c :: %s\033[0m\n",min,count,max,required,passwd);
    }
    else if(count < min)
    {
        printf("\033[;34m%3d <= %3d <= %3d :: %c :: %s\033[0m\n",min,count,max,required,passwd);
    }
}

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        int validCount = 0;

        int min, max;
        char required;
        char passwd[50];
        while(fscanf(file,"%d-%d %c: %s",&min,&max,&required,passwd) != EOF)
        {
            auto index = std::find(passwd,passwd+50,'\0');
            int count = std::count(passwd,index,required);

            DebugPrint(min,max,count,required,passwd);

//            Turns out I was encountering an error where, I wasn't clearing the buffer, or checking where the \0 null char is
//            int count = std::count(passwd,passwd+50,required);
//            std::fill(passwd,passwd+50,' ');

            if(min <= count && count <= max) {
                ++validCount;
            }
        }
        printf("\nValid %d\n",validCount);

        fclose(file);
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