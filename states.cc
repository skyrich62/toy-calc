#include "states.h"

namespace states
{
expression::~expression()
{
}

operation::~operation()
{
}

prefix_unary_operation::~prefix_unary_operation()
{
}

void
prefix_unary_operation::setOperator(const std::string &op)
{
    auto p = new nodes::prefix_unary_operator;
    p->setOperator(op);
    set(ptr(p));
}

binary_operation::~binary_operation()
{
}

void
binary_operation::setOperator(const std::string &)
{
    auto p = new nodes::binary_operator;
    p->addChild(get());
    set(ptr(p));
}

void
statement::setExpression(ptr expr)
{
    _stmt->addChild(expr);
}

void
statement_list::addStatement(ptr stmt)
{
    _stmts->addChild(stmt);
}


} // namespace states
