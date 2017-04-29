/**
 * @file
 * The nodes of the parse tree, and the implementation of the traversal.
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
