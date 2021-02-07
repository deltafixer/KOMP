#pragma once

#include "ASTNode.hpp"

class ReturnStatementNode : public ASTNode
{
public:
    ReturnStatementNode() : ASTNode("ReturnStatementNode")
    {
    }

    ReturnStatementNode(ASTNode *stmt) : ASTNode("ReturnStatementNode")
    {
        pushChild(stmt);
    }
    virtual ~ReturnStatementNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        if (numChildren() == 1)
        {
            out << subIndent << "\"statement\": {" << endl;
            m_children[0]->toStream(out, subIndent + indentShift);
            out << subIndent << "}" << endl;
        }
        out << indent << "}" << endl;
    }
};
