use std::fs;

fn main() {
    let filename = "input.txt";
    part1(filename);
}

fn part1(filename: &str)
{
    let mut positions: Vec<i32> = fs::read_to_string(filename)
        .expect("Failed to read file.")
        .split(",")
        .map(|char| char.parse().expect(format!("Failed to parse {}",char).as_str())).collect();
    
    positions.sort();
    
    let median = positions[positions.len()/2];
    println!("Median {}", median);
    
    let mut fuel = 0;
    for val in positions
    {
        fuel += (median - val).abs();
    }
    
    println!("Answer {}", fuel);
}