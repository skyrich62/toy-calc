#include "nodes.h"

namespace nodes
{
void
node::accept(visitor &v)
{
    v.visit(*this);
}

void
expression::accept(visitor &v)
{
    v.visit(*this);
}

operation::~operation()
{
}

void
prefix_unary_operator::accept(visitor &v)
{
    v.visit(*this);
}

void
binary_operator::accept(visitor &v)
{
    v.visit(*this);
}

void
numeric_literal::accept(visitor &v)
{
    v.visit(*this);
}

void
identifier::accept(visitor &v)
{
    v.visit(*this);
}

void
statement::accept(visitor &v)
{
    v.visit(*this);
}

void
statement_list::accept(visitor &v)
{
    v.visit(*this);
}

} // namespace nodes
