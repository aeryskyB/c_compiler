const std = @import("std");
const macro = @import("macro.zig");

const TYPE_LEN = 20;
const VALUE_LEN = 30;

const Token = struct {
    type: [TYPE_LEN]u8,
    value: [VALUE_LEN]u8,
};

const NULL: [1]u8 = [_]u8{0};
const NULL_TYPE: [TYPE_LEN]u8 = [_]u8{0} ** TYPE_LEN;
const NULL_VALUE: [VALUE_LEN]u8 = [_]u8{0} ** VALUE_LEN;
var input: std.fs.File = undefined;
var buf: [1]u8 = undefined;
var b: [1]u8 = undefined;

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
    var tok = Token{ .type = [_]u8{0} ** TYPE_LEN, .value = [_]u8{0} ** VALUE_LEN };
    if (try input.read(&buf) == 0) return;
    while (true) {
        // std.debug.print("{c} ", .{buf[0]});
        buf[0] = try lex_head(&tok);
        // if (buf[0] == 254) break;   // EOF during single-line comment
        if (std.mem.indexOfScalar(u8, &tok.value, 0) != 0) {
            try stdout.print("({s}, {s})\n", .{ tok.type, tok.value });
            std.mem.copyForwards(u8, &(tok.type), &NULL_TYPE);
            std.mem.copyForwards(u8, &(tok.value), &NULL_VALUE);
        } else if (std.mem.indexOfScalar(u8, &tok.type, 0) != 0) {
            try stdout.print("({s})\n", .{tok.type});
            std.mem.copyForwards(u8, &(tok.type), &NULL_TYPE);
        }
        // over-read
        if (buf[0] != 255) {
            continue;
        }
        if (buf[0] == 0) break; // EOF
        if (try input.read(&buf) == 0) break; // EOF
    }
}

pub fn lex_head(tok: *Token) !u8 {
    while (is_whitespace(buf[0])) {
        if (try input.read(&buf) == 0) return 0; // EOF
    }
    if (buf[0] == '+') {
        return lex_add(tok);
        // std.mem.copyForwards(u8, &(tok.type), macro.PLUS_TOK);
        // std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '-') {
        return lex_minus(tok);
        // std.mem.copyForwards(u8, &(tok.type), macro.MINUS_TOK);
        // std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '*') {
        return lex_mult(tok);
        // std.mem.copyForwards(u8, &(tok.type), macro.MULT_TOK);
        // std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '/') {
        return lex_div(tok);
        // std.mem.copyForwards(u8, &(tok.type), macro.DIV_TOK);
        // std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '%') {
        std.mem.copyForwards(u8, &(tok.type), macro.MOD_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '^') {
        std.mem.copyForwards(u8, &(tok.type), macro.XOR_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == ';') {
        std.mem.copyForwards(u8, &(tok.type), macro.SEMICOLON_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == ',') {
        std.mem.copyForwards(u8, &(tok.type), macro.COMMA_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '{') {
        std.mem.copyForwards(u8, &(tok.type), macro.LBRACE_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '}') {
        std.mem.copyForwards(u8, &(tok.type), macro.RBRACE_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == '(') {
        std.mem.copyForwards(u8, &(tok.type), macro.LPAREN_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (buf[0] == ')') {
        std.mem.copyForwards(u8, &(tok.type), macro.RPAREN_TOK);
        std.mem.copyForwards(u8, &(tok.value), &NULL);
    } else if (is_namechar(buf[0])) {
        return lex_sp(tok, buf[0]);
    } else if (is_relational_logical_bitwise(buf[0])) {
        return lex_bin(tok, buf[0]);
    } else if (is_num(buf[0])) {
        return lex_num(tok, buf[0]);
    }

    return 255;
}

pub fn lex_sp(tok: *Token, c: u8) !u8 {
    var idx: u8 = 0;
    var c_: usize = undefined;
    b[0] = c;
    while (true) {
        tok.value[idx] = b[0];
        c_ = try input.read(&b);
        idx += 1;
        if (!is_namechar(b[0]) and !is_num(b[0])) break;
    }

    if (std.mem.eql(u8, tok.value[0.."if".len], "if")) {
        std.mem.copyForwards(u8, &(tok.type), macro.IF_TOK);
    } else if (std.mem.eql(u8, tok.value[0.."else".len], "else")) {
        std.mem.copyForwards(u8, &(tok.type), macro.ELSE_TOK);
    } else if (std.mem.eql(u8, tok.value[0.."for".len], "for")) {
        std.mem.copyForwards(u8, &(tok.type), macro.FOR_TOK);
    } else if (std.mem.eql(u8, tok.value[0.."while".len], "while")) {
        std.mem.copyForwards(u8, &(tok.type), macro.WHILE_TOK);
    } else if (std.mem.eql(u8, tok.value[0.."int".len], "int")) {
        std.mem.copyForwards(u8, &(tok.type), macro.INTK_TOK);
    } else if (std.mem.eql(u8, tok.value[0.."float".len], "float")) {
        std.mem.copyForwards(u8, &(tok.type), macro.FLOATK_TOK);
    } else if (std.mem.eql(u8, tok.value[0.."char".len], "char")) {
        std.mem.copyForwards(u8, &(tok.type), macro.CHARK_TOK);
    } else {
        std.mem.copyForwards(u8, &(tok.type), macro.IDNT_TOK);
        tok.value[idx] = 0;
        if (c_ == 0) return 0;
        return b[0];
    }
    std.mem.copyForwards(u8, &(tok.value), &NULL_VALUE);
    if (c_ == 0) return 0;
    return b[0];
}

pub fn lex_bin(tok: *Token, c: u8) !u8 {
    b[0] = c;
    var c_: usize = undefined;
    std.mem.copyForwards(u8, &(tok.value), &NULL);
    if (c == '=') {
        c_ = try input.read(&b);
        if (b[0] == '=') {
            std.mem.copyForwards(u8, &(tok.type), macro.EQ_TOK);
        } else {
            std.mem.copyForwards(u8, &(tok.type), macro.ASSIGN_TOK);
            if (c_ == 0) return 0;
            return b[0];
        }
    } else if (c == '<') {
        c_ = try input.read(&b);
        if (b[0] == '=') {
            std.mem.copyForwards(u8, &(tok.type), macro.LE_TOK);
        } else if (b[0] == '<') {
            std.mem.copyForwards(u8, &(tok.type), macro.LSHIFT_TOK);
        } else {
            std.mem.copyForwards(u8, &(tok.type), macro.LT_TOK);
            if (c_ == 0) return 0;
            return b[0];
        }
    } else if (c == '>') {
        c_ = try input.read(&b);
        if (b[0] == '=') {
            std.mem.copyForwards(u8, &(tok.type), macro.GE_TOK);
        } else if (b[0] == '>') {
            std.mem.copyForwards(u8, &(tok.type), macro.RSHIFT_TOK);
        } else {
            std.mem.copyForwards(u8, &(tok.type), macro.GT_TOK);
            if (c_ == 0) return 0;
            return b[0];
        }
    } else if (c == '!') {
        c_ = try input.read(&b);
        if (b[0] == '=') {
            std.mem.copyForwards(u8, &(tok.type), macro.NEQ_TOK);
        } else {
            std.mem.copyForwards(u8, &(tok.type), macro.NOT_TOK);
            if (c_ == 0) return 0;
            return b[0];
        }
    } else if (c == '&') {
        c_ = try input.read(&b);
        if (b[0] == '&') {
            std.mem.copyForwards(u8, &(tok.type), macro.ANDL_TOK);
        } else {
            std.mem.copyForwards(u8, &(tok.type), macro.ANDB_TOK);
            if (c_ == 0) return 0;
            return b[0];
        }
    } else if (c == '|') {
        c_ = try input.read(&b);
        if (b[0] == '|') {
            std.mem.copyForwards(u8, &(tok.type), macro.ORL_TOK);
        } else {
            std.mem.copyForwards(u8, &(tok.type), macro.ORB_TOK);
            if (c_ == 0) return 0;
            return b[0];
        }
    }
    std.mem.copyForwards(u8, &(tok.value), &NULL);
    return 255;
}

pub fn lex_num(tok: *Token, c: u8) !u8 {
    var idx: u8 = 0;
    var c_: usize = undefined;
    b[0] = c;
    std.mem.copyForwards(u8, &(tok.type), macro.INT_TOK);
    while (true) {
        tok.value[idx] = b[0];
        idx += 1;
        c_ = try input.read(&b);
        if (!is_num(b[0])) break;
    }
    tok.value[idx] = 0;
    if (c_ == 0) return 0;
    return b[0];
}

pub fn lex_add(tok: *Token) !u8 {
    const c_: usize = try input.read(&b);
    if (b[0] == '+') {
        std.mem.copyForwards(u8, &(tok.type), macro.INCR_TOK);
    } else {
        std.mem.copyForwards(u8, &(tok.type), macro.PLUS_TOK);
        if (c_ == 0) return 0;
        return b[0];
    }
    return 255;
}

pub fn lex_minus(tok: *Token) !u8 {
    const c_: usize = try input.read(&b);
    if (b[0] == '-') {
        std.mem.copyForwards(u8, &(tok.type), macro.DECR_TOK);
    } else {
        std.mem.copyForwards(u8, &(tok.type), macro.MINUS_TOK);
        if (c_ == 0) return 0;
        return b[0];
    }
    return 255;
}

pub fn lex_div(tok: *Token) !u8 {
    var c_: usize = try input.read(&b);
    if (b[0] == '/') {
        while (true) {
            c_ = try input.read(&b);
            if (b[0] == '\n' or c_ == 0) break;
        }
        if (c_ == 0) return 0;
    } else if (b[0] == '*') {
        std.mem.copyForwards(u8, &(tok.type), macro.LCOMMENT_TOK);
    } else {
        std.mem.copyForwards(u8, &(tok.type), macro.DIV_TOK);
        if (c_ == 0) return 0;
        return b[0];
    }
    return 255;
}

pub fn lex_mult(tok: *Token) !u8 {
    const c_: usize = try input.read(&b);
    if (b[0] == '/') {
        std.mem.copyForwards(u8, &(tok.type), macro.RCOMMENT_TOK);
    } else {
        std.mem.copyForwards(u8, &(tok.type), macro.MULT_TOK);
        if (c_ == 0) return 0;
        return b[0];
    }
    return 255;
}

pub fn is_whitespace(c: u8) bool {
    return c == ' ' or c == '\t' or c == '\n' or c == '\r';
}

pub fn is_namechar(c: u8) bool {
    return (c >= 'A' and c <= 'Z') or (c >= 'a' and c <= 'z') or c == '_';
}

pub fn is_num(c: u8) bool {
    return c >= '0' and c <= '9';
}

pub fn is_relational_logical_bitwise(c: u8) bool {
    return c == '=' or c == '<' or c == '>' or c == '&' or c == '|' or c == '!';
}
