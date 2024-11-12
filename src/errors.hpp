#include "main.hpp"

namespace lox {
class RuntimeError: public std::exception {
public:
    RuntimeError(Token token, char* message): token_(token), message_(message) {}

    inline char* what() { return message_; }
    Token token_;

private:
    char* message_;
};

namespace err {
extern bool had_error;
extern bool hadRuntimeError;

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

static void runtimeError(RuntimeError error) {
    std::cerr << error.what() << "\n[line " << error.token_.line << "]" << std::endl;
    hadRuntimeError = true;
  }
} // namespace lox::err
} // namespace lox
