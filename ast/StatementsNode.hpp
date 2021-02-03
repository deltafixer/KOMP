#pragma once

#include "ASTNode.hpp"

class StatementsNode: public ASTNode {
public:
    StatementsNode(ASTNode *statements, ASTNode *statement): ASTNode("StatementsNode") { pushChild(statements); pushChild(statement); }
    StatementsNode(ASTNode *statement): ASTNode("StatementsNode") { pushChild(statement); }
    virtual ~StatementsNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        if (m_children.size() == 1) {
            out << subIndent << "\"statement\": {" << endl;
            m_children[0]->toStream(out, subIndent+indentShift);
            out << subIndent << "}" << endl;
        }
        if (m_children.size() == 2) {
            out << subIndent << "\"statements\": {" << endl;
            m_children[0]->toStream(out, subIndent+indentShift);
            out << subIndent << "}," << endl;
            out << subIndent << "\"statement\": {" << endl;
            m_children[1]->toStream(out, subIndent+indentShift);
            out << subIndent << "}" << endl;
        }
        out << indent << "}" << endl;
    }
};
