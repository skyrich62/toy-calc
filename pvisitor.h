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
