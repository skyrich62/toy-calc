#include "pvisitor.h"
#include <iostream>

void
printing_visitor::visit(nodes::node &n, nodes::traversal &, int)
{
    std::cerr << "Shouldn't have happened: " << __PRETTY_FUNCTION__ << std::endl;
}

void
printing_visitor::visit(nodes::error &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "error" << std::endl;
}

void
printing_visitor::visit(nodes::prefix_unary_operator &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "unary_op:  " << n.op();
    std::cout << ", operand: " << (*n.begin())->node_id();
    std::cout << std::endl;
}

void
printing_visitor::visit(nodes::binary_operator &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "binary_op:  " << n.op();
    std::cout << ", operands: ";
    for (auto it = n.cbegin(); it != n.cend(); ++it) {
        std::cout << (*it)->node_id() << " ";
    }
    std::cout << std::endl;
}

void
printing_visitor::visit(nodes::numeric_literal &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "numeric_literal: " << n.value() << std::endl;
}

void
printing_visitor::visit(nodes::identifier &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "identifier: " << n.id() << std::endl;
}

void
printing_visitor::visit(nodes::statement &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "statement, expression: " << (*n.begin())->node_id();
    std::cout << std::endl;
}

void
printing_visitor::visit(nodes::statement_list &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "statement_list" << std::endl;
}

void
printing_visitor::visit(nodes::parend_expr &n, nodes::traversal &, int)
{
    std::cout << n.node_id() << ": ";
    std::cout << "parend_expr, expression: " << (*n.begin())->node_id();
    std::cout << std::endl;
}
