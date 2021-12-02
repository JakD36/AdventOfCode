use std::fs;
use std::str::FromStr;

fn main()
{
    let filename = "input.txt"; 
    // part1(filename);
    part2(filename);
}

fn part1(filename : &str)
{
    let contents : String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());
    let ref mut lines = contents.split("\n");
    
    let mut horizontal = 0;
    let mut depth = 0;
    
    for line in lines
    {
        let ref mut key_value = line.split(" ");
        let command = key_value.next().expect("Missing the direction command");
        let value = key_value.next().expect("Missing the value of command");
        let value = i32::from_str(value)
            .expect(format!("Failed to parse {} as a i32", value).as_str());
        match command
        {
            "forward" => horizontal += value,
            "up" => depth -= value,
            "down" => depth += value,
            unexpected => assert!(false, "Unexpected command {}", unexpected)
        }
    }
    println!("Answer = {}", depth * horizontal)
}

fn part2(filename : &str)
{
    let contents : String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());
    let ref mut lines = contents.split("\n");

    let mut horizontal = 0;
    let mut depth = 0;
    let mut aim = 0;

    for line in lines
    {
        let ref mut key_value = line.split(" ");
        let command = key_value.next().expect("Missing the direction command");
        let value = key_value.next().expect("Missing the value of command");
        let value = i32::from_str(value)
            .expect(format!("Failed to parse {} as a i32", value).as_str());
        match command
        {
            "forward" =>
                {
                    horizontal += value;
                    depth += aim * value;
                },
            "up" => aim -= value,
            "down" => aim += value,
            unexpected => assert!(false, "Unexpected command {}", unexpected)
        };
    }
    println!("Answer = {}", depth * horizontal)
}
