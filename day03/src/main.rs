use std::fs;

fn main()
{
    let filename = "input.txt";
    part1(filename);
    
}

fn part1(filename : &str)
{
    let contents : String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());
    let ref mut lines = contents.split("\n").peekable();
    let line_count = lines.clone().count() as u32;
    let first_line = lines.peek().expect("First line is empty, file did not load correctly.");
    let bit_count = first_line.chars().count();
    
    
    let mut collection = vec![0; bit_count];
    
    for line in lines
    {
        let chars = line.to_ascii_lowercase();
        for (i, char) in chars.char_indices()
        {
            collection[i] += char as u32 - '0' as u32;    
        }
    }
    
    let mut gamma: u32 = 0;
    for i in 0..bit_count
    {
        let enabled = (collection[i] > (line_count / 2)) as u32;
        gamma |= enabled << (((bit_count - 1) - i) as u32);
    }

    println!();
    for val in &collection
    {
        print!("{} ", val);
    }
    println!("\n");
    
    println!("Gamma {}", gamma);
    
    let mask= (2 as u32).pow(bit_count as u32) - 1;
    let answer :u32 = gamma * (!gamma & mask);
    
    println!("Answer = {}", answer);
}

