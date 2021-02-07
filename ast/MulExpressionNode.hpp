#pragma once

#include "BinaryOperatorNode.hpp"

class MulExpressionNode : public BinaryOperatorNode
{
public:
    MulExpressionNode(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "MulExpressionNode") {}
    virtual ~MulExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
