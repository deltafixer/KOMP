#pragma once

#include "BinaryOperatorNode.hpp"

class AddNumericalExpressionNode: public BinaryOperatorNode {
public:
    AddNumericalExpressionNode(ASTNode *lop, ASTNode *rop): BinaryOperatorNode(lop, rop, "AddNumericalExpressionNode") { }
    virtual ~AddNumericalExpressionNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  
}; 
