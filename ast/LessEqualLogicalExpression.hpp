#pragma once

#include "BinaryOperatorNode.hpp"

class LessEqualLogicalExpression : public BinaryOperatorNode
{
public:
    LessEqualLogicalExpression(ASTNode *lop, ASTNode *rop) : BinaryOperatorNode(lop, rop, "LessEqualLogicalExpression") {}
    virtual ~LessEqualLogicalExpression() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
};
