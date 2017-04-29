/**
 * @file
 * This file describes the interface for the printing_visitor.
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

#ifndef __PVISITOR_H__ // {
#define __PVISITOR_H__

#include "nodes.h"

class printing_visitor : public nodes::visitor
{
public:
    virtual void visit(nodes::node &, nodes::traversal &, int);
    virtual void visit(nodes::error &, nodes::traversal &, int);
    virtual void visit(nodes::prefix_unary_operator &, nodes::traversal &, int);
    virtual void visit(nodes::binary_operator &, nodes::traversal &, int);
    virtual void visit(nodes::numeric_literal &, nodes::traversal &, int);
    virtual void visit(nodes::identifier &, nodes::traversal &, int);
    virtual void visit(nodes::statement &, nodes::traversal &, int);
    virtual void visit(nodes::statement_list &, nodes::traversal &, int);
    virtual void visit(nodes::parend_expr &, nodes::traversal &, int);
};

#endif // } _PVISITOR_H__
