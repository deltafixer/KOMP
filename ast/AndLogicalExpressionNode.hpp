#pragma once

#include "BinaryOperatorNode.hpp"

class AndLogicalExpressionNode: public BinaryOperatorNode {
public:
    AndLogicalExpressionNode(ASTNode *lop, ASTNode *rop): BinaryOperatorNode(lop, rop, "AndLogicalExpressionNode") { }
    virtual ~AndLogicalExpressionNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  
};
