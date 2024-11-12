#include "ast.hpp"

namespace lox {
class Interpreter: public Visitor<std::any> {
public:
    void interpret(Expr* expr);

    std::any visitBinaryExpr(Binary* expr) override {
        std::any  left = evaluate(expr-> left_);
        std::any right = evaluate(expr->right_);

        if (expr->op_.type == MINUS) return std::any_cast<double>(left) - std::any_cast<double>(right);
        if (expr->op_.type == SLASH) return std::any_cast<double>(left) / std::any_cast<double>(right);
        if (expr->op_.type ==  STAR) return std::any_cast<double>(left) * std::any_cast<double>(right);
        if (expr->op_.type ==  PLUS) {
            if ((IS_TYPE(left,      double)) && (IS_TYPE(right,      double))) 
                return std::any_cast<     double>(left) + std::any_cast<     double>(right);
            if ((IS_TYPE(left, std::string)) && (IS_TYPE(right, std::string)))
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            return nullptr;
        }

        if (expr->op_.type == GREATER      ) return std::any_cast<double>(left) >  std::any_cast<double>(right);
        if (expr->op_.type == GREATER_EQUAL) return std::any_cast<double>(left) >= std::any_cast<double>(right);
        if (expr->op_.type ==    LESS      ) return std::any_cast<double>(left) <  std::any_cast<double>(right);
        if (expr->op_.type ==    LESS_EQUAL) return std::any_cast<double>(left) <= std::any_cast<double>(right);

        if (expr->op_.type == EQUAL_EQUAL) return  is_equal(left, right);
        if (expr->op_.type ==  BANG_EQUAL) return !is_equal(left, right);
        return nullptr;
    }
  
    std::any visitGroupingExpr(Grouping* expr) override { return evaluate(expr->expr_); }
  
    std::any visitLiteralExpr(Literal* expr) override { return expr->value_; }
  
    std::any visitUnaryExpr(Unary* expr) override {
        std::any right = evaluate(expr->right_);

        if (expr->op_.type == MINUS) return -std::any_cast<double>(right);
        if (expr->op_.type ==  BANG) return !is_truthy(right);
        return nullptr;
    }

private:
    std::any evaluate(Expr* expr) { return expr->accept(this); }

    bool is_truthy(std::any value) {
        if (IS_TYPE(value, std::nullptr_t)) return false;
        if (IS_TYPE(value, bool)) return std::any_cast<bool>(value);
        return true;
    }

    bool is_equal(std::any left, std::any right) {
        if (left.type().name() != right.type().name()) return false;

        if (IS_TYPE(left, std::nullptr_t)) return true;
        if (IS_TYPE(left,           bool)) return std::any_cast<       bool>(left) == std::any_cast<       bool>(right);
        if (IS_TYPE(left,         double)) return std::any_cast<     double>(left) == std::any_cast<     double>(right);
        if (IS_TYPE(left,    std::string)) return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);

        return false;
    }
};
} // namespace lox