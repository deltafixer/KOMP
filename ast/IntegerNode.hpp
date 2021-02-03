#pragma once

#include "ASTNode.hpp"

class IntegerNode: public ASTNode {
public:
    IntegerNode(int value): ASTNode("IntegerNode"), m_value(value) {  }
    virtual ~IntegerNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual int value() const { return m_value; }

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": " << m_value << endl;
    }
protected:
    int     m_value;
};
