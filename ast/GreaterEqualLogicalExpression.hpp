#pragma once

#include "BinaryOperatorNode.hpp"

class GreaterEqualLogicalExpression : public BinaryOperatorNode
{
public:
    GreaterEqualLogicalExpression(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "GreaterEqualLogicalExpression") {}
    virtual ~GreaterEqualLogicalExpression() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
