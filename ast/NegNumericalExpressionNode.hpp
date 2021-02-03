#pragma once

#include "ASTNode.hpp"

class NegNumericalExpressionNode : public ASTNode
{
public:
    NegNumericalExpressionNode(ASTNode *op) : ASTNode("NegNumericalExpressionNode") { pushChild(op); }
    virtual ~NegNumericalExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"op\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
