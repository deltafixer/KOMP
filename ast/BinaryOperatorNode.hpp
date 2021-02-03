#pragma once

#include "ASTNode.hpp"

class BinaryOperatorNode: public ASTNode {
public:
    BinaryOperatorNode(ASTNode *lop, ASTNode *rop, string type): ASTNode(type) { pushChild(lop); pushChild(rop); }
    virtual ~BinaryOperatorNode() { }

    virtual void toStream(ostream &out, string indent="") {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"lop\": {" << endl;
        m_children[0]->toStream(out, subIndent+indentShift);
        out << subIndent << "}," << endl;
        out << subIndent << "\"rop\": {" << endl;
        m_children[1]->toStream(out, subIndent+indentShift);
        out << subIndent << "}" << endl;
        out << indent << "}" << endl;
    }
};
