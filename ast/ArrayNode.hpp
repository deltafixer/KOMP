#pragma once

#include "ASTNode.hpp"

class ArrayNode : public ASTNode
{
public:
    ArrayNode(ASTNode *numericalExpressions) : ASTNode("ArrayNode") { pushChild(numericalExpressions); }
    virtual ~ArrayNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    // TODO:
    virtual void toStream(ostream &out, string indent = "")
    {
    }
};
