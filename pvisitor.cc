/**
 * @file
 * The implementation of the printing_visitor.
 * @copyright
 * (C) 2017 Rich Newman.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
    std::cout << "error, children: ";
    for (auto it = n.cbegin(); it != n.cend(); ++it) {
        std::cout << (*it)->node_id() << " ";
    }
    std::cout << std::endl;
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
