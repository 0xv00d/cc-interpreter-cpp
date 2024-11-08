#pragma once

#include <any>
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
private:
    std::string trimmed_double(double value) {
        std::string vs = std::to_string(value);
        vs.erase(vs.find_last_not_of('0') + 1, std::string::npos);
        if (vs.back() == '.') vs.push_back('0');
        return vs;
    }

    std::string from_literal() {
        if (literal.type().name() == typeid(std::string).name()) return std::any_cast<std::string>(literal);
        if (literal.type().name() == typeid(double).name()) return trimmed_double(std::any_cast<double>(literal));
        if (literal.type().name() == typeid(std::nullptr_t).name()) return "null";
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
           void addToken(TokenType, std::any);

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
