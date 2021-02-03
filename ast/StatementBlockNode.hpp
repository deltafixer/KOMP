#pragma once

#include "ASTNode.hpp"

class StatementBlockNode: public ASTNode {
public:
    StatementBlockNode(ASTNode *statements): ASTNode("StatementBlockNode") { pushChild(statements); }
    virtual ~StatementBlockNode() { }

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
