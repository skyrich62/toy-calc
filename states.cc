/**
 * @file
 * State implementation details.
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

#include <iostream>
#include "states.h"

namespace states
{
expression::~expression()
{
}

nodes::expression*
expression::expr() const
{
    return dynamic_cast<nodes::expression*>(_expr.get());
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
binary_operation::setOperator(const std::string &op)
{
    auto p = new nodes::binary_operator;
    p->setOperator(op);
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
