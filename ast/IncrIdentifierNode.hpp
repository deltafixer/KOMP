#pragma once

#include "ASTNode.hpp"

class IncrIdentifierNode : public ASTNode
{
public:
    IncrIdentifierNode(ASTNode *id) : ASTNode("IncrIdentifierNode") { pushChild(id); }
    virtual ~IncrIdentifierNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"id\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
