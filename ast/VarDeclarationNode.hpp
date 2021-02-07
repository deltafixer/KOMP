#pragma once

#include "ASTNode.hpp"

class VarDeclarationNode : public ASTNode
{
public:
    VarDeclarationNode(ASTNode *identifier, ASTNode *expression) : ASTNode("VarDeclarationNode")
    {
        pushChild(identifier);
        pushChild(expression);
    }
    virtual ~VarDeclarationNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"identifier\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"assignmentExpression\": {" << endl;
        m_children[1]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
