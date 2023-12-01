const std = @import("std");
const eql = std.mem.eql;

pub fn main() !void {
    var filepath = "input.txt";

    const timer = std.time.Timer;
    var t = try timer.start();

    {
        var sum = try part1(filepath);
        std.debug.print("{d}\n", .{sum});
    }

    {
        var sum = try part2(filepath);
        std.debug.print("{d}\n", .{sum});
    }

    const duration: f64 = @floatFromInt(t.read());
    std.debug.print("{d:.5} ms\n", .{duration / 1000000.0});
}

fn part1(filepath: []const u8) !u64 {
    const file = try std.fs.cwd().openFile(filepath, std.fs.File.OpenFlags{ .mode = .read_only });
    defer file.close();

    var buffer: [100]u8 = undefined;

    var sum: u64 = 0;
    while (try nextLine(file.reader(), &buffer)) |input| {
        var lhs: u8 = 0;
        for (input) |character| {
            var val = character - '0';
            if (val >= 0 and val < 10) {
                lhs = val;
                break;
            }
        }

        // how do I loop backwards sensibly, surely all this casting isn't right
        var rhs: u8 = 0;
        var end = input.len - 1;
        var idx: i8 = @as(i8, @intCast(end));
        while (idx >= 0) : (idx -= 1) {
            var i: usize = @as(usize, @intCast(idx));

            var val = input[i] - '0';
            if (val >= 0 and val < 10) {
                rhs = val;
                break;
            }
        }

        // std.debug.print("{d} {d}\n\n\n", .{ lhs, rhs });
        sum += lhs * 10 + rhs;
    }
    return sum;
}

fn part2(filepath: []const u8) !u64 {
    const file = try std.fs.cwd().openFile(filepath, std.fs.File.OpenFlags{ .mode = .read_only });
    defer file.close();

    var buffer: [100]u8 = undefined;

    var sum: u64 = 0;
    while (try nextLine(file.reader(), &buffer)) |input| {
        var lhs: u8 = undefined;
        for (input, 0..) |character, i| {
            var val = character - '0';
            if (val >= 0 and val < 10) {
                lhs = val;
                break;
            } else {
                if (i >= 2) {
                    // std.debug.print("{s}\n", .{input[i - 2 .. i + 1]});
                    if (eql(u8, input[i - 2 .. i + 1], "one")) {
                        lhs = 1;
                        break;
                    }
                    if (eql(u8, input[i - 2 .. i + 1], "two")) {
                        lhs = 2;
                        break;
                    }
                    if (eql(u8, input[i - 2 .. i + 1], "six")) {
                        lhs = 6;
                        break;
                    }
                }
                if (i >= 3) {
                    // std.debug.print("{s}\n", .{input[i - 3 .. i + 1]});
                    if (eql(u8, input[i - 3 .. i + 1], "four")) {
                        lhs = 4;
                        break;
                    }
                    if (eql(u8, input[i - 3 .. i + 1], "five")) {
                        lhs = 5;
                        break;
                    }
                    if (eql(u8, input[i - 3 .. i + 1], "nine")) {
                        lhs = 9;
                        break;
                    }
                }
                if (i >= 4) {
                    // std.debug.print("{s}\n", .{input[i - 4 .. i + 1]});
                    if (eql(u8, input[i - 4 .. i + 1], "three")) {
                        lhs = 3;
                        break;
                    }
                    if (eql(u8, input[i - 4 .. i + 1], "seven")) {
                        lhs = 7;
                        break;
                    }
                    if (eql(u8, input[i - 4 .. i + 1], "eight")) {
                        lhs = 8;
                        break;
                    }
                }
            }
            // std.debug.print("\n", .{});
        }

        // how do I loop backwards sensibly, surely all this casting isn't right
        var rhs: u8 = 0;
        var end: usize = input.len - 1;
        var idx: i8 = @as(i8, @intCast(end));

        while (idx >= 0) : (idx -= 1) {
            var i: u8 = @as(u8, @intCast(idx));
            var val = input[@as(usize, @intCast(i))] - '0';
            if (val >= 0 and val < 10) {
                rhs = val;
                // std.debug.print("Found via num {d}\n", .{rhs});
                break;
            } else {
                if (end - i >= 2) {
                    // std.debug.print("{s}\n", .{input[i .. i + 3]});
                    if (eql(u8, input[i .. i + 3], "one")) {
                        rhs = 1;
                        break;
                    }
                    if (eql(u8, input[i .. i + 3], "two")) {
                        rhs = 2;
                        break;
                    }
                    if (eql(u8, input[i .. i + 3], "six")) {
                        rhs = 6;
                        break;
                    }
                }
                if (end - i >= 3) {
                    // std.debug.print("{s}\n", .{input[i .. i + 4]});
                    if (eql(u8, input[i .. i + 4], "four")) {
                        rhs = 4;
                        break;
                    }
                    if (eql(u8, input[i .. i + 4], "five")) {
                        rhs = 5;
                        break;
                    }
                    if (eql(u8, input[i .. i + 4], "nine")) {
                        rhs = 9;
                        break;
                    }
                }
                if (end - i >= 4) {
                    // std.debug.print("{s}\n", .{input[i .. i + 5]});
                    if (eql(u8, input[i .. i + 5], "three")) {
                        rhs = 3;
                        break;
                    }
                    if (eql(u8, input[i .. i + 5], "seven")) {
                        rhs = 7;
                        break;
                    }
                    if (eql(u8, input[i .. i + 5], "eight")) {
                        rhs = 8;
                        break;
                    }
                }
            }
            // std.debug.print("\n", .{});
        }

        // std.debug.print("{d} {d}\n", .{lhs + '0', rhs + '0'});

        sum += lhs * 10 + rhs;
    }
    return sum;
}

// Adapted from ZigLearn
fn nextLine(reader: anytype, buffer: []u8) !?[]const u8 {
    var line = (try reader.readUntilDelimiterOrEof(
        buffer,
        '\n',
    )) orelse return null;

    // trim annoying windows-only carriage return character
    if (@import("builtin").os.tag == .windows) {
        return std.mem.trimRight(u8, line, "\r");
    } else {
        return line;
    }
}
