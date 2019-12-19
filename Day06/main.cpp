#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int main()
{
    char filepath[] = "Day06/input.txt";
    FILE* file = fopen(filepath,"r");
    
    unordered_map<string,string> orbits;
   
    // Gather inputs
    char satelite[4];
    int i = 0;
    char mass[4];
    while(fscanf(file,"%3c)%3c\n",mass,satelite) != EOF)
    {                    
        orbits.insert(make_pair<string,string>(satelite,mass));
    }
    fclose(file);

    // Part 1
    unordered_map<string,string>::iterator iter;
    int total = 0;
    for(iter = orbits.begin(); iter != orbits.end(); ++iter)
    {
        string recursiveMass = "";
        unordered_map<string,string>::iterator tmp = iter;
        ++total;
        while(tmp != orbits.end() && tmp->second != "COM")
        {
            ++total;
            tmp = orbits.find(tmp->second);
        }
    }
    printf("Total Orbits = %d\n",total);

    // Part 2
    iter = orbits.find("YOU");
    vector<string> visitedOrbits;
    while(iter != orbits.end() && iter->second != "COM")
    {       
        visitedOrbits.push_back(iter->second); 
        iter = orbits.find(iter->second);     
    }
    visitedOrbits.push_back(iter->second);
    
    
    iter = orbits.find("SAN");
    vector<string>::iterator searchIter;
    int count = 0;
    while(iter != orbits.end() && iter->second != "COM")
    {
        searchIter = find(visitedOrbits.begin(),visitedOrbits.end(),iter->second);
        if(searchIter != visitedOrbits.end())
        {
            break;
        }
        iter = orbits.find(iter->second);
        ++count;
    }

    int distanceToCommon = (int)std::distance(visitedOrbits.begin(), searchIter);

    printf("Common node %s\n",iter->second.c_str());
    printf("YOU -> common %d\n",distanceToCommon);
    printf("SAN -> common %d\n",count);

    int answer = distanceToCommon + count;
    printf("Part 2 Answer = %d\n",answer);

    return 0;
}