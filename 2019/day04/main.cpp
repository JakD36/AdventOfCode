#include <stdio.h>
#include <math.h>

int main()
{
    int start = 382345;
    int end = 843167;

    int array[6];
    int count = 0;

    for(int i = start; i < end; ++i)
    {
        bool neverDecreases = true;
        bool doubleFound = false;

        // Split the provided number into individual digits
        int sum = 0;
        for(int n = 0; n < 6; ++n)
        {
            array[n] = (i - sum) / pow(10,6-1-n) ;
            sum += array[n] * pow(10,6-1-n);
        }

        int n = 1;
        int lastValue = array[0];
        
        while(n < 6)
        {
            if(array[n] < lastValue)
            {
                neverDecreases = false;
                break;
            }
            else if(array[n] == lastValue)
            {
                ++n;
                int batchCount = 0;
                while(n < 6)
                {
                    if(array[n] < lastValue)
                    {
                        neverDecreases = false;
                        break;
                    }
                    if(array[n] == lastValue)
                        ++batchCount;
                    else
                        break;
                    ++n;
                }   
                if(batchCount == 0)
                    doubleFound = true;
            }

            lastValue = array[n];
            ++n;
        }
        
        if(doubleFound && neverDecreases)
        {
            ++count;
        }    
    }

    printf("Answer = %d\n",count);

    return 0;
}
