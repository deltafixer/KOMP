#pragma once

#include "BinaryOperatorNode.hpp"

class LessLogicalExpression : public BinaryOperatorNode
{
public:
    LessLogicalExpression(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "LessLogicalExpression") {}
    virtual ~LessLogicalExpression() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
