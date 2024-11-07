#include "main.hpp"

namespace lox {
bool hadError = false;

static void report(int line, std::string where, std::string message) {
    std::cerr << "[" << line << "] Error " << where << ": " << message;
    hadError = true;
}

static void error(int line, std::string message) {
    report(line, "", message);
}
} //namespace lox
