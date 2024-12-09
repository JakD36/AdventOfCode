import time
from dataclasses import dataclass
from enum import Enum, IntFlag
from pathlib import Path
from sys import flags
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


@dataclass
class Orientation:
    x: int
    y: int
    dir: Dir


def walk(lines: [str], start: Orientation, visited: [[dir]], path_taken: [Orientation]) -> bool:
    x = start.x
    y = start.y
    dir = start.dir
    path_taken.append(start)

    while True:
        if dir == Dir.Up:
            if y == 0:
                return False
            elif lines[y - 1][x] == '#':
                dir = Dir.Right
            elif visited[y - 1][x] & dir != 0:
                return True
            else:
                y -= 1
                visited[y][x] |= dir
                path_taken.append(Orientation(x,y,dir))

        elif dir == Dir.Right:
            if lines[y][x + 1] == '\n':
                return False
            elif lines[y][x + 1] == '#':
                dir = Dir.Down
            elif visited[y][x + 1] & dir != 0:
                return True
            else:
                x += 1
                visited[y][x] |= dir
                path_taken.append(Orientation(x, y, dir))

        elif dir == Dir.Down:
            if y + 1 == len(lines):
                return False
            elif lines[y + 1][x] == '#':
                dir = Dir.Left
            elif visited[y + 1][x] & dir != 0:
                return True
            else:
                y += 1
                visited[y][x] |= dir
                path_taken.append(Orientation(x, y, dir))

        elif dir == Dir.Left:
            if x == 0:
                return False
            elif lines[y][x - 1] == '#':
                dir = Dir.Up
            elif visited[y][x - 1] & dir != 0:
                return True
            else:
                x -= 1
                visited[y][x] |= dir
                path_taken.append(Orientation(x, y, dir))



def part2(filepath: Path) -> None:
    start: float = time.process_time()

    with open(filepath, 'r') as f:
        lines = f.readlines()
    result = find_guard(lines)
    if result is None:
        print("Failed to find guard")
        return

    x, y = result

    rows = len(lines)
    cols = len(lines[0]) - 1

    grid = [[Dir.void for x in range(cols)] for y in range(rows)]
    start_p = Orientation(x, y, Dir.Up)
    path_taken = []
    walk(lines, start_p, grid, path_taken)

    dup = lines.copy()
    tested = set()
    loops = 0
    for idx, p in enumerate(path_taken):
        # print(f"Checking point {idx} {p.x}, {p.y}")

        if (p.x, p.y) in tested:
            continue

        tested.add((p.x, p.y))

        x = p.x
        y = p.y

        # reset the grid
        for i in range(rows):
            for j in range(cols):
                grid[i][j] = Dir.void

        for i in range(len(dup)):
            dup[i] = lines[i]

        dup[y] = dup[y][:x] + '#' + dup[y][x + 1:]
        if walk(dup, start_p, grid, []):
            loops += 1

    print(f"Part 2 {loops} in {(time.process_time() - start) * 1000} ms")


if __name__ == '__main__':
    # filepath = "testInput.txt"
    filepath = "input.txt"

    part1(filepath)
    part2(filepath)

