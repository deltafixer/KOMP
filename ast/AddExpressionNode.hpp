#pragma once

#include "BinaryOperatorNode.hpp"

class AddExpressionNode : public BinaryOperatorNode
{
public:
    AddExpressionNode(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "AddExpressionNode") {}
    virtual ~AddExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
