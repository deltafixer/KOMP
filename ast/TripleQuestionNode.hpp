#pragma once

#include "ASTNode.hpp"

class TripleQuestionNode : public ASTNode
{
public:
    TripleQuestionNode(ASTNode *cond, ASTNode *gtZero, ASTNode *eqZero, ASTNode *ltZero) : ASTNode("TripleQuestionNode")
    {
        pushChild(cond);
        pushChild(gtZero);
        pushChild(eqZero);
        pushChild(ltZero);
    }
    virtual ~TripleQuestionNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        out << subIndent << "\"cond\": {" << endl;
        m_children[0]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;

        out << indent << "\"" << type() << "\": {" << endl;
        out << subIndent << "\"gtZero\": {" << endl;
        m_children[1]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;

        out << indent << "\"" << type() << "\": {" << endl;
        out << subIndent << "\"eqZero\": {" << endl;
        m_children[2]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;

        out << indent << "\"" << type() << "\": {" << endl;
        out << subIndent << "\"ltZero\": {" << endl;
        m_children[3]->toStream(out, subIndent + indentShift);
        out << subIndent << "}," << endl;
    }
};
