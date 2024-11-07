#pragma once

#include <string>
#include <sstream>
#include <map>
#include <vector>

namespace lox {
enum TokenType {
     // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    tk_EOF
};

extern std::map<TokenType, std::string> token_string;

struct Token {
    const TokenType type;
    const std::string lexeme;
    const void* literal;
    const int line;

    std::string to_string() {
        std::ostringstream out;
        out << token_string[type] << " "
            << lexeme << " "
            << "null";
            //<< literal;
        return out.str();
    }
};

class Scanner {
public:
    Scanner(std::string);

    std::vector<Token> scan_tokens();
private:
    std::string source_;
    std::vector<Token> tokens_;

    int start_{0};
    int current_{0};
    int line_{1};

    void scan_token();

    inline void addToken(TokenType type) { addToken(type, nullptr); }
           void addToken(TokenType, void*);

    bool match(char);
    char peek();

    inline char advance() { return source_.at(current_++); }
    inline bool  is_end() { return current_ >= source_.size(); }
};
} // namespace lox