use std::fs;
use std::str::FromStr;

fn main()
{
    let filename = "input.txt"; // 1502
    // part1(filename);
    part2(filename); // 1538
}

fn part1(filename : &str) 
{
    let contents : String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());
    let ref mut lines = contents.split("\n");
    
    let first_line = lines.next().expect("Line count is 0, failed to find a value.");
    let mut prev = u32::from_str(first_line).expect(format!("Failed to parse {} as a u32", first_line).as_str());
    let mut increase_count = 0;
    
    for line in lines
    {
        let val = u32::from_str(line).expect(format!("Failed to parse {} as a u32", line).as_str());
        
        increase_count += (val > prev) as u32;

        let debug = if val > prev 
        {
            "Increased"
        } 
        else 
        {
            "Decreased"
        };
        
        println!("{} > {} = {}", val, prev, );

        prev = val;
    }
    
    println!("Increase count {}", increase_count)
}

fn part2(filename: &str) 
{
    const WINDOW_SIZE: usize = 3;
    
    let contents = fs::read_to_string(filename).expect(format!("Failed to read the {}", filename).as_str());
    let ref mut lines = contents.split("\n");
    let line_count = contents.split("\n").count();

    let mut increase_count = 0;
    
    let mut window_sum: [u32; WINDOW_SIZE] = [0; WINDOW_SIZE];
    let mut prev = None;
    for i in 0..WINDOW_SIZE-1 
    {
        let first_line = lines.next().expect("Ran out of lines from the file to read.");
        let val = u32::from_str(first_line).expect(format!("Failed to parse {} as a u32", first_line).as_str());
        for j in 0..i+1
        {
            window_sum[j] += val;
        }
    }
    
    for i in 0..line_count-(WINDOW_SIZE-1)*2
    {
        let first_line = lines.next().expect("Ran out of lines from the file to read.");
        let val = u32::from_str(first_line).expect(format!("Failed to parse {} as a u32", first_line).as_str());
        
        for j in 0..WINDOW_SIZE
        {
            window_sum[j] += val;
        }
        
        if prev != None
        {
            increase_count += (window_sum[i % WINDOW_SIZE] > prev.unwrap()) as u32;
        }
        prev = Some(window_sum[i % WINDOW_SIZE]);
        println!("{}", window_sum[i % WINDOW_SIZE]);
        
        window_sum[i % WINDOW_SIZE] = 0;
    }

    println!("{} {} {}", window_sum[0], window_sum[1], window_sum[2]);
    println!();
    // do opposite of the first loop and only add to the second two and then only the last
    for i in line_count-(WINDOW_SIZE-2)..line_count+1
    {
        let line = lines.next().expect("Ran out of lines from the file to read.");
        println!("line: {}", line);
        let val = u32::from_str(line).expect(format!("Failed to parse {} as a u32", line).as_str());
        
        let idx = i % WINDOW_SIZE; println!("Index {}", idx);
        
        for j in idx..idx+2
        {
            println!("Add {} to index {}", val,j % WINDOW_SIZE);
            window_sum[j % WINDOW_SIZE] += val;
        }
        println!("{} {} {}", window_sum[0], window_sum[1], window_sum[2]);

        if prev != None
        {
            increase_count += (window_sum[i % WINDOW_SIZE] > prev.unwrap()) as u32;
        }
        
        println!()
    }

    println!("Increase count {}", increase_count)
}
