#pragma once

#include "ASTNode.hpp"

class FnParamsNode : public ASTNode
{
public:
    FnParamsNode(ASTNode *params, ASTNode *param) : ASTNode("FnParamsNode")
    {
        for (auto it = params->begin(); it != params->end(); it++)
        {
            pushChild(*it);
        }
        pushChild(param);
    }

    FnParamsNode(ASTNode *param) : ASTNode("FnParamsNode")
    {
        pushChild(param);
    }

    FnParamsNode() : ASTNode("FnParamsNode")
    {
    }

    virtual ~FnParamsNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": {" << endl;
        string subIndent = indent + indentShift;
        for (int i = 0; i < numChildren(); i++)
        {
            out << subIndent << "\"param" << i << "\": {" << endl;
            m_children[i]->toStream(out, subIndent + indentShift);
            out << subIndent << "}," << endl;
        }
        out << indent << "}" << endl;
    }
};
