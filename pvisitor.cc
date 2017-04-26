#include "pvisitor.h"
#include <iostream>

void
printing_visitor::visit(nodes::node &n)
{
    std::cerr << "Shouldn't have happened: " << __PRETTY_FUNCTION__ << std::endl;
}

void
printing_visitor::visit(nodes::error &n)
{
    std::cout << "error" << std::endl;
}

void
printing_visitor::visit(nodes::prefix_unary_operator &n)
{
    std::cout << "unary_op: " << std::endl;
}

void
printing_visitor::visit(nodes::binary_operator &n)
{
    std::cout << "binary_op: " << std::endl;
}

void
printing_visitor::visit(nodes::numeric_literal &n)
{
    std::cout << "numeric_literal: " << std::endl;
}

void
printing_visitor::visit(nodes::identifier &n)
{
    std::cout << "identifier: " << std::endl;
}

void
printing_visitor::visit(nodes::statement &n)
{
    std::cout << "statement: " << std::endl;
}

void
printing_visitor::visit(nodes::statement_list &n)
{
    std::cout << "statement_list" << std::endl;
}
