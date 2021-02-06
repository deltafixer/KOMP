#pragma once

#include "ASTNode.hpp"

class ExpressionsNode : public ASTNode
{
public:
    ExpressionsNode(ASTNode *numericalExpressions, ASTNode *numericalExpression) : ASTNode("ExpressionsNode")
    {
        for (auto it = numericalExpressions->begin(); it != numericalExpressions->end(); ++it)
        {
            pushChild(*it);
        }
        pushChild(numericalExpression);
    }
    ExpressionsNode(ASTNode *numericalExpression) : ASTNode("ExpressionsNode") { pushChild(numericalExpression); }
    virtual ~ExpressionsNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        if (m_children.size() == 1)
        {
            out << subIndent << "\"numericalExpression\": {" << endl;
            m_children[0]->toStream(out, subIndent + indentShift);
            out << subIndent << "}" << endl;
        }
        if (m_children.size() == 2)
        {
            out << subIndent << "\"numericalExpressions\": {" << endl;
            m_children[0]->toStream(out, subIndent + indentShift);
            out << subIndent << "}," << endl;
            out << subIndent << "\"numericalExpression\": {" << endl;
            m_children[1]->toStream(out, subIndent + indentShift);
            out << subIndent << "}" << endl;
        }
        out << indent << "}" << endl;
    }
};
