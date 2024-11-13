#pragma once

#include "scanner.hpp"
#include "errors.hpp"

#include <any>
#include <unordered_map>

namespace lox {
struct Environment {
    Environment(                      ): enclosing_(nullptr  ) {}
    Environment(Environment* enclosing): enclosing_(enclosing) {}

    Environment* enclosing_;

    void define(const std::string& variable, std::any value) {
        values_[variable] = value;
    }

    std::any get(Token name) {
        auto it = values_.find(name.lexeme);
        if (it == values_.end()) {
            if (enclosing_) return enclosing_->get(name);

            std::string err = "Undefined variable '" + name.lexeme + "'.";
            throw RuntimeError(name, err.data());
        }
        return it->second;
    }

    void assign(Token name, std::any value) {
        auto it = values_.find(name.lexeme);
        if (it == values_.end()) {
            if (enclosing_) return enclosing_->assign(name, value);

            std::string err = "Undefined variable '" + name.lexeme + "'.";
            throw RuntimeError(name, err.data());
        }
        values_[name.lexeme] = value;
    }

private:
    std::unordered_map<std::string, std::any> values_;
};
} // namespace lox
