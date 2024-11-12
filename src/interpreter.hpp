#include "ast.hpp"
#include "errors.hpp"

namespace lox {
class Interpreter: public Visitor<std::any> {
public:
    void interpret(Expr* expr) { 
        try {
            std::any value = evaluate(expr);
            std::cout << stringify(value) << std::endl;
        } catch (RuntimeError error) {
            err::runtimeError(error);
        }
    }

    std::any visitBinaryExpr(Binary* expr) override {
        std::any  left = evaluate(expr-> left_);
        std::any right = evaluate(expr->right_);

        if (expr->op_.type == MINUS) {
            assert_numbers(expr->op_, left, right);
            return std::any_cast<double>(left) - std::any_cast<double>(right);
        }
        if (expr->op_.type == SLASH) {
            assert_numbers(expr->op_, left, right);
            return std::any_cast<double>(left) / std::any_cast<double>(right);
        }
        if (expr->op_.type ==  STAR) {
            assert_numbers(expr->op_, left, right);
            return std::any_cast<double>(left) * std::any_cast<double>(right);
        }
        if (expr->op_.type ==  PLUS) {
            if ((IS_TYPE(left,      double)) && (IS_TYPE(right,      double))) 
                return std::any_cast<     double>(left) + std::any_cast<     double>(right);
            if ((IS_TYPE(left, std::string)) && (IS_TYPE(right, std::string)))
                return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
            throw RuntimeError(expr->op_, "Operands must be two numbers or two strings.");
        }

        if (expr->op_.type == GREATER      ) {
            assert_numbers(expr->op_, left, right);
            return std::any_cast<double>(left) >  std::any_cast<double>(right);
        }
        if (expr->op_.type == GREATER_EQUAL) {
            assert_numbers(expr->op_, left, right);
            return std::any_cast<double>(left) >= std::any_cast<double>(right);
        }
        if (expr->op_.type ==    LESS      ) {
            assert_numbers(expr->op_, left, right);
            return std::any_cast<double>(left) <  std::any_cast<double>(right);
        }
        if (expr->op_.type ==    LESS_EQUAL) {
            assert_numbers(expr->op_, left, right);
            return std::any_cast<double>(left) <= std::any_cast<double>(right);
        }

        if (expr->op_.type == EQUAL_EQUAL) return  is_equal(left, right);
        if (expr->op_.type ==  BANG_EQUAL) return !is_equal(left, right);
        return nullptr;
    }
  
    std::any visitGroupingExpr(Grouping* expr) override { return evaluate(expr->expr_); }
  
    std::any visitLiteralExpr(Literal* expr) override { return expr->value_; }
  
    std::any visitUnaryExpr(Unary* expr) override {
        std::any right = evaluate(expr->right_);

        if (expr->op_.type == MINUS) {
            assert_number(expr->op_, right);
            return -std::any_cast<double>(right);
        }
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

    void assert_number(Token op, std::any operand) {
        if (IS_TYPE(operand, double)) return;
        throw RuntimeError(op, "Operand must be a number.");
    }

    void assert_numbers(Token op, std::any left, std::any right) {
        if (IS_TYPE(left, double) && IS_TYPE(right, double)) return;
        throw RuntimeError(op, "Operands must be numbers.");
    }

    std::string stringify(std::any value) {
        if (IS_TYPE(value, std::nullptr_t)) return "nil";
        if (IS_TYPE(value, std::string)) return std::any_cast<std::string>(value);
        if (IS_TYPE(value, double)) return trimmed_double(std::any_cast<double>(value));
        if (IS_TYPE(value, bool)) return std::any_cast<bool>(value) ? "true" : "false";
        return "?";
    }
};
} // namespace lox