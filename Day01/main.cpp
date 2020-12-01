#include <vector>
#include <algorithm>
#include <chrono>

inline int FindSolutionFromBruteForce(std::vector<int> &inputs);
inline int FindSolutionFromSortedInputs(std::vector<int> &inputs);

int main() {
    auto start = std::chrono::steady_clock::now();

    char filepath[] = "input.txt";
    FILE* file;
    file = fopen(filepath,"r");

    std::vector<int> inputs;
    inputs.reserve(300);

    if(file)
    {
        int val;
        while(fscanf(file,"%d\n",&val) != EOF)
        {
            inputs.push_back(val);
        }
        
        std::sort(begin(inputs),end(inputs));
        int solution = FindSolutionFromSortedInputs(inputs);
//        int solution = FindSolutionFromBruteForce(inputs);
        printf("%d\n",solution);

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

inline int FindSolutionFromBruteForce(std::vector<int> &inputs)
{
    for(int a : inputs)
    {
        for(int b : inputs)
        {
            for(int c :inputs)
            {
                int sum = a + b + c;
                if(sum == 2020)
                {
                    return a * b * c;
                }
            }
        }
    }
    return -1;
}

inline int FindSolutionFromSortedInputs(std::vector<int> &inputs)
{
    for(int a : inputs)
    {
        for(int b : inputs)
        {
            int sum1 = a + b;
            int diff = sum1 - 2020;
            if(diff > 0) //
                continue;
            for(int c : inputs)
            {
                int sum2 = sum1 + c;
                int diff = sum2 - 2020;
                if(diff < 0)
                {
                    continue;
                }
                else if(diff == 0)
                {
                    // found the solution
                    return a * b * c;
                }
                else
                {
                    break; // Our sum is greater than 2020 no solution after will work
                }
            }
        }
    }
    return -1;
}