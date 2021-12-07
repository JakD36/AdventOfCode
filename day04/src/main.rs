use std::fs;
use std::str::FromStr;

fn main()
{
    let filename = "input.txt";
    // part1(filename);
    part2(filename);
}

const BOARD_SIZE: usize = 5;

#[derive(Copy, Clone)]
struct Board
{
    values : [[u32; BOARD_SIZE]; BOARD_SIZE],
}

fn gen_winning_markers() -> [u32; 2 * BOARD_SIZE] 
{
    let mut rows = [0; 2*BOARD_SIZE];
    for row in 0..BOARD_SIZE
    {
        for col in 0..BOARD_SIZE
        {
            let val = (1 << (BOARD_SIZE * row + col)) as u32;
            rows[row] |= val;
            rows[BOARD_SIZE + col] |= val;
        }
    }
    return rows;
}

fn part1(filename : &str)
{
    let contents: String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());
    let ref mut lines = contents.split("\n");

    let called_nums : Vec<u32> = lines.next()
        .expect("First line is empty, file did not load correctly.")
        .split(",")
        .map(|num| u32::from_str(num)
            .expect(format!("Failed to parse {} to u32",num).as_str()))
        .collect();

    let mut boards :Vec<Board> = vec!();
    
    loop 
    {
        if lines.next().is_none()
        {
            break;
        }
        
        let mut board = Board{ values: [[0; BOARD_SIZE]; BOARD_SIZE]};
        for row_i in 0..BOARD_SIZE
        {
            let value_iter = lines
                .next()
                .expect("Failed to get next row")
                .split(" ")
                .filter_map(|num| u32::from_str(num).ok());
            
            for (i, val) in value_iter.enumerate()
            {
                board.values[row_i][i] = val;    
            }
        }
        boards.push(board);
    }
    
    let winning_markers = gen_winning_markers();
    for val in winning_markers
    {
        print!("{} ",val);
    }
    println!();
    
    let mut markers = vec![0; boards.len()];
    for val in called_nums
    {
        for (board_id, &board) in boards.iter().enumerate()
        {
            for (row_idx, &row) in board.values.iter().enumerate()
            {
                for (col_idx, &col) in row.iter().enumerate()
                {
                     markers[board_id] |= ((col == val) as u32) << (BOARD_SIZE * row_idx + col_idx);
                }
            }
        }
        
        for (i, board_marker) in markers.iter().enumerate()
        {
            for winning_mark in winning_markers
            {
                if (winning_mark & board_marker) == winning_mark
                {
                    println!("Winning mark val {}", winning_mark);
                    println!("board val {}", board_marker);
                    println!("board1 val {}", markers[1] & winning_mark);
                    println!("Found a Winnder! board {}", i);
                    let mut score = 0;
                    for (row_idx, &row) in boards[i].values.iter().enumerate()
                    {
                        for (col_idx, &col) in row.iter().enumerate()
                        {
                            let boolean = (board_marker & (1 << (BOARD_SIZE * row_idx + col_idx)) > 0) as u32;
                            let add_val = col * (!boolean & 1);
                            println!("add val {}", add_val);
                            score += add_val;
                        }
                    }
                    println!("Answer {}", score * val);
                    return;
                }
            }
        }
    }
}

fn part2(filename : &str)
{
    let contents: String = fs::read_to_string(filename)
        .expect(format!("Failed to read the {}", filename).as_str());
    let ref mut lines = contents.split("\n");

    let called_nums : Vec<u32> = lines.next()
        .expect("First line is empty, file did not load correctly.")
        .split(",")
        .map(|num| u32::from_str(num)
            .expect(format!("Failed to parse {} to u32",num).as_str()))
        .collect();

    let mut boards :Vec<Board> = vec!();

    loop
    {
        if lines.next().is_none()
        {
            break;
        }

        let mut board = Board{ values: [[0; BOARD_SIZE]; BOARD_SIZE]};
        for row_i in 0..BOARD_SIZE
        {
            let value_iter = lines
                .next()
                .expect("Failed to get next row")
                .split(" ")
                .filter_map(|num| u32::from_str(num).ok());

            for (i, val) in value_iter.enumerate()
            {
                board.values[row_i][i] = val;
            }
        }
        boards.push(board);
    }

    let winning_markers = gen_winning_markers();
    for val in winning_markers
    {
        print!("{} ",val);
    }
    println!();
    
    let mut winning_boards: u128 = 0;
    let mut winners_found : u8 = 0;
    
    let mut markers = vec![0; boards.len()];
    for val in called_nums
    {
        for (board_id, &board) in boards.iter().enumerate()
        {
            for (row_idx, &row) in board.values.iter().enumerate()
            {
                for (col_idx, &col) in row.iter().enumerate()
                {
                    markers[board_id] |= ((col == val) as u32) << (BOARD_SIZE * row_idx + col_idx);
                }
            }
        }

        for (i, board_marker) in markers.iter().enumerate()
        {
            for winning_mark in winning_markers
            {
                let has_won = ((winning_mark & board_marker) == winning_mark) as u128;
                let winner_mask = has_won << (i as u128);
                let new_winner = ((winner_mask & winning_boards) == 0) as u8;
                winners_found += new_winner * (has_won > 0) as u8;
                winning_boards |= winner_mask;
                if winners_found == boards.len() as u8
                {
                    println!("Found a last Winnder! board {}", i);
                    let score = calculate_board_score(&boards[i], &board_marker);
                    println!("Answer {}", score * val);
                    return;
                }
            }
        }
    }
}

fn calculate_board_score(board: &Board, board_marker: &u32) -> u32
{
    let mut score = 0;
    for (row_idx, &row) in board.values.iter().enumerate()
    {
        for (col_idx, &col) in row.iter().enumerate()
        {
            let boolean = (board_marker & (1 << (BOARD_SIZE * row_idx + col_idx)) > 0) as u32;
            let add_val = col * (!boolean & 1);
            score += add_val;
        }
    }
    
    return score;
}
