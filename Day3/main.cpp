#include <stdio.h>
#include <math.h>

#include <set>
#include <vector>
#include <array>

using namespace std;

int main()
{
    char filepath[] = "Day3/input.txt";
    FILE* file = fopen(filepath,"r");
    
    vector< array<int,2> > lines;
    set< array<int,2> > lineSet;
    vector< array<int,2> > intersections;

    const array<int,2> origin = {0,0};

    char dir;
    int val;
    
    array<int,2> lastPoint = {2,0};

    lastPoint = origin;
    
    while(fscanf(file,"%1c",&dir) != EOF && dir != '\n')
    {         
        fscanf(file,"%d",&val);
        while(val > 0)
        {
            switch(dir)
            {
                case 'U':
                    ++lastPoint[1];
                    break;
                case 'R':
                    ++lastPoint[0];
                    break;
                case 'D':
                    --lastPoint[1];
                    break;
                case 'L':
                    --lastPoint[0];
                    break;
            }
            --val;
            lines.push_back(lastPoint);
            lineSet.insert(lastPoint);
        }
        
        fscanf(file,",");
    }

    lastPoint = origin;
    
    int steps = 0;
    int minSteps = INT_MAX;

    while(fscanf(file,"%1c",&dir) != EOF && dir != '\n')
    {         
        fscanf(file,"%d",&val);
        while(val > 0)
        {
            switch(dir)
            {
                case 'U':
                    ++lastPoint[1];
                    break;
                case 'R':
                    ++lastPoint[0];
                    break;
                case 'D':
                    --lastPoint[1];
                    break;
                case 'L':
                    --lastPoint[0];
                    break;
            }
            ++steps;
            --val;
            
            set<array<int,2> >::iterator setIter = lineSet.find(lastPoint);
            if(setIter != lineSet.end())
            {
                int startSearch = std::distance(lineSet.begin(),setIter);
                vector<array<int,2> >::iterator it = std::find(lines.begin(),lines.end(),lastPoint); // perform a search to make sure this point exists on the first line/wire before searching for the amount of steps
                if(it != lines.end())
                {
                    // Intersection found
                    intersections.push_back(lastPoint);
                    int stepsAtIntersect = std::distance(lines.begin(), it) + 1; // Add 1 as distance provides an off by 1 answer
                    
                    int sum = steps + stepsAtIntersect;

                    printf("intersect %d,%d sum steps = %d\n",lastPoint[0],lastPoint[1],sum);

                    if(sum < minSteps)
                        minSteps = sum;
                }
            }
               
        }
        fscanf(file,",");
    }

    int minDistance = INT_MAX;
    for(int i = 0; i < intersections.size(); ++i)
    {
        int manhattenDistance = abs(intersections[i][0] - origin[0]) + abs(intersections[i][1] - origin[1]);
        if(manhattenDistance < minDistance)
            minDistance = manhattenDistance;
    }

    if(intersections.size() == 0)
    {
        printf("Error No intersections found\n");
        return 2;
    }
        
    printf("Answer = %d\n",minSteps);

    return 0;
}
  


