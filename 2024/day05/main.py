import time
from enum import Enum
from functools import cmp_to_key
from pathlib import Path
from typing import Tuple


class Mode(Enum):
    input_part1 = 0,
    input_part2 = 1


def part1(filepath: Path) -> None:
    start: float = time.process_time()
    pairs: [Tuple[int, int]] = []
    updates: [[int]] = []

    mode: Mode = Mode.input_part1
    with open(filepath, 'r') as f:
        line: str = f.readline()
        while line:
            line = line.strip()
            if mode == Mode.input_part1:
                if line.isspace() or len(line) == 0:
                    mode = Mode.input_part2
                else:
                    a, b = line.split('|', 1)
                    pairs.append((int(a),int(b)))
            else:
                updates.append([int(x) for x in line.split(',')])

            line = f.readline()

    total: int = 0
    for update in updates:
        passed = True
        for a, b in pairs:
            if a in update and b in update and update.index(a) > update.index(b):
                passed = False
                break
        if passed:
            total += update[len(update) // 2]
    print(f"Part 1 {total} in {(time.process_time() - start) * 1000} ms")


def part2(filepath: Path) -> None:
    start: float = time.process_time()
    pairs: [Tuple[int, int]] = []
    updates: [[int]] = []

    mode: Mode = Mode.input_part1
    with open(filepath, 'r') as f:
        line: str = f.readline()
        while line:
            line = line.strip()
            if mode == Mode.input_part1:
                if line.isspace() or len(line) == 0:
                    mode = Mode.input_part2
                else:
                    a, b = line.split('|', 1)
                    pairs.append((int(a),int(b)))
            else:
                updates.append([int(x) for x in line.split(',')])

            line = f.readline()

    total: int = 0
    for update in updates:
        passed = True
        for a, b in pairs:
            if a in update and b in update and update.index(a) > update.index(b):
                passed = False
                break
        if passed:
            pass
            # total += update[len(update) // 2]
        else:
            def cmp(a: int, b: int) -> int:
                for x, y in pairs:
                    if a == x and b == y:
                        if update.index(a) < update.index(b):
                            return 1
                        else:
                            return -1
                    elif b == x and a == y:
                        if update.index(b) < update.index(a):
                            return 1
                        else:
                            return -1
                return 0

            update = sorted(update, key = cmp_to_key(cmp))
            total += update[len(update) // 2]

    print(f"Part 2 {total} in {(time.process_time() - start) * 1000} ms")


if __name__ == '__main__':
    # filepath = "testInput.txt"
    filepath = "input.txt"

    part1(filepath)
    part2(filepath)


