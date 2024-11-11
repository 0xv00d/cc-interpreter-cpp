#include "main.hpp"

namespace lox::err {
extern bool had_error;

static void report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error: " << where << message << std::endl;
    had_error = true;
}

static void error(int line, std::string message) {
    report(line, "", message);
}

static void error(Token token, std::string message) {
    if (token.type == tk_EOF) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}
} // namespace lox::err
