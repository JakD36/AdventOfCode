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
   
    char satelite[4];
    int i = 0;
    char mass[4];
    while(fscanf(file,"%3c)%3c\n",mass,satelite) != EOF)
    {                    
        // printf("%3s orbits %3s",satelite,mass);
        orbits.insert(make_pair<string,string>(satelite,mass));
    }
    fclose(file);

    // Part 1
    unordered_map<string,string>::iterator iter;
    int total = 0;
    // FILE* out = fopen("Day06/debug.txt","a");
    for(iter = orbits.begin(); iter != orbits.end(); ++iter)
    {
        string recursiveMass = "";
        unordered_map<string,string>::iterator tmp = iter;
        // fprintf(out,"%s\t",tmp->first.c_str());
        // fprintf(out,"%s\t",tmp->second.c_str());
        ++total;
        while(tmp != orbits.end() && tmp->second != "COM")
        {
            ++total;
            tmp = orbits.find(tmp->second);
            // if(tmp != orbits.end())
            //     fprintf(out,"%s\t",tmp->second.c_str());
        }
        // fprintf(out,"\n");
    }
    // fclose(out);
    printf("%d\n",total);

    // Part 2
    unordered_map<string,string>::iterator uIter = orbits.find("YOU");
    unordered_map<string,string>::iterator sanIter = orbits.find("SAN");
    unordered_map<string,string>::iterator tmp = uIter;

    vector<string> visitedOrbits;
    while(tmp != orbits.end() && tmp->second != "COM")
    {       
        visitedOrbits.push_back(tmp->second); 
        tmp = orbits.find(tmp->second);     
    }
    visitedOrbits.push_back(tmp->second);
    
    vector<string>::iterator printIter;
    FILE* debug = fopen("Day06/debug.txt","a");
    for(printIter = visitedOrbits.begin(); printIter != visitedOrbits.end(); ++printIter)
    {
        fprintf(debug,"%s\t",(*printIter).c_str());
    }
    fprintf(debug,"\n");
    

    tmp = sanIter;
    vector<string>::iterator setIter;
    int count = 0;
    bool answerFound = false;
    fprintf(debug,"%s\t",tmp->first.c_str());
    while(tmp != orbits.end() && tmp->second != "COM")
    {
        
        setIter = find(visitedOrbits.begin(),visitedOrbits.end(),tmp->second);
        fprintf(debug,"%s\t",tmp->second.c_str());
        if(setIter != visitedOrbits.end() && !answerFound)
        {
            printf("Common node %s\n",tmp->second.c_str());
            printf("YOU -> common %d\n",std::distance(visitedOrbits.begin(), setIter));
            printf("SAN -> common %d\n",count);

            int answer = (int)(std::distance(visitedOrbits.begin(), setIter)) + count;
            printf("Answer = %d\n",answer);
            answerFound = true;
            // break;
        }
        tmp = orbits.find(tmp->second);
        ++count;
    }
    fprintf(debug,"%s",tmp->second.c_str());
    fprintf(debug,"\n");
    fclose(debug);

    return 0;
}