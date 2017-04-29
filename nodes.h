/**
 * @file
 * This file describes the nodes, vistor and traversal of the parse tree.
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

#ifndef _NODES_H__ // {
#define _NODES_H__

#include <list>
#include <memory>

namespace nodes {

class visitor;
class traversal;

class node
{
public:
    using ptr = std::shared_ptr<node>;
    using Children = std::list<ptr>;
    using iterator = Children::iterator;
    using const_iterator = Children::const_iterator;

    node() : _node_id(_next_id)
    {
        ++_next_id;
    }

    virtual ~node() = default;

    void addChild(ptr node)                      { _children.push_back(node); }
    iterator begin()                             { return _children.begin(); }
    iterator end()                               { return _children.end(); }
    const_iterator cbegin() const                { return _children.cbegin(); }
    const_iterator cend() const                  { return _children.cend(); }
    virtual void accept(visitor&, traversal&, int);
    size_t node_id(void) const                   { return _node_id; }
private:
    Children      _children;
    size_t        _node_id;
    static size_t _next_id;
};

class error : public node
{
public:
    virtual void accept(visitor &, traversal&, int);
};

class expression : public node
{
public:
    expression() = default;
    virtual ~expression() = default;
    virtual void accept(visitor&, traversal&, int);
private:
};

class parend_expr : public expression
{
public:
    parend_expr() = default;
    virtual ~parend_expr() = default;
    virtual void accept(visitor&, traversal&, int);
};

class operation : public expression
{
public:
    operation() = default;
    virtual ~operation();
    void setOperator(const std::string &op)      { _op = op; }
    void addOperand(ptr node)                    { addChild(node); }
    std::string op(void) const                   { return _op; }
private:
    std::string _op;
};

class prefix_unary_operator : public operation
{
public:
    prefix_unary_operator() = default;
    virtual ~prefix_unary_operator() = default;
    virtual void accept(visitor&, traversal&, int);

};

class binary_operator : public operation
{
public:
    binary_operator() = default;
    virtual ~binary_operator() = default;
    virtual void accept(visitor&, traversal&, int);
};

class numeric_literal : public expression
{
public:
    numeric_literal() = default;
    virtual ~numeric_literal() = default;
    void setValue(long long int val)             { _value = val; }
    virtual void accept(visitor&, traversal&, int);
    long long int value(void) const              { return _value; }
private:
    long long int _value;
};

class identifier : public expression
{
public:
    identifier() = default;
    virtual ~identifier() = default;
    void setIdentifier(const std::string &id)    { _id = id; }
    virtual void accept(visitor&, traversal&, int);
    std::string id(void) const                   { return _id; }
private:
    std::string _id;
};

class statement : public node
{
public:
    statement() = default;
    virtual ~statement() = default;
    virtual void accept(visitor&, traversal&, int);
};

class statement_list : public node
{
public:
    statement_list() = default;
    virtual ~statement_list() = default;
    void addStatement(ptr stmt)                  { addChild(stmt); }
    virtual void accept(visitor&, traversal&, int);
};

class visitor
{
public:
    virtual void visit(node &, traversal &, int) =0;
    virtual void visit(error &, traversal &, int) = 0;
    virtual void visit(prefix_unary_operator &, traversal &, int) = 0;
    virtual void visit(binary_operator &, traversal &, int) = 0;
    virtual void visit(numeric_literal &, traversal &, int) = 0;
    virtual void visit(identifier &, traversal &, int) = 0;
    virtual void visit(statement &, traversal &, int) = 0;
    virtual void visit(statement_list &, traversal &, int) = 0;
    virtual void visit(parend_expr &, traversal &, int) = 0;
};

class traversal
{
public:
    enum {PRE = 1, POST = 2, NATURAL = 4};
    traversal(visitor &v) : _visitor(&v), _abort(false) { }
    void  traverse(node &n, int mode);
    void disable(void)                           { _abort = true; }
    void set_visitor(visitor &v)                 { _visitor = &v; }
private:
    visitor *_visitor;
    bool     _abort;
};

} // namespace nodes

#endif // } _NODES_H__
