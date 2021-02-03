#pragma once

#include "BinaryOperatorNode.hpp"

class MulNumericalExpressionNode: public BinaryOperatorNode {
public:
    MulNumericalExpressionNode(ASTNode *lop, ASTNode *rop): BinaryOperatorNode(lop, rop, "MulNumericalExpressionNode") { }
    virtual ~MulNumericalExpressionNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  
};
