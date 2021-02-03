#pragma once

#include "ASTNode.hpp"


class IfNode: public ASTNode {
public:
    IfNode(ASTNode *expression, ASTNode *statement): ASTNode("IfNode") { pushChild(expression); pushChild(statement); }
    virtual ~IfNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"expression\": {" << endl;
        m_children[0]->toStream(out, subIndent+indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"statement\": {" << endl;
        m_children[1]->toStream(out, subIndent+indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
