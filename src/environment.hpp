#pragma once

#include "scanner.hpp"
#include "errors.hpp"

#include <any>
#include <unordered_set>

namespace lox {
struct Environment {
    std::unordered_map<std::string, std::any> values;

    void define(const std::string& variable, std::any value) {
        values[variable] = value;
    }

    std::any get(Token name) {
        auto it = values.find(name.lexeme);
        if (it == values.end()) {
            std::string err = "Undefined variable '" + name.lexeme + "'.";
            throw RuntimeError(name, err.data());
        }
        return it->second;
    }
};
} // namespace lox
