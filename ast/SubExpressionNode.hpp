#pragma once

#include "BinaryOperatorNode.hpp"

class SubExpressionNode : public BinaryOperatorNode
{
public:
    SubExpressionNode(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "SubExpressionNode") {}
    virtual ~SubExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
