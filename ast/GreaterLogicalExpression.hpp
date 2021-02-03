#pragma once

#include "BinaryOperatorNode.hpp"

class GreaterLogicalExpression: public BinaryOperatorNode {
public:
    GreaterLogicalExpression(ASTNode *lop, ASTNode *rop): BinaryOperatorNode(lop, rop, "GreaterLogicalExpression") { }
    virtual ~GreaterLogicalExpression() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  
};
