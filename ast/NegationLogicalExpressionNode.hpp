#pragma once

#include "ASTNode.hpp"

class NegationLogicalExpressionNode : public ASTNode
{
public:
    NegationLogicalExpressionNode(ASTNode *logicalExpression) : ASTNode("NegationLogicalExpressionNode") { pushChild(logicalExpression); }
    virtual ~NegationLogicalExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"logicalExpression\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
