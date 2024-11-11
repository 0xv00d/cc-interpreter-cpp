#include "ast.hpp"

#include <functional>

namespace lox {
class ParseError: public std::exception {
public:
    ParseError(char* message): message_(message) {}

    inline char* what() { return message_; }

private:
    char* message_;
};

class Parser {
public:
    Parser(std::vector<Token> tokens): tokens_(tokens) {}
    inline Expr* parse() {
        try { return expression(); }
        catch (ParseError error) { return nullptr; }
    }

private:
    std::vector<Token> tokens_;
    int current_{0};

    ParseError error(Token, std::string);
    void synchronize();

    inline Token     peek() { return tokens_.at(current_); }
    inline Token previous() { return tokens_.at(current_ - 1); }

    inline bool    is_end() { return peek().type == tk_EOF; }

    Token advance();
    bool    check(            TokenType );
    bool    match(std::vector<TokenType>);

    Token consume(TokenType, std::string);

    Expr* construct_binary(std::function<Expr*()>, std::vector<TokenType>);

           Expr*    primary();
           Expr*      unary();
    inline Expr*     factor() { return construct_binary(std::bind(&Parser::unary,      this), {SLASH, STAR}); }
    inline Expr*       term() { return construct_binary(std::bind(&Parser::factor,     this), {MINUS, PLUS}); }
    inline Expr* comparison() { return construct_binary(std::bind(&Parser::term,       this), {GREATER, GREATER_EQUAL, LESS, LESS_EQUAL}); }
    inline Expr*   equality() { return construct_binary(std::bind(&Parser::comparison, this), {BANG_EQUAL, EQUAL_EQUAL}); }
    inline Expr* expression() { return equality(); }
};
} // namespace lox
