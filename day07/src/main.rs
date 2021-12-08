use std::fs;

fn main() {
    let filename = "test_input.txt";
    // part1(filename);
    println!("Sample Input");
    part2(filename);
    println!("Correct Answer 168");
    println!();
    println!("Puzzle Input");
    part2("input.txt");
    println!("Correct Answer 105461913");
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

fn part2(filename: &str)
{
    let positions: Vec<i32> = fs::read_to_string(filename)
        .expect("Failed to read file.")
        .split(",")
        .map(|char| char.parse().expect(format!("Failed to parse {}",char).as_str())).collect();
    
    let mut sum : f32 = 0.0;
    for &p in &positions
    {
        sum += p as f32;
    }
    let avg = sum / positions.len() as f32;
    println!("avg {}", avg);
    println!("rounded avg {}", avg.round());
    println!("floored avg {}", avg.floor());
    
    let floor_avg = avg.floor() as i32;
    
    let mut fuel = 0;
    for val in &positions
    {
        let diff = (floor_avg - val).abs();
        fuel += (1..diff+1).sum::<i32>();
    }
    
    println!("Answer floored {}", fuel);


    let round_avg = avg.round() as i32;

    let mut fuel = 0;
    for val in positions
    {
        let diff = (round_avg - val).abs();
        fuel += (1..diff+1).sum::<i32>();
    }

    println!("Answer rounded {}", fuel);
}