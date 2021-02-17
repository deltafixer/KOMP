#pragma once

#include "ASTNode.hpp"

class FnParamNode : public ASTNode
{
public:
    FnParamNode(ASTNode *param) : ASTNode("FnParamNode")
    {
        pushChild(param);
    }

    virtual ~FnParamNode(){};

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

class FnIntParamNode : public FnParamNode
{
public:
    FnIntParamNode(ASTNode *param) : FnParamNode(param)
    {
        pushChild(param);
    }

    virtual ~FnIntParamNode(){};

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

class FnArrayParamNode : public FnParamNode
{
public:
    FnArrayParamNode(ASTNode *param) : FnParamNode(param)
    {
        pushChild(param);
    }

    virtual ~FnArrayParamNode(){};

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
