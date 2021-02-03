#pragma once

#include "ASTNode.hpp"

class PrintNode : public ASTNode
{
public:
    PrintNode(ASTNode *expressionStatement) : ASTNode("PrintNode") { pushChild(expressionStatement); }
    virtual ~PrintNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"expressionStatement\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
