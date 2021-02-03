#pragma once

#include "BinaryOperatorNode.hpp"

class SubNumericalExpressionNode: public BinaryOperatorNode {
public:
    SubNumericalExpressionNode(ASTNode *lop, ASTNode *rop): BinaryOperatorNode(lop, rop, "SubNumericalExpressionNode") { }
    virtual ~SubNumericalExpressionNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  
};
