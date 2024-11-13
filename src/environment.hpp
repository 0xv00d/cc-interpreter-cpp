#pragma once

#include "scanner.hpp"
#include "errors.hpp"

#include <any>
#include <unordered_map>

namespace lox {
struct Environment {
    void define(const std::string& variable, std::any value) {
        values_[variable] = value;
    }

    std::any get(Token name) {
        auto it = values_.find(name.lexeme);
        if (it == values_.end()) {
            std::string err = "Undefined variable '" + name.lexeme + "'.";
            throw RuntimeError(name, err.data());
        }
        return it->second;
    }

    void assign(Token name, std::any value) {
        auto it = values_.find(name.lexeme);
        if (it == values_.end()) {
            std::string err = "Undefined variable '" + name.lexeme + "'.";
            throw RuntimeError(name, err.data());
        }
        values_[name.lexeme] = value;
    }

private:
    std::unordered_map<std::string, std::any> values_;
};
} // namespace lox
