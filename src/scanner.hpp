#pragma once

#include <any>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#define IS_TYPE(arg, target) arg.type().name() == typeid(target).name()

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
extern std::map<std::string, TokenType> keywords;

extern std::string trimmed_double(double);

struct Token {
private:
    std::string from_literal() {
        if (IS_TYPE(literal, std::string)) return std::any_cast<std::string>(literal);
        if (IS_TYPE(literal, double)) return trimmed_double(std::any_cast<double>(literal));
        if (IS_TYPE(literal, std::nullptr_t)) return "null";
        return "?";
    }

public:
    const TokenType type;
    const std::string lexeme;
    const std::any literal;
    const int line;

    std::string to_string() {
        std::ostringstream out;
        out << token_string[type] << " "
            << lexeme << " "
            << from_literal();
        return out.str();
    }
};

class Scanner {
public:
    Scanner(std::string source): source_(source) {}

    std::vector<Token> scan_tokens();
private:
    std::string source_;
    std::vector<Token> tokens_;

    int start_{0};
    int current_{0};
    int line_{1};

    void scan_token();

           void addToken(TokenType, std::any);
    inline void addToken(TokenType type) { addToken(type, NULL); }

    bool match(char);
    char peek();
    char peek_next();

    void     string();
    void     number();
    void identifier();

    inline char advance() { return source_.at(current_++); }
    inline bool  is_end() { return current_ >= source_.size(); }
};
} // namespace lox
