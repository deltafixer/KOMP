#pragma once

#include "ASTNode.hpp"

class IdentifierNode: public ASTNode {
public:
    IdentifierNode(const char *idName): ASTNode("IdentifierNode"), m_id(string(idName)) { cout << "IDENTIFIER CHAR*: '" << idName << "'" << endl; }
    IdentifierNode(string idName): ASTNode("IdentifierNode"), m_id(idName)  { cout << "IDENTIFIER STRING: '" << idName << "'" << endl; }
    virtual ~IdentifierNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual string id() const { return m_id; }

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": \"" << m_id << "\"" << endl;
    }
protected:
    string     m_id;
};
