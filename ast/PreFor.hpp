#pragma once

#include "ASTNode.hpp"

class PreFor: public ASTNode {
public:
    PreFor(): ASTNode("PreFor") { }
    PreFor(ASTNode *assignmentExpression): ASTNode("PreFor") { pushChild(assignmentExpression); }
    virtual ~PreFor() { }

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); } 
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }  

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        if (m_children.size() == 1) {
            out << subIndent << "\"assignmentExpression\": {" << endl;
            m_children[0]->toStream(out, subIndent+indentShift);
            out << subIndent << "}" << endl;
        }
        out << indent << "}" << endl;
    }
};
