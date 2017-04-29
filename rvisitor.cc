#include "rvisitor.h"
#include <iostream>

// --------------- natural_visitor --------------------------------
void
natural_visitor::visit(nodes::node &n, nodes::traversal &ilt, int mode)
{
}

void
natural_visitor::visit(nodes::error &n, nodes::traversal &ilt, int mode)
{
}

void
natural_visitor::visit(nodes::prefix_unary_operator &n,
                       nodes::traversal &ilt,
                       int mode)
{
    auto op1 = *(n.begin());
    _visitor->visit(n, ilt, mode);
    ilt.traverse(*op1, mode);
}

void
natural_visitor::visit(nodes::binary_operator &n, nodes::traversal &ilt, int mode)
{
    auto it = n.begin();
    auto op1 = *it;
    auto op2 = *++it;
    ilt.traverse(*op1, mode);
    _visitor->visit(n, ilt, mode);
    ilt.traverse(*op2, mode);
}

void
natural_visitor::visit(nodes::numeric_literal &n, nodes::traversal &ilt, int mode)
{
    _visitor->visit(n, ilt, mode);
}

void
natural_visitor::visit(nodes::identifier &n, nodes::traversal &ilt, int mode)
{
    _visitor->visit(n, ilt, mode);
}

void
natural_visitor::visit(nodes::statement &n, nodes::traversal &ilt, int mode)
{
    auto expr = *(n.begin());
    ilt.traverse(*expr, mode);
    _visitor->visit(n, ilt, mode);
}

void
natural_visitor::visit(nodes::statement_list &n, nodes::traversal &ilt, int mode)
{
    for (auto &p :n) {
        ilt.traverse(*p, mode);
    }
}

void
natural_visitor::visit(nodes::parend_expr &n, nodes::traversal &ilt, int mode)
{
    auto expr = *(n.begin());
    _visitor->visit(n, ilt, nodes::traversal::PRE);
    ilt.traverse(*expr, mode);
    _visitor->visit(n, ilt, nodes::traversal::POST);

}

// --------------- reconstructing_visitor --------------------------------

void
reconstructing_visitor::visit(nodes::node &n, nodes::traversal &, int)
{
    std::cerr << "Shouldn't have happened: " << __PRETTY_FUNCTION__ << std::endl;
}

void
reconstructing_visitor::visit(nodes::error &n, nodes::traversal &, int)
{
    std::cout << "error" << std::endl;
}

void
reconstructing_visitor::visit(nodes::prefix_unary_operator &n,
                              nodes::traversal &,
                              int mode)
{
    std::cout << " " << n.op();
}

void
reconstructing_visitor::visit(nodes::binary_operator &n,
                              nodes::traversal &,
                              int mode)
{
    std::cout << " " << n.op() << " ";
}

void
reconstructing_visitor::visit(nodes::numeric_literal &n, nodes::traversal &, int)
{
    std::cout << n.value();
}

void
reconstructing_visitor::visit(nodes::identifier &n, nodes::traversal &, int)
{
    std::cout << n.id();
}

void
reconstructing_visitor::visit(nodes::statement &n, nodes::traversal &, int)
{
    std::cout << ";" << std::endl;
}

void
reconstructing_visitor::visit(nodes::statement_list &n, nodes::traversal &, int)
{
}

void
reconstructing_visitor::visit(nodes::parend_expr &,
                              nodes::traversal &,
                              int mode)
{
    if (mode & nodes::traversal::PRE) {
        std::cout << "(";
        return;
    }

    if (mode & nodes::traversal::POST) {
        std::cout << ")";
        return;
    }
}
