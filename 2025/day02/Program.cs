
using System.Diagnostics;
using System.Text;

public class App
{
    public static void Main()
    {
        // const string filepath = "testInput.txt";
        const string filepath = "input.txt";
        
        Part1(filepath);
        Part2(filepath);
    }

    public static void Part1(string filepath)
    {
        string input = File.ReadAllText(filepath);
        Stopwatch sw = Stopwatch.StartNew();
        string[] ranges = input.Split(',');

        ulong answer = 0;
        StringBuilder sb = new StringBuilder();

        // StringBuilder log = new StringBuilder();
        
        foreach (var range in ranges)
        {
            // log.AppendLine(range);
            string[] minmax = range.Split('-');
            string min = minmax[0];
            string max = minmax[1];
            
            // for each value in range
            // split string in two, does each side match
            // We only need to increment along the major half

            ulong minVal = ulong.Parse(min);
            ulong maxVal = ulong.Parse(max);
            
            ulong startMajor = ulong.Parse(min.Substring(0, Math.Max(min.Length / 2, 1)));
            ulong endMajor = ulong.Parse(max.Substring(0, (int)Math.Ceiling(max.Length / 2f)));
            
            int count = 0;
            
            for (ulong i = startMajor; i <= endMajor; ++i)
            {
                sb.Append(i);
                sb.Append(i);
                ulong val = ulong.Parse(sb.ToString());
                if (val >= minVal && val <= maxVal)
                {
                    // log.AppendLine($"\t{val}");
                    answer += val;
                    count += 1;
                }

                sb.Clear();
            }

            // log.AppendLine();
            // log.AppendLine($"\t count => {count}");
            // log.AppendLine($"\t total => {answer}");
            // log.AppendLine();
        }
        
        // Console.WriteLine(log.ToString());
        // Console.WriteLine(); 
        Console.WriteLine($"Answer {answer} in {sw.ElapsedMilliseconds} ms");
    }

    private static void Part2(string filepath)
    {
        string input = File.ReadAllText(filepath);
        Stopwatch sw = Stopwatch.StartNew();
        string[] ranges = input.Split(',');

        ulong answer = 0;

        var covered = new HashSet<ulong>();
        
        foreach (var range in ranges)
        {
            string[] minmax = range.Split('-');
            
            ulong min = ulong.Parse(minmax[0]);
            ulong max = ulong.Parse(minmax[1]);

            for (int i = 1; i < 20; ++i) // just picked a large max to make sure I was covered
            {
                ulong divider = (ulong)Math.Pow(10, i);
                ulong start = min / divider;
                ulong end = max / divider;
                
                if(end == 0)
                    break;
                
                for (ulong j = start; j <= end; ++j)
                {
                    if(j == 0)
                        continue;
                    
                    ulong val = j;

                    int increments = (int)(Math.Log10(j) + 1);
                    
                    for (int k = increments; k <= i; k+=increments)
                    {
                        val += (ulong)Math.Pow(10, k) * j;
                    }

                    if (covered.Add(val) && val >= min && val <= max)
                    {
                        // Console.WriteLine($"\t{val}");
                        answer += val;
                    }
                }
            }
            covered.Clear();
        }
        
        // Console.WriteLine();
        Console.WriteLine($"Answer {answer} in {sw.ElapsedMilliseconds} ms");
    }
}
