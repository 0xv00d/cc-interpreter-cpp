#pragma once

#include "expressions.hpp"
#include <iostream>

namespace lox {
class ASTPrinter: public ExprVisitor<std::string> {
public:
    inline std::string print(Expr* expr) { return expr->accept(this); }

    inline std::string visit_binary_expr(Binary* expr) override {
        return parenthesize(expr->op_.lexeme, {expr->left_, expr->right_});
    }
    inline std::string visit_grouping_expr(Grouping* expr) override {
        return parenthesize("group", {expr->expr_});
    }
  
    std::string visit_literal_expr(Literal* expr) override {
        if (IS_TYPE(expr->value_, std::nullptr_t)) return "nil";
        if (IS_TYPE(expr->value_, std::string)) return std::any_cast<std::string>(expr->value_);
        if (IS_TYPE(expr->value_, double)) return trimmed_double(std::any_cast<double>(expr->value_));
        if (IS_TYPE(expr->value_, bool)) return std::any_cast<bool>(expr->value_) ? "true" : "false";
        return "?";
    }
    
    inline std::string visit_logical_expr(Logical* expr) override {
        return parenthesize(expr->op_.lexeme, {expr->left_, expr->right_});
    }
  
    inline std::string visit_unary_expr(Unary* expr) override {
        return parenthesize(expr->op_.lexeme, {expr->right_});
    }
    inline std::string visit_variable_expr(Variable* expr) override {
        return expr->name_.lexeme;
    }
    inline std::string visit_assign_expr(Assign* expr) override {
        return parenthesize(expr->name_.lexeme, {expr->value_});
    }

private:
    std::string parenthesize(std::string name, std::vector<Expr*> exprs) {
        std::ostringstream out;

        out << "(" << name;
        for (auto expr : exprs) out << " " << expr->accept(this);
        out << ")";

        return out.str();
    }
};
} // namespace lox
