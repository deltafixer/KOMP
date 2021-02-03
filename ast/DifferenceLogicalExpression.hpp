#pragma once

#include "BinaryOperatorNode.hpp"

class DifferenceLogicalExpression : public BinaryOperatorNode
{
public:
    DifferenceLogicalExpression(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "DifferenceLogicalExpression") {}
    virtual ~DifferenceLogicalExpression() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
