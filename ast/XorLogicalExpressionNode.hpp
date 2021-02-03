#pragma once

#include "BinaryOperatorNode.hpp"

class XorLogicalExpressionNode : public BinaryOperatorNode
{
public:
    XorLogicalExpressionNode(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "XorLogicalExpressionNode") {}
    virtual ~XorLogicalExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
