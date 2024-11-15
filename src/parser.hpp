#pragma once

#include "ast/statements.hpp"

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
    std::vector<Stmt*> parse() {
        try {
            std::vector<Stmt*> statements;
            while (!is_end()) {
                statements.emplace_back(declaration());
            }
            return statements;
        } catch (ParseError e) {
            return {};
        }
    }
    inline Expr* parse(int i) {
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
           Expr* assignment();
           Expr*    or_expr();
           Expr*   and_expr();
    inline Expr* expression() { return assignment(); }

    std::vector<Stmt*> block();

    Stmt* expression_statement();
    Stmt*      print_statement();
    Stmt*        for_statement();
    Stmt*         if_statement();
    Stmt*      while_statement();
    Stmt*            statement() {
        if (match({       FOR})) return   for_statement();
        if (match({        IF})) return    if_statement();
        if (match({     PRINT})) return print_statement();
        if (match({     WHILE})) return while_statement();
        if (match({LEFT_BRACE})) return new Block(block());
        return expression_statement();
    }
    Stmt*      declaration();
    Stmt*  var_declaration();
};
} // namespace lox
