#include "scanner.hpp"
#include "errors.hpp"

#include <cctype>

namespace lox {
std::map<TokenType, std::string> token_string = {
    {LEFT_PAREN, "LEFT_PAREN"}, {RIGHT_PAREN, "RIGHT_PAREN"},
    {LEFT_BRACE, "LEFT_BRACE"}, {RIGHT_BRACE, "RIGHT_BRACE"},
    {COMMA, "COMMA"}, {DOT, "DOT"},
    {MINUS, "MINUS"}, {PLUS, "PLUS"},
    {SEMICOLON, "SEMICOLON"}, {SLASH, "SLASH"},
    {STAR, "STAR"},

    {BANG, "BANG"}, {BANG_EQUAL, "BANG_EQUAL"},
    {EQUAL, "EQUAL"}, {EQUAL_EQUAL, "EQUAL_EQUAL"},
    {GREATER, "GREATER"}, {GREATER_EQUAL, "GREATER_EQUAL"},
    {LESS, "LESS"}, {LESS_EQUAL, "LESS_EQUAL"},

    {IDENTIFIER, "IDENTIFIER"}, {STRING, "STRING"},
    {NUMBER, "NUMBER"},

    {AND, "AND"}, {CLASS, "CLASS"}, {ELSE, "ELSE"},
    {FALSE, "FALSE"}, {FUN, "FUN"}, {FOR, "FOR"},
    {IF, "IF"}, {NIL, "NIL"}, {OR, "OR"},
    {PRINT, "PRINT"}, {RETURN, "RETURN"},
    {SUPER, "SUPER"}, {THIS, "THIS"},
    {TRUE, "TRUE"}, {VAR, "VAR"}, {WHILE, "WHILE"},

    {tk_EOF, "EOF"}
};

void Scanner::addToken(TokenType type, std::any literal) {
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

char Scanner::peek_next() {
    if (current_ + 1 >= source_.size()) return '\0';
    return source_.at(current_ + 1);
}

void Scanner::string() {
    while (peek() != '"' && !is_end()) {
      if (peek() == '\n') line_++;
      advance();
    }

    if (is_end()) {
      error(line_, "Unterminated string.");
      return;
    }

    advance(); // Close string

    std::string value = source_.substr(start_ + 1, current_ - start_ - 2); // Trim
    addToken(STRING, value);
}

void Scanner::number() {
    while (::isdigit(peek())) advance();
    if (peek() == '.' && ::isdigit(peek_next())) {
        advance();
        while (::isdigit(peek())) advance();
    }

    addToken(NUMBER, std::stod(source_.substr(start_, current_ - start_)));
}

void Scanner::identifier() {
    while (::isalpha(peek()) || ::isdigit(peek()) || peek() == '_') advance();
    addToken(IDENTIFIER);
}

void Scanner::scan_token() {
    char c = advance();
    switch (c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
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
        case '"': string(); break;
        default:
            if (::isdigit(c)) { number(); break; }
            if (::isalpha(c) || c == '_') { identifier(); break; }

            std::ostringstream ss;
            ss << "Unexpected character: " << c;
            error(line_, ss.str());
            break;
    }
}

std::vector<Token> Scanner::scan_tokens() {
    while (!is_end()) {
        start_ = current_;
        scan_token();
    }

    tokens_.emplace_back(Token{tk_EOF, "", nullptr, line_});
    return tokens_;
}

Scanner::Scanner(std::string source): source_(source) {}
} // namespace lox