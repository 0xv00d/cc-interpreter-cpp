#pragma once

#include "expressions.hpp"

namespace lox {
struct Expression;
struct Print;
struct Var;
struct Block;
struct If;
struct While;

template <typename T>
class StmtVisitor {
public:
    virtual T visit_expression_stmt(Expression*) = 0;
    virtual T visit_print_stmt(Print*) = 0;
    virtual T visit_var_stmt(Var*) = 0;
    virtual T visit_block_stmt(Block*) = 0;
    virtual T visit_if_stmt(If*) = 0;
    virtual T visit_while_stmt(While*) = 0;
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
struct If: public Stmt {
    If(Expr* condition, Stmt* then_branch, Stmt* else_branch)
        : condition_(condition), then_branch_(then_branch), else_branch_(else_branch) {}
    
    Expr* condition_;
    Stmt* then_branch_;
    Stmt* else_branch_;
    
    void accept(StmtVisitor<void>* visitor) override { visitor->visit_if_stmt(this); }
};
struct While: public Stmt {
    While(Expr* condition, Stmt* body) : condition_(condition), body_(body) {}

    Expr* condition_;
    Stmt* body_;
    
    void accept(StmtVisitor<void>* visitor) override { visitor->visit_while_stmt(this); }
};
} // namespace lox