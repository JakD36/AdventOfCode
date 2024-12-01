using System.Diagnostics;

// string filepath = "testInput.txt";
string filepath = "input.txt";

Part1(filepath);
Part2(filepath);

void Part1(string filepath)
{
    var sw = Stopwatch.StartNew();
    var lines = File.ReadAllLines(filepath);
    var a = new int[lines.Length];
    var b = new int[lines.Length];
    ParseLines(lines, a, b);
    Array.Sort(a);
    Array.Sort(b);
    int sum = 0;
    for (int i = 0; i < lines.Length; ++i)
    {
        sum += Math.Abs(b[i] - a[i]);
    }
    
    Console.WriteLine($"Part 1 {sum} in {sw.ElapsedTicks} ticks");
}

void Part2(string filepath)
{
    var sw = Stopwatch.StartNew();
    var lines = File.ReadAllLines(filepath);
    var a = new int[lines.Length];
    var b = new int[lines.Length];
    ParseLines(lines, a, b);
    Array.Sort(a);
    Array.Sort(b);
    
    int similarity = 0;
    int count = 0;
    
    int j = 0;
    int i = 0;
    while(i < lines.Length && j < lines.Length)
    {
        if (a[i] < b[j])
        {
            int current = a[i];
            int mul = 0;
            while (i < lines.Length && current == a[i])
            {
                ++mul;
                ++i;
            }
            
            similarity += current * count * mul;
            count = 0;
        }
        else if(a[i] > b[j])
        {
            similarity += a[i] * count;
            count = 0;
            ++j;
        }
        else
        {
            ++count;
            ++j;
        }
    }
    
    Console.WriteLine($"Part 2 {similarity} in {sw.ElapsedTicks} ticks");
}

void ParseLines(string[] lines, int[] a, int[] b)
{
    for (int i = 0; i < lines.Length; ++i)
    {
        string[] numbers = lines[i].Split(" ");
        a[i] = int.Parse(numbers[0]);
        b[i] = int.Parse(numbers[numbers.Length-1]);
    }
}
