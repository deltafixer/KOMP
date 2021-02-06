#pragma once

#include "ASTNode.hpp"

class ArrayNode : public ASTNode
{
public:
    ArrayNode(ASTNode *expressions) : ASTNode("ArrayNode")
    {
        for (auto it = expressions->begin(); it != expressions->end(); ++it)
        {
            pushChild(*it);
        }
    }
    virtual ~ArrayNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    // TODO:
    virtual void toStream(ostream &out, string indent = "")
    {
    }
};
