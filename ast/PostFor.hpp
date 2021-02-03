#pragma once

#include "ASTNode.hpp"

class PostFor : public ASTNode
{
public:
    PostFor() : ASTNode("PostFor") {}
    PostFor(ASTNode *assignmentExpression) : ASTNode("PostFor") { pushChild(assignmentExpression); }
    PostFor(ASTNode *postFor, ASTNode *assignmentExpression) : ASTNode("PostFor")
    {
        pushChild(postFor);
        pushChild(assignmentExpression);
    }
    virtual ~PostFor() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        if (m_children.size() == 1)
        {
            out << subIndent << "\"assignmentExpression\": {" << endl;
            m_children[0]->toStream(out, subIndent + indentShift);
            out << subIndent << "}" << endl;
        }
        if (m_children.size() == 2)
        {
            out << subIndent << "\"postFor\": {" << endl;
            m_children[0]->toStream(out, subIndent + indentShift);
            out << subIndent << "}," << endl;
            out << subIndent << "\"assignmentExpression\": {" << endl;
            m_children[1]->toStream(out, subIndent + indentShift);
            out << subIndent << "}" << endl;
        }
        out << indent << "}" << endl;
    }
};
