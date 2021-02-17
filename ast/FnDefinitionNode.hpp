#pragma once

#include "ASTNode.hpp"

class FnDefinitionNode : public ASTNode
{
public:
    FnDefinitionNode(ASTNode *identifier, ASTNode *params, ASTNode *statements) : ASTNode("FnDefinitionNode")
    {
        pushChild(identifier);
        pushChild(params);
        pushChild(statements);
    }
    virtual ~FnDefinitionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"identifier\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"params\": {" << endl;
        m_children[1]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"statements\": {" << endl;
        m_children[2]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
