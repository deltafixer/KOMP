#pragma once

#include <vector>
#include <ostream>
#include <string>

#include "ASTNodeVisitor.hpp"

using namespace std;

const string indentShift = "  ";

class ASTNode
{
public:
    ASTNode(string type = "") : m_type(type) {}
    virtual ~ASTNode()
    {
        for (auto child : m_children)
            delete child;
    }

    virtual void accept(ASTNodeVisitor &visitor) = 0;
    virtual void accept(ASTNodeVisitor &&visitor)
    {
        ASTNodeVisitor &rVisitor = visitor;
        accept(rVisitor);
    }
    virtual void acceptChildren(ASTNodeVisitor &visitor)
    {
        for (auto child : m_children)
            child->accept(visitor);
    }

    virtual void acceptTopDown(ASTNodeVisitor &visitor)
    {
        this->accept(visitor);
        for (auto child : m_children)
            child->acceptTopDown(visitor);
    }
    virtual void acceptBottomUp(ASTNodeVisitor &visitor)
    {
        for (auto child : m_children)
            child->acceptBottomUp(visitor);
        this->accept(visitor);
    }

    typedef vector<ASTNode *>::iterator iterator;

    iterator begin() { return m_children.begin(); }
    iterator end() { return m_children.end(); }

    virtual int numChildren() { return m_children.size(); }
    virtual ASTNode &getChild(int idx)
    {
        if (idx < 0 || idx >= m_children.size())
            throw runtime_error("invalid child index");
        if (m_children[idx] == nullptr)
            throw runtime_error("child is NULL");
        return *m_children[idx];
    }

    friend ostream &operator<<(ostream &out, ASTNode &node)
    {
        node.toStream(out);
        return out;
    }
    virtual string type() const { return m_type; }

    virtual void toStream(ostream &out, string indent = "") = 0;

protected:
    string m_type;
    vector<ASTNode *> m_children;

    virtual void pushChild(ASTNode *child) { m_children.push_back(child); }
};
