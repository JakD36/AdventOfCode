
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
        var lines = File.ReadAllLines(filepath);
        int count = 0;
        int value = 50;

        const int dialSize = 100;
        
        foreach (var line in lines)
        {
            var direction = line[0];
            var rotation = int.Parse(line.Substring(1));

            switch (direction)
            {
                case 'R':
                    value = (value + rotation) % dialSize;
                    break;
                case 'L':
                    value = ((value - rotation) % dialSize + dialSize) % dialSize;
                    break;
            }
            
            if (value == 0)
            {
                ++count;
            }
        }
        Console.WriteLine($"Answer: {count}");
    }
    
    public static void Part2(string filepath) 
    {
        var lines = File.ReadAllLines(filepath);
        int count = 0;
        int value = 50;

        const int dialSize = 100;
        
        foreach (var line in lines)
        {
            var direction = line[0];
            var rotation = int.Parse(line.Substring(1));
            
            switch (direction)
            {
                case 'R':
                {
                    int tmp = value + rotation;
                    count += tmp / dialSize;

                    value = tmp % dialSize;
                }
                    break;
                case 'L':
                {
                    // There has to be a better solution for this but I'm tired
                    int increment = 0;
                    int current = value;
                    {
                        for (int i = 0; i < rotation; ++i)
                        {
                            current = (current - 1 + dialSize) % dialSize;
                            if (current == 0)
                            {
                                ++increment;
                            }
                        }    
                    }
                    // int old = value;
                    count += increment;
                    value = ((value - rotation) % dialSize + dialSize) % dialSize;
                    
                    // Console.WriteLine($"{old} {rotation} {current} {value}");
                }
                    break;
            }
        }
        Console.WriteLine($"Answer: {count}");
    }
}
