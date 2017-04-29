/**
 * @file
 * The various parser states.
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

#ifndef _STATES_H__ // {
#define _STATES_H__

#include "nodes.h"

namespace states
{
    using ptr = nodes::node::ptr;

class statement_list
{
public:
    statement_list() : _stmts(new nodes::statement_list) { }

    template<typename Input, typename... States>
    statement_list(const Input &in, States&& ... st) :
        _stmts(new nodes::statement_list)
    { }

    template<typename Input, typename... States>
    void success(const Input &in, States&& ... st)
    { }

    void addStatement(ptr stmt);
    ptr get() const                              { return _stmts; }
private:
    ptr _stmts;
};

class statement
{
public:
    template<typename Input, typename... States>
    statement(const Input &in, States&& ... st) :
        _stmt(new nodes::statement)
    {
    }

    template<typename Input>
    void success(const Input &in, statement_list &st)
    {
        st.addStatement(_stmt);
    }

    void setExpression(ptr expr);

private:
    ptr _stmt;
};

class operation;

class expression
{
public:
    expression() = default;

    template<typename Input>
    expression(const Input &in, expression &st)
    {
        set(st.get());
    }

    virtual ~expression();

    void set(ptr expr)                    { _expr = expr; }
    ptr get() const                       { return _expr; }

    template<typename Input>
    void success(const Input &in, expression& st)
    {
        st.set(get());
    }

    template<typename Input>
    void success(const Input &in, operation& st);

    template<typename Input>
    void success(const Input &in, statement& st)
    {
        st.setExpression(get());
    }

    nodes::expression* expr() const;

private:
    ptr _expr;
};

class operation : public expression
{
public:
    operation() = default;
    virtual ~operation();

    virtual void setOperator(const std::string &op) = 0;
    void addOperand(ptr op)
    {
        if (get()) {
            get()->addChild(op);
        } else {
            set(op);
        }
    }
};

class parend_expr : public operation
{
public:
    template<typename Input, typename... States>
    parend_expr(const Input &in, States&& ...st)
    {
        std::cout << "creating parend_expr node" << std::endl;
        set(ptr(new nodes::parend_expr));
    }
    virtual void setOperator(const std::string &op)  { }
};

class prefix_unary_operation : public operation
{
public:
    virtual ~prefix_unary_operation();

    template<typename Input, typename... States>
    prefix_unary_operation(const Input &in, States&& ...st)
    { }

    virtual void setOperator(const std::string &op);
};

class binary_operation : public operation
{
public:
    binary_operation() = default;
    virtual ~binary_operation();

    template<typename Input, typename... States>
    binary_operation(const Input &in, States&& ... st)
    { }

    virtual void setOperator(const std::string &op);

};

template<typename Input>
void expression::success(const Input &in, operation& st)
{
    st.addOperand(get());
}

} // namespace states
#endif // } _STATES_H__
