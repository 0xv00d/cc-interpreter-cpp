#include "scanner.hpp"

#include <typeinfo>
#include <iostream>

namespace lox {
struct Binary;
struct Grouping;
struct Literal;
struct Unary;

class Visitor {
public:
    virtual std::string visitBinaryExpr(Binary*) = 0;
    virtual std::string visitGroupingExpr(Grouping*) = 0;
    virtual std::string visitLiteralExpr(Literal*) = 0;
    virtual std::string visitUnaryExpr(Unary*) = 0;
};

struct Expr {
    virtual std::string accept(Visitor*) = 0;
};
struct Binary: public Expr {
    Binary(Expr* left, Token op, Expr* right): left_(left), op_(op), right_(right) {}

    Expr* left_;
    Token op_;
    Expr* right_;

    std::string accept(Visitor* visitor) override {  return visitor->visitBinaryExpr(this); }
};
struct Grouping: public Expr {
    Grouping(Expr* expr): expr_(expr) {}

    Expr* expr_;

    std::string accept(Visitor* visitor) override { return visitor->visitGroupingExpr(this); }
};
struct Literal: public Expr {
    Literal(std::any value): value_(value) {}

    std::any value_;

    std::string accept(Visitor* visitor) override { return visitor->visitLiteralExpr(this); }
};
struct Unary: public Expr {
    Unary(Token op, Expr* right): op_(op), right_(right) {}

    Token op_;
    Expr* right_;

    std::string accept(Visitor* visitor) override { return visitor->visitUnaryExpr(this); }
};

class ASTPrinter: public Visitor {
public:
    std::string print(Expr* expr) { return expr->accept(this); }

    std::string visitBinaryExpr(Binary* expr) override {
        return parenthesize(expr->op_.lexeme, {expr->left_, expr->right_});
    }
  
    std::string visitGroupingExpr(Grouping* expr) override {
        return parenthesize("group", {expr->expr_});
    }
  
    std::string visitLiteralExpr(Literal* expr) override {
        if (IS_TYPE(expr->value_, std::nullptr_t)) return "nil";
        if (IS_TYPE(expr->value_, std::string)) return std::any_cast<std::string>(expr->value_);
        if (IS_TYPE(expr->value_, double)) return trimmed_double(std::any_cast<double>(expr->value_));
        if (IS_TYPE(expr->value_, bool)) return std::any_cast<bool>(expr->value_) ? "true" : "false";
        return "?";
    }
  
    std::string visitUnaryExpr(Unary* expr) override {
        return parenthesize(expr->op_.lexeme, {expr->right_});
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
