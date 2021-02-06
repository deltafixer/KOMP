#pragma once

#include "ASTNode.hpp"

class ArrayNode : public IdentifierNode
{
public:
    ArrayNode(ASTNode *numericalExpressions) : IdentifierNode("TMP_ARRAY_") { pushChild(numericalExpressions); }
    virtual ~ArrayNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    // TODO:
    virtual void toStream(ostream &out, string indent = "")
    {
    }
};
