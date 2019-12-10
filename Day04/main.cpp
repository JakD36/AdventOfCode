#include <math.h>
#include <stdio.h>

int main()
{
    // 1 in 10 match 
    // 10 in 100 match
    // 100 in 1000 match
    // 1000 in 10,000 match
    // 10,000 in 100,000 match 

    const int start = 382345;
    const int end = 843167;

    int array[6];
    
    int count = 0;

    for(int i = start; i < end; ++i)
    {
        bool containsDouble = false;
        bool neverDecreases = true;

        int sum = 0;
        for(int n = 0; n < 6; ++n)
        {
            array[n] = (i - sum) / pow(10,6-1-n) ;
            sum += array[n] * pow(10,6-1-n);
        }

        int doubles = 0;
        for(int n = 1; n < 6; ++n)
        {
            if(array[n] == array[n-1])
            {
                ++doubles;
                if(n > 1 && array[n-2] == array[n])
                {
                    --doubles;
                }
            }
            else if (array[n] < array[n-1])
            {
                neverDecreases = false;
                break;
            }
        }
        if(doubles > 0 && neverDecreases)
        {
            ++count;
        }
            
    }

    printf("Answer = %d\n",count);

    return 0;
}
