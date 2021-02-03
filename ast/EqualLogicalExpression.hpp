#pragma once

#include "BinaryOperatorNode.hpp"

class EqualLogicalExpression : public BinaryOperatorNode
{
public:
    EqualLogicalExpression(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "EqualLogicalExpression") {}
    virtual ~EqualLogicalExpression() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
