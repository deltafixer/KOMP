#pragma once

#include "ASTNode.hpp"

class FloatNode : public ASTNode
{
public:
    FloatNode(float value) : ASTNode("FloatNode"), m_value(value) {}
    virtual ~FloatNode() {}

    virtual void accept(ASTNodeVisitor &visitor) { visitor.visit(*this); }
    virtual void accept(ASTNodeVisitor &&visitor) { visitor.visit(*this); }

    virtual float value() const { return m_value; }

    virtual void toStream(ostream &out, string indent = "")
    {
        out << indent << "\"" << type() << "\": " << m_value << endl;
    }

protected:
    float m_value;
};
