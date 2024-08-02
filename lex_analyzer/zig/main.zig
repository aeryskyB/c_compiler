const std = @import("std");
const macro = @import("macro.zig");

const TYPE_LEN = 20;
const VALUE_LEN = 30;

const Token = struct {
    type: [TYPE_LEN]u8,
    value: [VALUE_LEN]u8,
};

const NULL: [1]u8 = [_]u8{0};
var input: std.fs.File = undefined;

pub fn main() !void {
    // argument check
    var argiter = std.process.args();
    _ = argiter.next();
    const input_file = argiter.next();
    const tst = argiter.next();
    std.debug.assert(input_file != null and tst == null);

    // std.debug.print("{?s}\n", .{input_file});
    input = try std.fs.cwd().openFile(input_file.?, .{});
    defer input.close();

    const stdout = std.io.getStdOut().writer();
    // try stdout.print("{s}\n", .{macro.PLUS_TOK});
    // std.debug.print("{s}\n", .{macro.PLUS_TOK});

    // var reader = input.reader();
    var buffer: [1]u8 = undefined;
    var tok = Token{ .type = [_]u8{0} ** TYPE_LEN, .value = [_]u8{0} ** VALUE_LEN };
    while (try input.read(&buffer) != 0) {
        if (buffer[0] == 0) break; // EOF
        // std.debug.print("{c}", .{buffer[0]});
        lex_head(&tok, &buffer);
        if (std.mem.indexOfScalar(u8, &tok.value, 0) != 0) {
            try stdout.print("({s}, {s}) -> {d}\n", .{ tok.type, tok.value, tok.value.len });
        } else if (std.mem.indexOfScalar(u8, &tok.type, 0) != 0) {
            try stdout.print("({s})\n", .{tok.type});
        }
    }
}

pub fn lex_head(tok_ptr: *Token, buf: []u8) void {
    while (is_whitespace(buf[0])) {
        input.read(&buf);
    }
    if (buf[0] == '+') {
        std.mem.copyForwards(u8, &(tok_ptr.type), macro.PLUS_TOK);
        std.mem.copyForwards(u8, &(tok_ptr.value), &NULL);
    } else if (buf[0] == '-') {
        std.mem.copyForwards(u8, &(tok_ptr.type), macro.MINUS_TOK);
        std.mem.copyForwards(u8, &(tok_ptr.value), &NULL);
    } else if (buf[0] == '*') {
        std.mem.copyForwards(u8, &(tok_ptr.type), macro.MULT_TOK);
        std.mem.copyForwards(u8, &(tok_ptr.value), &NULL);
    } else if (buf[0] == '/') {
        std.mem.copyForwards(u8, &(tok_ptr.type), macro.DIV_TOK);
        std.mem.copyForwards(u8, &(tok_ptr.value), &NULL);
    }
}

pub fn is_whitespace(c: u8) bool {
    return c == ' ' or c == '\t' or c == '\n' or c == '\r';
}
