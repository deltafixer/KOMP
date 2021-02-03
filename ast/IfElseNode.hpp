#pragma once

#include "ASTNode.hpp"

class IfElseNode: public ASTNode {
public:
    IfElseNode(ASTNode *expression, ASTNode *statementIf, ASTNode *statementElse): ASTNode("IfElseNode") { pushChild(expression); pushChild(statementIf); pushChild(statementElse); }
    virtual ~IfElseNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"expression\": {" << endl;
        m_children[0]->toStream(out, subIndent+indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"statementIf\": {" << endl;
        m_children[1]->toStream(out, subIndent+indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"statementElse\": {" << endl;
        m_children[2]->toStream(out, subIndent+indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
