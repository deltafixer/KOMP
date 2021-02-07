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

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"array\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
