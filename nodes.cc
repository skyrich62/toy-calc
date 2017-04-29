#include "nodes.h"

namespace nodes
{

size_t node::_next_id{0};

void
node::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

void
expression::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

operation::~operation()
{
}

void
prefix_unary_operator::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

void
binary_operator::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

void
numeric_literal::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

void
identifier::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

void
statement::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

void
statement_list::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

void
parend_expr::accept(visitor &v, traversal &ilt, int mode)
{
    v.visit(*this, ilt, mode);
}

// ----------------------- traversal -------------------------------

void
traversal::traverse(node &n, int mode)
{
    if (_abort) {
        return;
    }
    if (mode == NATURAL) {
        n.accept(*_visitor, *this, mode);
        return;
    }
    if (mode & PRE) {
        n.accept(*_visitor, *this, PRE);
    }
    for (auto& p : n) {
        if (p) {
            traverse(*p, mode);
        }
    }
    if (mode & POST) {
        n.accept(*_visitor, *this, POST);
    }
}

} // namespace nodes
