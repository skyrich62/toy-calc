#ifndef __RVISITOR_H__ // {
#define __RVISITOR_H__

#include "nodes.h"

class natural_visitor : public nodes::visitor
{
public:
    natural_visitor(visitor &v) : _visitor(&v) { }
    virtual void visit(nodes::node &, nodes::traversal &, int);
    virtual void visit(nodes::error &, nodes::traversal &, int);
    virtual void visit(nodes::prefix_unary_operator &, nodes::traversal &, int);
    virtual void visit(nodes::binary_operator &, nodes::traversal &, int);
    virtual void visit(nodes::numeric_literal &, nodes::traversal &, int);
    virtual void visit(nodes::identifier &, nodes::traversal &, int);
    virtual void visit(nodes::statement &, nodes::traversal &, int);
    virtual void visit(nodes::statement_list &, nodes::traversal &, int);
private:
    visitor *_visitor;
};

class reconstructing_visitor : public nodes::visitor
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
};

#endif // } _RVISITOR_H__
