#include "interpreter.hpp"

namespace lox {
std::any Interpreter::visit_binary_expr(Binary* expr) {
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
        std::string err = "Operands must be two numbers or two strings.";
        throw RuntimeError(expr->op_, err.data());
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

std::any Interpreter::visit_unary_expr(Unary* expr) {
    std::any right = evaluate(expr->right_);

    if (expr->op_.type == MINUS) {
        assert_number(expr->op_, right);
        return -std::any_cast<double>(right);
    }
    if (expr->op_.type ==  BANG) return !is_truthy(right);
    return nullptr;
}

std::any Interpreter::visit_assign_expr(Assign* expr) {
    std::any value = evaluate(expr->value_);
    environment_.assign(expr->name_, value);
    return value;
}

void Interpreter::visit_print_stmt(Print* stmt) {
    std::any value = evaluate(stmt->expr_);
    std::cout << stringify(value) << std::endl;
}

void Interpreter::visit_var_stmt(Var* stmt) {
    std::any value = nullptr;
    if (stmt->initializer_ != nullptr) value = evaluate(stmt->initializer_);

    environment_.define(stmt->name_.lexeme, value);
}

void Interpreter::execute_block(std::vector<Stmt*> statements, Environment* environment) {
    Environment previous = environment_;
    environment_ = environment;

    try {
        for (auto stmt: statements) execute(stmt);
    } catch (RuntimeError error) {
        err::runtimeError(error);
    }

    environment_ = previous;
}
} // namespace lox