#ifndef __PVISITOR_H__ // {
#define __PVISITOR_H__

#include "nodes.h"

class printing_visitor : public nodes::visitor
{
public:
    virtual void visit(nodes::node &);
    virtual void visit(nodes::error &);
    virtual void visit(nodes::prefix_unary_operator &);
    virtual void visit(nodes::binary_operator &);
    virtual void visit(nodes::numeric_literal &);
    virtual void visit(nodes::identifier &);
    virtual void visit(nodes::statement &);
    virtual void visit(nodes::statement_list &);
};

#endif // } _PVISITOR_H__
