#include <stdio.h>
#include <math.h>

int main()
{
    char filepath[] = "Day1/input.txt";
    FILE* file;
    file = fopen(filepath,"r");
    
    if(file)
    {
        unsigned int requiredFuel = 0;
        
        int mass;
        while(fscanf(file,"%d\n",&mass) != EOF)
        {       
            requiredFuel += (int)floor((float)mass / 3.0f) - 2;
        }
        fclose(file);

        printf("%d\n",requiredFuel);
    }
    else
    {
        printf("Failed to open %s",filepath);
    }
    return 0;
}
