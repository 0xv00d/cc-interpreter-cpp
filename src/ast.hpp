#include "scanner.hpp"

#include <iostream>

namespace lox {
template <class T>
class Visitor;

struct Expr {
    template <typename T>
    T accept(Visitor<T>*) { return T(); };
};
struct Binary: public Expr {
    Expr left;
    Token op;
    Expr right;

    template <typename T>
    T accept(Visitor<T>* visitor) {  return visitor->visitBinaryExpr(this); }
};
struct Grouping: public Expr {
    Expr expression;

    template <typename T>
    T accept(Visitor<T>* visitor) { return visitor->visitGroupingExpr(this); }
};
struct Literal: public Expr {
    std::any value;

    template <typename T>
    T accept(Visitor<T>* visitor) { return visitor->visitLiteralExpr(this); }
};
struct Unary: public Expr {
    Token op;
    Expr right;

    template <typename T>
    T accept(Visitor<T>* visitor) { return visitor->visitUnaryExpr(this); }
};

template <class T>
class Visitor {
    virtual T visitBinaryExpr(Binary*) = 0;
    virtual T visitGroupingExpr(Grouping*) = 0;
    virtual T visitLiteralExpr(Literal*) = 0;
    virtual T visitUnaryExpr(Unary*) = 0;
};
class ASTPrinter: public Visitor<std::string> {
public:
    std::string print(Expr* expr) { return expr->accept(this); }

    std::string visitBinaryExpr(Binary* expr) {
        return parenthesize(expr->op.lexeme, {expr->left, expr->right});
    }
  
    std::string visitGroupingExpr(Grouping* expr) {
        return parenthesize("group", {expr->expression});
    }
  
    std::string visitLiteralExpr(Literal* expr) {
        if (IS_TYPE(expr->value, std::nullptr_t)) return "nil";
        if (IS_TYPE(expr->value, std::string)) return std::any_cast<std::string>(expr->value);
        if (IS_TYPE(expr->value, double)) return trimmed_double(std::any_cast<double>(expr->value));
        return "?";
    }
  
    std::string visitUnaryExpr(Unary* expr) {
        return parenthesize(expr->op.lexeme, {expr->right});
    }

private:
    std::string parenthesize(std::string name, std::vector<Expr> exprs) {
        std::ostringstream out;

        out << "(" << name;
        for (Expr expr : exprs) out << " " << expr.accept(this);
        out << ")";

        return out.str();
    }
};
} // namespace lox
