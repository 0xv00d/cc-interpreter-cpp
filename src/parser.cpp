#include "parser.hpp"
#include "errors.hpp"

namespace lox {
ParseError Parser::error(Token token, std::string message) {
    err::error(token, message);
    return ParseError(message.data());
}

void Parser::synchronize() {
    advance();

    while (!is_end()) {
        if (previous().type == SEMICOLON) return;

        switch (peek().type) {
            case CLASS: case    FUN: case   VAR:
            case   FOR: case     IF: case WHILE:
            case PRINT: case RETURN:
                return;
        }
        advance();
    }
}

Token Parser::advance() {
    if (!is_end()) current_++;
    return previous();
}

bool Parser::check(TokenType type) {
    if (is_end()) return false;
    return peek().type == type;
}

bool Parser::match(std::vector<TokenType> types) {
    for (TokenType type: types) if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, std::string message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

Expr* Parser::primary() {
    if (match({FALSE})) return new Literal(false);
    if (match({ TRUE})) return new Literal(true);
    if (match({  NIL})) return new Literal(nullptr);

    if (match({NUMBER, STRING})) {
        return new Literal(previous().literal);
    }

    if (match({LEFT_PAREN})) {
        Expr* expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return new Grouping(expr);
    }

    throw error(peek(), "Expect expression.");
}

Expr* Parser::unary() {
    if (match({BANG, MINUS})) {
        Token op = previous();
        Expr* right = unary();
        return new Unary(op, right);
    }

    return primary();
}

Expr* Parser::construct_binary(std::function<Expr*()> func, std::vector<TokenType> tokens) {
    Expr* expr = func();

    while (match(tokens)) {
        Token op = previous();
        Expr* right = func();
        expr = new Binary(expr, op, right);
    }

    return expr;
}
} // namespace lox
