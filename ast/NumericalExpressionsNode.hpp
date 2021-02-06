#pragma once

#include "ASTNode.hpp"

class NumericalExpressionsNode : public ASTNode
{
public:
    NumericalExpressionsNode(ASTNode *numericalExpressions, ASTNode *numericalExpression) : ASTNode("NumericalExpressionsNode")
    {
        pushChild(numericalExpressions);
        pushChild(numericalExpression);
    }
    NumericalExpressionsNode(ASTNode *numericalExpression) : ASTNode("NumericalExpressionsNode") { pushChild(numericalExpression); }
    virtual ~NumericalExpressionsNode() {}

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
