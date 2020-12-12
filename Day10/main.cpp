#include <chrono>
#include <cstdio>
#include <vector>
#include <algorithm>

void Part1(FILE* file);
void Part2(FILE* file);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    if(file)
    {
        Part2(file);
    }
    else
    {
        printf("Failed to open %s\n",filepath);
    }

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("%d microseconds\n",duration);
    return 0;
}

void Part1(FILE* file)
{
    int freq[4] = {0,0,0,0};
    std::vector<int> adapters = {0};
    int val;

    while(fscanf(file,"%d",&val) != EOF)
    {
        adapters.push_back(val);
    }
    std::sort(begin(adapters),end(adapters));

    for(int i = 1, count = adapters.size(); i < count; ++i)
    {
        ++freq[adapters[i] - adapters[i-1]];
    }
    ++freq[3];

    printf("0=%d 1=%d 2=%d 3=%d\tTotal=%d\n",freq[0],freq[1],freq[2],freq[3],freq[1]*freq[3]);
}

void Part2(FILE* file)
{
    std::vector<int> adapters = {0};
    int val;

    while(fscanf(file,"%d",&val) != EOF)
    {
        adapters.push_back(val);
    }

    std::sort(begin(adapters),end(adapters));

    int count = adapters.size();
    adapters.push_back(adapters[count++-1]+3);

    int* freq = new int[count];

    for(int i = 0; i < count; ++i)
    {
        for(int j = i+1; j < count; ++j)
        {
            if(adapters[j]-adapters[i] <= 3)
            {
                ++freq[i];
            }
            else
            {
                break;
            }
        }
//        printf("%d\n",freq[i]);
    }

    // validate my assumption that, branches follow a distinct pattern that simplifies
    // the process of calculating number of branches
    // if we have a frequency of 3, it must be followed by 1,1,1 or 2,1,1 or 3,2,1
    // if we have a frequency of 2, it must be followed by 1,1
    /*
    for(int i = 0; i < count-1; ++i)
    {
        if(freq[i] > 1)
            printf("%d -> %d %d %d == %d -> %d %d %d\n",freq[i],freq[i+1],freq[i+2],freq[i+3],adapters[i],adapters[i+1],adapters[i+2],adapters[i+3]);
        if(freq[i] == 3)
        {
            assert((freq[i+1] == 1 && freq[i+2] == 1 && freq[i+3] == 1) ||
                           (freq[i+1] == 2 && freq[i+2] == 1 && freq[i+3] == 1) ||
                           (freq[i+1] == 3 && freq[i+2] == 2 && freq[i+3] == 1) );
        }
        if(freq[i] == 2)
        {
            assert(freq[i+1] == 1 && freq[i+2] == 1);
        }
        // no assert at this point so the idea is correct
    }
    */

    unsigned long branches = 1;
    for(int i = 0; i < count-1;)
    {
        switch (freq[i])
        {
            case 1:
                ++i; // No extra branch so don't need to change multiply
                break;
            case 2:
                branches *= 2; // Freq 2 is followed by 1,1 so we just have 2 new branches to multiply by
                i += 3;
                break;
            case 3:
                int sum = freq[i+1] + freq[i+2] + freq[i+3];
                switch (sum)
                {
                    case 3: // 1,1,1
                        branches *= 3;
                        i+=4;
                        break;
                    case 4: // 2,1,1
                        branches *= 4;
                        i += 4;
                        break;
                    case 6: // 3,2,1
                        // Going by the pattern we have established where 2 is followed by 1,1
                        // it must go 3,2,1,1 if the next 3 freqs add to 6,
                        // so we can multiply the current number of branches by 7 3+2+1+1, and skip 5 indexes ahead
                        branches *= 7;
                        i += 5;
                        break;
                }
                break;
        }
    }
    printf("Total %lu\n",branches);


}
