#pragma once

#include "ASTNode.hpp"

class RepeatUntilNode : public ASTNode
{
public:
    RepeatUntilNode(ASTNode *statements, ASTNode *expression) : ASTNode("RepeatUntilNode")
    {
        pushChild(statements);
        pushChild(expression);
    }
    virtual ~RepeatUntilNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"expressions\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"expression\": {" << endl;
        m_children[1]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
