use std::fs;
use std::str::FromStr;

fn main() {
    let filename = "input.txt";
    // part1(filename);
    part2(filename);
}

fn part1(filename: &str)
{
    let mut fish: Vec<u32> = fs::read_to_string(filename)
        .expect("Failed to read file.")
        .split(",")
        .map(|char| char.parse().expect(format!("Failed to parse {}",char).as_str())).collect();
    
    const DAYS : u32 = 80;
    
    for _ in 0..DAYS
    {
        let total = fish.len();
        println!("Total {}", total);
        for j in 0..total
        {
            match fish[j]
            {
                0 =>
                    {
                        fish[j] = 6;
                        fish.push(8);
                    }
                _ => fish[j] -= 1
            }
        }
    }
    
    println!("Answer {}", fish.len());
}

fn part2(filename: &str)
{
    let fish : Vec<usize> = fs::read_to_string(filename)
        .expect("Failed to read file.")
        .split(",")
        .map(
            |char| usize::from_str(char).expect(format!("Failed to parse {}",char).as_str())
        ).collect();

    const DAYS : u32 = 256;
    
    let mut freq: [u64;9] = [0; 9];
    for f in fish
    {
        freq[f] += 1;
    }
    
    for _ in 0..DAYS
    {
        let new_fish = freq[0];
        for i in 1..9
        {
            freq[i-1] = freq[i];
        }
        freq[6] += new_fish;
        freq[8] = new_fish
    }
    
    let mut total = 0;
    for val in freq
    {
        total += val;
    }
    
    println!("Answer {}", total);
}