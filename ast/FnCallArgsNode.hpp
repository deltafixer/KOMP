#pragma once

#include "ASTNode.hpp"

class FnCallArgsNode : public ASTNode
{
public:
    FnCallArgsNode(ASTNode *args, ASTNode *arg) : ASTNode("FnCallArgsNode")
    {
        for (auto it = args->begin(); it != args->end(); it++)
        {
            pushChild(*it);
        }
        pushChild(arg);
    }

    FnCallArgsNode(ASTNode *arg) : ASTNode("FnCallArgsNode")
    {
        pushChild(arg);
    }

    FnCallArgsNode() : ASTNode("FnCallArgsNode")
    {
    }

    virtual ~FnCallArgsNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        for (int i = 0; i < numChildren(); i++)
        {
            out << subIndent << "\"arg" << i << "\": {" << endl;
            m_children[i]->toStream(out, subIndent + indentShift);
            out << subIndent << "}," << endl;
        }
        out << indent << "}" << endl;
    }
};
