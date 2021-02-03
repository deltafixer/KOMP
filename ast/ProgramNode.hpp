#pragma once

#include "ASTNode.hpp"

class ProgramNode: public ASTNode {
public:
    ProgramNode(ASTNode *statements): ASTNode("ProgramNode") { pushChild(statements); }
    virtual ~ProgramNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"statements\": {" << endl;
        m_children[0]->toStream(out, subIndent+indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
