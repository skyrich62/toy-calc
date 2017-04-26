#ifndef _STATES_H__ // {
#define _STATES_H__

#include "nodes.h"

namespace states
{
    using ptr = nodes::node::ptr;

class expression
{
public:
    virtual ~expression();

    void set(ptr expr)                    { _expr = expr; }
    ptr get() const                       { return _expr; }

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
        get()->addChild(op);
    }
};

class prefix_unary_operation : public operation
{
public:
    virtual ~prefix_unary_operation();

    template<typename Input, typename... States>
    prefix_unary_operation(const Input &in, States&& ...st)
    { }

    template<typename Input, typename... States>
    void success(const Input &in, States&& ... st)
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

    template<typename Input, typename... States>
    void success(const Input &in, States&& ... st)
    { }

    virtual void setOperator(const std::string &op);

};

class statement
{
public:
    template<typename Input, typename... States>
    statement(const Input &in, States&& ... st)
    { }

    template<typename Input, typename... States>
    void success(const Input &in, States&& ... st)
    { }

    void setExpression(ptr expr);

private:
    ptr _stmt;
};

class statement_list
{
public:
    statement_list() = default;

    template<typename Input, typename... States>
    statement_list(const Input &in, States&& ... st)
    { }

    template<typename Input, typename... States>
    void success(const Input &in, States&& ... st)
    { }

    void addStatement(ptr stmt);
    ptr get() const                              { return _stmts; }
private:
    ptr _stmts;
};

} // namespace states
#endif // } _STATES_H__
