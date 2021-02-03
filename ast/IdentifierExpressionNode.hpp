#pragma once

#include "ASTNode.hpp"

class IdentifierExpressionNode: public ASTNode {
public:
    IdentifierExpressionNode(ASTNode *identifier): ASTNode("IdentifierExpressionNode") { pushChild(identifier); }
    virtual ~IdentifierExpressionNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual string id() const { return m_id; }

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"identifier:\": {" << endl;
        m_children[0]->toStream(out, subIndent+indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
protected:
    string     m_id;
};
