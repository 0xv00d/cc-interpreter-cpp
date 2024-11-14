#pragma once

#include "scanner.hpp"

namespace lox {
struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Logical;
struct Unary;
struct Variable;

template <typename T>
class ExprVisitor {
public:
    virtual T visit_assign_expr(Assign*) = 0;
    virtual T visit_binary_expr(Binary*) = 0;
    virtual T visit_grouping_expr(Grouping*) = 0;
    virtual T visit_literal_expr(Literal*) = 0;
    virtual T visit_logical_expr(Logical*) = 0;
    virtual T visit_unary_expr(Unary*) = 0;
    virtual T visit_variable_expr(Variable*) = 0;
};

struct Expr {
    virtual std::string accept(ExprVisitor<std::string>*) = 0;
    virtual std::any    accept(ExprVisitor<std::any   >*) = 0;
};
struct Assign: public Expr {
    Assign(Token name, Expr* value): name_(name), value_(value) {}

    Token name_;
    Expr* value_;

    std::string accept(ExprVisitor<std::string>* visitor) override { return visitor->visit_assign_expr(this); }
    std::any    accept(ExprVisitor<std::any   >* visitor) override { return visitor->visit_assign_expr(this); }
};
struct Binary: public Expr {
    Binary(Expr* left, Token op, Expr* right): left_(left), op_(op), right_(right) {}

    Expr* left_;
    Token op_;
    Expr* right_;

    std::string accept(ExprVisitor<std::string>* visitor) override { return visitor->visit_binary_expr(this); }
    std::any    accept(ExprVisitor<std::any   >* visitor) override { return visitor->visit_binary_expr(this); }
};
struct Grouping: public Expr {
    Grouping(Expr* expr): expr_(expr) {}

    Expr* expr_;

    std::string accept(ExprVisitor<std::string>* visitor) override { return visitor->visit_grouping_expr(this); }
    std::any    accept(ExprVisitor<std::any   >* visitor) override { return visitor->visit_grouping_expr(this); }
};
struct Literal: public Expr {
    Literal(std::any value): value_(value) {}

    std::any value_;

    std::string accept(ExprVisitor<std::string>* visitor) override { return visitor->visit_literal_expr(this); }
    std::any    accept(ExprVisitor<std::any   >* visitor) override { return visitor->visit_literal_expr(this); }
};
struct Logical: public Expr {
    Logical(Expr* left, Token op, Expr* right): left_(left), op_(op), right_(right) {}

    Expr* left_;
    Token op_;
    Expr* right_;

    std::string accept(ExprVisitor<std::string>* visitor) override { return visitor->visit_logical_expr(this); }
    std::any    accept(ExprVisitor<std::any   >* visitor) override { return visitor->visit_logical_expr(this); }
};
struct Unary: public Expr {
    Unary(Token op, Expr* right): op_(op), right_(right) {}

    Token op_;
    Expr* right_;

    std::string accept(ExprVisitor<std::string>* visitor) override { return visitor->visit_unary_expr(this); }
    std::any    accept(ExprVisitor<std::any   >* visitor) override { return visitor->visit_unary_expr(this); }
};
struct Variable: public Expr {
    Variable(Token name): name_(name) {}

    Token name_;

    std::string accept(ExprVisitor<std::string>* visitor) override { return visitor->visit_variable_expr(this); }
    std::any    accept(ExprVisitor<std::any   >* visitor) override { return visitor->visit_variable_expr(this); }
};
} // namespace lox