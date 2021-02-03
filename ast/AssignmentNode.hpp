#pragma once

#include "ASTNode.hpp"

class AssignmentNode: public ASTNode {
public:
    AssignmentNode(ASTNode *assignmentExpression): ASTNode("AssignmentNode") { pushChild(assignmentExpression); }
    virtual ~AssignmentNode() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"assignmentExpression\": {" << endl;
        m_children[0]->toStream(out, subIndent+indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
