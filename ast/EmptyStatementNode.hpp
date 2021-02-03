#pragma once

#include "ASTNode.hpp"

class EmptyStatementNode: public ASTNode {
public:
    EmptyStatementNode(): ASTNode("EmptyStatementNode") {  }
    virtual ~EmptyStatementNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {}" << endl;
    }
};
