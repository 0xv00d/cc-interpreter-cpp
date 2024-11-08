#include "scanner.hpp"
#include "errors.hpp"

namespace lox {
std::map<TokenType, std::string> token_string = {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"}, {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"}, {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::COMMA, "COMMA"}, {TokenType::DOT, "DOT"},
    {TokenType::MINUS, "MINUS"}, {TokenType::PLUS, "PLUS"},
    {TokenType::SEMICOLON, "SEMICOLON"}, {TokenType::SLASH, "SLASH"},
    {TokenType::STAR, "STAR"},

    {TokenType::BANG, "BANG"}, {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL, "EQUAL"}, {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"}, {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"}, {TokenType::LESS_EQUAL, "LESS_EQUAL"},

    {TokenType::IDENTIFIER, "IDENTIFIER"}, {TokenType::STRING, "STRING"},
    {TokenType::NUMBER, "NUMBER"},

    {TokenType::AND, "AND"}, {TokenType::CLASS, "CLASS"}, {TokenType::ELSE, "ELSE"},
    {TokenType::FALSE, "FALSE"}, {TokenType::FUN, "FUN"}, {TokenType::FOR, "FOR"},
    {TokenType::IF, "IF"}, {TokenType::NIL, "NIL"}, {TokenType::OR, "OR"},
    {TokenType::PRINT, "PRINT"}, {TokenType::RETURN, "RETURN"},
    {TokenType::SUPER, "SUPER"}, {TokenType::THIS, "THIS"},
    {TokenType::TRUE, "TRUE"}, {TokenType::VAR, "VAR"}, {TokenType::WHILE, "WHILE"},

    {TokenType::tk_EOF, "EOF"}
};

void Scanner::addToken(TokenType type, void* literal) {
    std::string text = source_.substr(start_, current_ - start_);
    tokens_.emplace_back(Token{type, text, literal, line_});
}

bool Scanner::match(char expected) {
    if (is_end()) return false;
    if (source_.at(current_) != expected) return false;

    current_++;
    return true;
}

char Scanner::peek() {
    if (is_end()) return '\0';
    return source_.at(current_);
}

void Scanner::scan_token() {
    char c = advance();
    switch (c) {
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '-': addToken(TokenType::MINUS); break;
        case '+': addToken(TokenType::PLUS); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case '*': addToken(TokenType::STAR); break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) while (peek() != '\n' && !is_end()) advance();
            else addToken(SLASH);
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line_++;
            break;
        default:
            error(line_, "Unexpected character: " + c);
            break;
    }
}

std::vector<Token> Scanner::scan_tokens() {
    while (!is_end()) {
        start_ = current_;
        scan_token();
    }

    tokens_.emplace_back(Token{TokenType::tk_EOF, "", nullptr, line_});
    return tokens_;
}

Scanner::Scanner(std::string source): source_(source) {}
} // namespace lox