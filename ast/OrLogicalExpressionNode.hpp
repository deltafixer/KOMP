#pragma once

#include "BinaryOperatorNode.hpp"

class OrLogicalExpressionNode : public BinaryOperatorNode
{
public:
    OrLogicalExpressionNode(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "OrLogicalExpressionNode") {}
    virtual ~OrLogicalExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
