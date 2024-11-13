#pragma once

#include "scanner.hpp"

#include <typeinfo>
#include <iostream>

namespace lox {
struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;

template <typename T>
class ExprVisitor {
public:
    virtual T visit_assign_expr(Assign*) = 0;
    virtual T visit_binary_expr(Binary*) = 0;
    virtual T visit_grouping_expr(Grouping*) = 0;
    virtual T visit_literal_expr(Literal*) = 0;
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

struct Expression;
struct Print;
struct Var;
struct Block;

template <typename T>
class StmtVisitor {
public:
    virtual T visit_expression_stmt(Expression*) = 0;
    virtual T visit_print_stmt(Print*) = 0;
    virtual T visit_var_stmt(Var*) = 0;
    virtual T visit_block_stmt(Block*) = 0;
};

struct Stmt {
    virtual void accept(StmtVisitor<void>*) = 0;
};
struct Expression: public Stmt {
    Expression(Expr* expr): expr_(expr) {}

    Expr* expr_;

    void accept(StmtVisitor<void>* visitor) override { visitor->visit_expression_stmt(this); }
};
struct Print: public Stmt {
    Print(Expr* expr): expr_(expr) {}

    Expr* expr_;

    void accept(StmtVisitor<void>* visitor) override { visitor->visit_print_stmt(this); }
};
struct Var: public Stmt {
    Var(Token name, Expr* initializer): name_(name), initializer_(initializer) {}

    Token name_;
    Expr* initializer_;

    void accept(StmtVisitor<void>* visitor) override { visitor->visit_var_stmt(this); }
};
struct Block: public Stmt {
    Block(std::vector<Stmt*> statements): statements_(statements) {}

    std::vector<Stmt*> statements_;

    void accept(StmtVisitor<void>* visitor) override { visitor->visit_block_stmt(this); }
};

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
