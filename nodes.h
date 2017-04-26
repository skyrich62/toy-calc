#ifndef _NODES_H__ // {
#define _NODES_H__

#include <list>
#include <memory>

namespace nodes {

class visitor;

class node
{
public:
    using ptr = std::shared_ptr<node>;
    using Children = std::list<ptr>;
    using iterator = Children::iterator;
    using const_iterator = Children::const_iterator;

    node() = default;
    virtual ~node() = default;

    void addChild(ptr node)                      { _children.push_back(node); }
    iterator begin()                             { return _children.begin(); }
    iterator end()                               { return _children.end(); }
    const_iterator cbegin() const                { return _children.cbegin(); }
    const_iterator cend() const                  { return _children.cend(); }
    virtual void accept(visitor&);
private:
    Children _children;
};

class error : public node
{
public:
    virtual void accept(visitor *);
};

class expression : public node
{
public:
    expression() = default;
    virtual ~expression() = default;
    virtual void accept(visitor&);
};

class operation : public node
{
public:
    operation() = default;
    virtual ~operation();
    void setOperator(const std::string &op)      { _op = op; }
    void addOperand(ptr node)                    { addChild(node); }
private:
    std::string _op;
};

class prefix_unary_operator : public operation
{
public:
    prefix_unary_operator() = default;
    virtual ~prefix_unary_operator() = default;
    virtual void accept(visitor&);

};

class binary_operator : public operation
{
public:
    binary_operator() = default;
    virtual ~binary_operator() = default;
    virtual void accept(visitor&);
};

class numeric_literal : public expression
{
public:
    numeric_literal() = default;
    virtual ~numeric_literal() = default;
    void setValue(long long int val)             { _value = val; }
    virtual void accept(visitor&);
private:
    long long int _value;
};

class identifier : public expression
{
public:
    identifier() = default;
    virtual ~identifier() = default;
    void setIdentifier(const std::string &id)    { _id = id; }
    virtual void accept(visitor&);
private:
    std::string _id;
};

class statement : public node
{
public:
    statement() = default;
    virtual ~statement() = default;
    virtual void accept(visitor&);
};

class statement_list : public node
{
public:
    statement_list() = default;
    virtual ~statement_list() = default;
    void addStatement(ptr stmt)                  { addChild(stmt); }
    virtual void accept(visitor&);
};

class visitor
{
public:
    virtual void visit(node &) =0;
    virtual void visit(error &) = 0;
    virtual void visit(prefix_unary_operator &) = 0;
    virtual void visit(binary_operator &) = 0;
    virtual void visit(numeric_literal &) = 0;
    virtual void visit(identifier &) = 0;
    virtual void visit(statement &) = 0;
    virtual void visit(statement_list &) = 0;
};

} // namespace nodes

#endif // } _NODES_H__
