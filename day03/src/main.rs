use std::fs;

fn main()
{
    let filename = "input.txt";
    // part1(filename);
    // part1_alt(filename);
    part2(filename);
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

fn bit_convert_str(line : &str, bit_count : u32) -> u32
{
    let mut val = 0; 
    let chars = line.to_ascii_lowercase();
    for (i, char) in chars.char_indices()
    {
        let enabled = char as u32 - '0' as u32;
        val |= enabled << (((bit_count - 1) - i as u32) as u32); // need to flip the bits;
    }
    return val;
}

fn part1_alt(filename : &str)
{
    let contents : String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());
    
    let ref mut lines = contents.split("\n").peekable();
    let line_count = lines.clone().count();
    let first_line = lines.peek()
        .expect("First line is empty, file did not load correctly.");
    let bit_count = first_line.chars().count();
    
    let values : Vec<u32> = lines.map(|line| bit_convert_str(line, bit_count as u32)).collect();

    let mut collection = vec![0; bit_count];
    for val in values
    {
        for i in 0..bit_count
        {
            collection[i] += (val & (1<<i) > 0) as u32;
        }
    }
    
    let mut gamma: u32 = 0;
    for i in 0..bit_count
    {
        let enabled = (collection[i] > (line_count / 2) as u32) as u32;
        gamma |= enabled << i as u32;
    }
    
    println!("Gamma {}", gamma);
    
    let mask= (2 as u32).pow(bit_count as u32) - 1;
    let answer :u32 = gamma * (!gamma & mask);

    println!("Answer = {}", answer);
}

fn find_most_popular(values : &[u32], mask: u32, accepted_range: usize) -> bool
{
    let mut count: u32 = 0;
    for i in 0..accepted_range
    {
        count += (values[i] & mask > 0) as u32;
    }
    
    return count as f32 >= (accepted_range as f32 / 2.0);
}

fn part2(filename : &str)
{
    let contents : String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());

    let ref mut lines = contents.split("\n").peekable();
    
    let first_line = lines.peek()
        .expect("First line is empty, file did not load correctly.");
    let bit_count = first_line.chars().count();

    let mut values : Vec<u32> = lines.map(|line| bit_convert_str(line, bit_count as u32)).collect();
    
    let mut accepted_range = values.len();
    for i in 0..bit_count as u32
    {
        let shift = bit_count as u32 - 1 - i;
        let mask = 1 << shift;
        
        let mut idx = 0;
        let gamma = find_most_popular(&values, mask, accepted_range);
        
        for j in 0..accepted_range
        {
            if gamma == ((values[j] & mask) > 0)
            {
                values.swap(j, idx);
                idx += 1;
            }
        }
        accepted_range = idx;
        if idx == 1
        {
            break;
        }
    }
    let o2_rating = values[0];
    println!("o2 Rating {}", o2_rating);
    
    let mut accepted_range = values.len();
    for i in 0..bit_count as u32
    {
        let shift = bit_count as u32 - 1 - i;
        let mask = 1 << shift;

        let mut idx = 0;
        let gamma = !find_most_popular(&values, mask, accepted_range);

        for j in 0..accepted_range
        {
            if gamma == ((values[j] & mask) > 0)
            {
                values.swap(j, idx);
                idx += 1;
            }
        }
        accepted_range = idx;
        if idx == 1
        {
            break;
        }
    }
    
    let co2_rating = values[0];
    println!("C02 Rating {}", co2_rating);
    
    let answer :u32 = o2_rating * co2_rating;
    
    println!("Answer = {}", answer);
}