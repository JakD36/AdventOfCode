use std::collections::HashMap;
use std::fs;

fn main() {
    let filename = "input.txt";
    part1(filename);
}

struct LineSegment
{
    start: (i32, i32),
    end: (i32, i32),
    pos: Option<(i32, i32)>
}

impl LineSegment
{
    fn new(start: (i32, i32), end: (i32, i32)) -> LineSegment
    {
        return LineSegment
        {
            start,
            end,
            pos: Some(start)
        };
    }
}

impl Iterator for LineSegment
{
    type Item = (i32, i32);
    fn next(&mut self) -> Option<Self::Item> 
    {
        if self.pos.is_none()
        {
            println!();
            return None;
        }
        println!("start ({} {}), end({} {}), cur({} {})", self.start.0, self.start.1, self.end.0, self.end.1, self.pos.unwrap().0, self.pos.unwrap().1);
        let vec = ((self.end.0 - self.start.0), self.end.1 - self.start.1);
        let dir  = (vec.0.signum(), vec.1.signum());
        
        let current = self.pos.unwrap();
        self.pos = Some((self.pos.unwrap().0 + dir.0, self.pos.unwrap().1 + dir.1));
        
        if current.0 == self.end.0 && current.1 == self.end.1 
        {
            self.pos = None;
        }
        return Some(current);
    }
}

fn get_line_seg(line_str: &str) -> LineSegment
{
    let mut points = line_str.split(" -> ");
    let mut start_pair = points.next().expect("Failed to get point from line.").split(",");
    let mut end_pair = points.next().expect("Failed to get point from line.").split(",");

    return LineSegment::new(
        (start_pair.next()
                    .expect("Failed to get x from start")
                    .parse()
                    .expect("Failed to parse start x"),
                start_pair.next()
                    .expect("Failed to get y from start")
                    .parse()
                    .expect("Failed to parse start y")),
        (end_pair.next()
                  .expect("Failed to get x from end")
                  .parse()
                  .expect("Failed to parse end x"),
              end_pair.next()
                  .expect("Failed to get y from end")
                  .parse()
                  .expect("Failed to parse end y"))
    )
}

fn part1(filename: &str)
{
    let line_segs: Vec<LineSegment> = fs::read_to_string(filename)
        .expect("Failed to read file.")
        .split("\n")
        .map(get_line_seg).collect();

    let mut collisions : HashMap<(i32,i32),u32> = HashMap::new();
    
    // println!("line seg count = {}", line_segs.len());
    for seg in line_segs
    {
        // Comment out this if statement for part 2
        // if seg.start.0 != seg.end.0 && seg.start.1 != seg.end.1
        // {
        //     continue;
        // }
        
        for point in seg
        {
            match collisions.get(&point)
            {
                Some(&p) => collisions.insert(point, p+1),
                None => collisions.insert(point, 1)
            };  
        }
    }
    
    let mut count = 0;
    for &c in collisions.values()
    {
        // println!("{}",c);
        if c >= 2
        {
            count += 1;
        }
    }
    
    println!("Answer = {}", count);
}
