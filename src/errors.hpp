#include "main.hpp"

namespace lox {
extern bool had_error;

static void report(int line, std::string where, std::string message) {
    std::cerr << "[line " << line << "] Error: " << where << message << std::endl;
    had_error = true;
}

static void error(int line, std::string message) {
    report(line, "", message);
}
} //namespace lox
