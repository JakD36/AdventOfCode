#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>

void Part1(FILE* file);
void Part2(FILE* file);

class Bag{
public:
    std::string Name;
    std::vector<Bag*> Parents;
    std::vector<Bag*> Children;
    std::vector<int> ChildCount;

    Bag(std::string name)
    {
        Name = name;
    }
};

std::unordered_map<std::string,Bag*> BuildTree(FILE* file);
int RecursiveCount(int current, Bag* bag);

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

Bag* FindOrCreate(std::unordered_map<std::string,Bag*> &map, std::string name)
{
    auto current = map.find(name);
    Bag* bag = nullptr;
    if(current == end(map))
    {
        bag = new Bag(name);
        map.emplace(name,bag);
    }
    else
    {
        bag = current->second;
    }
    return bag;
}

void Part1(FILE* file)
{
    auto map = BuildTree(file);

    // count
    std::set<Bag*> uniqueBagsContaining;
    auto gold = map.find("shiny gold");
    std::queue<Bag*> queue;
    queue.push(gold->second);
    while (queue.size() > 0)
    {
        auto next = queue.front();
        queue.pop();
        for(auto parent : next->Parents){queue.push(parent);}
        if(next->Name != "shiny gold")
            uniqueBagsContaining.emplace(next);
    }
    printf("%d\n",uniqueBagsContaining.size());
}

void Part2(FILE* file)
{
    // Build our tree
    auto map = BuildTree(file);

    // count
    std::set<Bag*> uniqueBagsContaining;
    auto gold = map.find("shiny gold");

    int total = RecursiveCount(1,gold->second) - 1; // subtract 1 as we don't count the shiny gold bag

    printf("%d\n",total);
}

int RecursiveCount(int current, Bag* bag)
{
    int total = current;
    for(int i = 0; i < bag->Children.size(); ++i)
    {
        total += RecursiveCount(bag->ChildCount[i] * current, bag->Children[i]);
    }
    return total;
}

std::unordered_map<std::string,Bag*> BuildTree(FILE* file)
{
    char buffer[60];
    char buffer2[60];
    char buffer3[128];
    buffer[0] = '\0';
    buffer2[0] = '\0';
    buffer3[0] = '\0';
    std::unordered_map<std::string,Bag*> map;
    while(fscanf(file,"%s %s %*s %*s %[^\n]",buffer,buffer2,buffer3) != EOF)
    {
        std::string name(buffer); name.append(" ").append(buffer2);
        Bag* bag = FindOrCreate(map,name);

        int offset = 0;
        int count = 0, bytesRead = 0;
        char b1[30] = "\0";
        char b2[30] = "\0";
        int result = sscanf(&buffer3[offset], "%d %s %s %*s %n",&count,b1,b2,&bytesRead);
        do
        {
            if(result != 3)
                break;

            std::string subBag(b1); subBag.append(" ").append(b2);
            Bag* sub = FindOrCreate(map,subBag);
            sub->Parents.push_back(bag);
            bag->ChildCount.push_back(count);
            bag->Children.push_back(sub);
            offset += bytesRead;
            result = sscanf(&buffer3[offset], "%d %s %s %*s %n",&count,b1,b2,&bytesRead);
        }while(result != EOF);
    }
    return map;
}