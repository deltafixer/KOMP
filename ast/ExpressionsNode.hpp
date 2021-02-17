#pragma once

#include "ASTNode.hpp"

class ExpressionsNode : public ASTNode
{
public:
    ExpressionsNode(ASTNode *expressions, ASTNode *expression) : ASTNode("ExpressionsNode")
    {
        for (auto it = expressions->begin(); it != expressions->end(); ++it)
        {
            pushChild(*it);
        }
        pushChild(expression);
    }
    ExpressionsNode(ASTNode *expression) : ASTNode("ExpressionsNode") { pushChild(expression); }
    virtual ~ExpressionsNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        if (m_children.size() == 1)
        {
            out << subIndent << "\"expression\": {" << endl;
            m_children[0]->toStream(out, subIndent + indentShift);
            out << subIndent << "}" << endl;
        }
        if (m_children.size() == 2)
        {
            out << subIndent << "\"expressions\": {" << endl;
            m_children[0]->toStream(out, subIndent + indentShift);
            out << subIndent << "}," << endl;
            out << subIndent << "\"expression\": {" << endl;
            m_children[1]->toStream(out, subIndent + indentShift);
            out << subIndent << "}" << endl;
        }
        out << indent << "}" << endl;
    }
};
