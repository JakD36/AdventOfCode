#include <stdio.h>
#include <string>
#include <unordered_map>

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

    return 0;
}