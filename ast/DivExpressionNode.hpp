#pragma once

#include "BinaryOperatorNode.hpp"

class DivExpressionNode : public BinaryOperatorNode
{
public:
    DivExpressionNode(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "AddExpressionNode") {}
    virtual ~DivExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
