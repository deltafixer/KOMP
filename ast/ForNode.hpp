#pragma once

#include "ASTNode.hpp"

class ForNode : public ASTNode
{
public:
    ForNode(ASTNode *preFor, ASTNode *expression, ASTNode *postFor, ASTNode *statement) : ASTNode("ForNode")
    {
        pushChild(preFor);
        pushChild(expression);
        pushChild(postFor);
        pushChild(statement);
    }
    virtual ~ForNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"preFor\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"expression\": {" << endl;
        m_children[1]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"postFor\": {" << endl;
        m_children[2]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"statement\": {" << endl;
        m_children[3]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
