#pragma once

#include "BinaryOperatorNode.hpp"

class DivNumericalExpressionNode: public BinaryOperatorNode {
public:
    DivNumericalExpressionNode(ASTNode *lop, ASTNode *rop): BinaryOperatorNode(lop, rop, "AddNumericalExpressionNode") { }
    virtual ~DivNumericalExpressionNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  
};
