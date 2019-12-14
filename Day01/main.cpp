#include <stdio.h>
#include <math.h>

int FuelForMass(int mass);

int main()
{
    char filepath[] = "Day01/input.txt";
    FILE* file;
    file = fopen(filepath,"r");
    
    if(file)
    {
        unsigned int requiredFuel = 0;
        
        int mass;
        while(fscanf(file,"%d\n",&mass) != EOF)
        {       
            int moduleFuel = FuelForMass(mass);
            int fuel = moduleFuel;
            while(true)
            {
                fuel = FuelForMass(fuel);  
                if(fuel > 0)
                {
                    moduleFuel += fuel;
                }
                else
                {
                    break;
                }
            }
            requiredFuel += moduleFuel;
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


int FuelForMass(int mass)
{
    return (int)floor((float)mass / 3.0f) - 2;
}