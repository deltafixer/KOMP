#pragma once

#include "ASTNode.hpp"

class ExpressionStatementNode: public ASTNode {
public:
    ExpressionStatementNode(ASTNode *expression): ASTNode("ExpressionStatementNode") { pushChild(expression); }
    virtual ~ExpressionStatementNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }
 
    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"expression\": {" << endl;
        m_children[0]->toStream(out, subIndent+indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }  
};
