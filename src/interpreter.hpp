#pragma once

#include "ast.hpp"
#include "environment.hpp"
#include "errors.hpp"

namespace lox {
class Interpreter: public ExprVisitor<std::any>, public StmtVisitor<void> {
public:
    void interpret(std::vector<Stmt*> stmts) { 
        try {
            for (auto stmt: stmts) execute(stmt);
        } catch (RuntimeError error) {
            err::runtimeError(error);
        }
    }

    void interpret(Expr* expr) { 
        try {
            std::any value = evaluate(expr);
            std::cout << stringify(value) << std::endl;
        } catch (RuntimeError error) {
            err::runtimeError(error);
        }
    }

           std::any   visit_binary_expr( Binary*      ) override;
    inline std::any visit_grouping_expr(Grouping* expr) override { return evaluate(expr->expr_); }
    inline std::any  visit_literal_expr( Literal* expr) override { return expr->value_; }
           std::any    visit_unary_expr(   Unary*     ) override;
    inline std::any visit_variable_expr(Variable* expr) override { return environment_->get(expr->name_); }
           std::any   visit_assign_expr(  Assign*     ) override;

    inline void visit_expression_stmt(Expression* stmt) override { evaluate(stmt->expr_); }
           void      visit_print_stmt(     Print*     ) override;
           void        visit_var_stmt(       Var*     ) override;
    inline void      visit_block_stmt(     Block* stmt) override {
        execute_block(stmt->statements_, new Environment(environment_));
    }

private:
    Environment* environment_ = new Environment();

    std::any evaluate(Expr* expr) { return expr->accept(this); }
    void      execute(Stmt* stmt) {        stmt->accept(this); }

    void execute_block(std::vector<Stmt*>, Environment*);

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
        std::string err = "Operand must be a number.";
        throw RuntimeError(op, err.data());
    }

    void assert_numbers(Token op, std::any left, std::any right) {
        if (IS_TYPE(left, double) && IS_TYPE(right, double)) return;
        std::string err = "Operands must be numbers.";
        throw RuntimeError(op, err.data());
    }

    std::string trimmed_double(double value) {
        std::string vs = std::to_string(value);
        vs.erase(vs.find_last_not_of('0') + 1, std::string::npos);
        if (vs.back() == '.') vs.pop_back();
        return vs;
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