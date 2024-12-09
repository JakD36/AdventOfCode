import time
from enum import IntFlag
from pathlib import Path
from typing import Tuple, Optional


class Dir(IntFlag):
    Up = 1 << 0
    Right = 1 << 1
    Down = 1 << 2
    Left = 1 << 3
    void = 0


def part1(filepath: Path) -> None:
    start: float = time.process_time()

    with open(filepath, 'r') as f:
        lines = f.readlines()
    result = find_guard(lines)
    if result is None:
        print("Failed to find guard")
        return

    steps = 0
    total = 0
    x, y = result
    dir = Dir.Up
    while True:
        if dir == Dir.Up:
            if y == 0:
                break
            elif lines[y - 1][x] == '#':
                dir = Dir.Right
            else:
                y -= 1
                steps += 1
                lines[y] = lines[y][:x] + "X" + lines[y][x+1:]

        elif dir == Dir.Right:
            if lines[y][x + 1] == '\n':
                break
            elif lines[y][x + 1] == '#':
                dir = Dir.Down
            else:
                x += 1
                steps += 1
                lines[y] = lines[y][:x] + "X" + lines[y][x+1:]

        elif dir == Dir.Down:
            if y + 1 == len(lines):
                break
            elif lines[y + 1][x] == '#':
                dir = Dir.Left
            else:
                y += 1
                steps += 1
                lines[y] = lines[y][:x] + "X" + lines[y][x+1:]

        elif dir == Dir.Left:
            if x == 0:
                break
            elif lines[y][x - 1] == '#':
                dir = Dir.Up
            else:
                x -= 1
                steps += 1
                lines[y] = lines[y][:x] + "X" + lines[y][x+1:]

    for line in lines:
        total += line.count('X')

    print(f"Part 1 {total} in {(time.process_time() - start) * 1000} ms")


def find_guard(lines: [str]) -> Optional[Tuple[int, int]]:
    for y, line in enumerate(lines):
        for x, char in enumerate(line):
            if char == '^':
                return x, y
    return None

if __name__ == '__main__':
    # filepath = "testInput.txt"
    filepath = "input.txt"

    part1(filepath)


