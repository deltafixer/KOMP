#pragma once

#include "ASTNode.hpp"

class AssignmentExpressionNode : public ASTNode
{
public:
    AssignmentExpressionNode(ASTNode *identifier, ASTNode *expressionOrArray, ASTNode *expression = nullptr) : ASTNode("AssignmentExpressionNode")
    {
        pushChild(identifier);
        pushChild(expressionOrArray);
        if (expression)
        {
            pushChild(expression);
        }
    }
    virtual ~AssignmentExpressionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    // TODO:
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

protected:
    int m_id;
};
