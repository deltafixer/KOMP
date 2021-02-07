#include "../ast/all.hpp"

#include <deque>
#include <iostream>
#include <sstream>

using namespace std;

class PseudoAssemblerContext
{
protected:
    unordered_map<string, int> m_nameToId;
    unordered_map<string, string> m_nameToLabel;

    PseudoAssemblerContext *m_parent;

    static int NEXT_ID;

public:
    PseudoAssemblerContext()
    {
        m_parent = nullptr;
    }

    PseudoAssemblerContext(PseudoAssemblerContext *parent)
    {
        this->m_parent = parent;
    }

    PseudoAssemblerContext *get_parent()
    {
        return m_parent;
    }

    optional<int> get_id(const string &name)
    {
        if (m_nameToId.find(name) == m_nameToId.end())
        {
            if (m_parent != nullptr)
            {
                return m_parent->get_id(name);
            }

            return {};
        }

        return m_nameToId[name];
    }

    int get_id_or_throw(const string &name)
    {
        auto id_opt = get_id(name);
        if (!id_opt)
        {
            cout << "\nCompile error: identifier '" << name << "' not found" << endl;
            throw new runtime_error("Compile error: identifier '" + name + "' not found");
        }

        return *id_opt;
    }

    int add(const string &name)
    {
        m_nameToId[name] = PseudoAssemblerContext::NEXT_ID++;
        return m_nameToId[name];
    }

    void add_function(string name, string begin_label)
    {
        m_nameToLabel[name] = begin_label;
    }

    optional<string> get_function_label(const string &name)
    {
        if (m_nameToLabel.find(name) == m_nameToLabel.end())
        {
            if (m_parent != nullptr)
            {
                return m_parent->get_function_label(name);
            }

            return {};
        }

        return m_nameToLabel[name];
    }

    string get_function_label_or_throw(const string &name)
    {
        auto label_opt = get_function_label(name);
        if (!label_opt)
        {
            cout << "\nCompile error: function '" << name << "' not found" << endl;
            throw new runtime_error("Compile error: function '" + name + "' not found");
        }
        return *label_opt;
    }
};

int PseudoAssemblerContext::NEXT_ID = 0;

class PseudoAssemblerVisitor : public ASTNodeVisitor
{
public:
    PseudoAssemblerVisitor(ostream &out = cout) : m_out(out), m_lastLabel(0), m_context(new PseudoAssemblerContext()) {}
    virtual ~PseudoAssemblerVisitor() {}

    virtual void visit(DifferenceLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tCMPEQ" << endl;
        m_out << "\tNOT" << endl;
    }
    virtual void visit(IncrIdentifierNode &node)
    {
        string lblIncr = nextLabel("INCR_");

        string name = ((IdentifierNode &)node.getChild(0)).id();
        int id = m_context->get_id_or_throw(name);

        m_out << lblIncr << ":" << endl;
        m_out << "\tPUSH ID#" << id << endl;
        m_out << "\tPOP tmp" << endl;

        m_out << "\tPUSH tmp" << endl;
        m_out << "\tPUSH 1" << endl;
        m_out << "\tADD" << endl;

        m_out << "\tPOP ID#" << id << endl;
        m_out << "\tPUSH tmp" << endl;
    }
    virtual void visit(RepeatUntilNode &node)
    {
        string lblRepeatBegin = nextLabel("REPEAT_BEGIN_");
        string lblRepeatEnd = nextLabel("REPEAT_END_");

        m_out << lblRepeatBegin << ":" << endl;
        node.getChild(0).accept(*this);

        node.getChild(1).accept(*this);
        m_out << "\tPUSH 0" << endl;
        m_out << "\tCMPEQ" << endl;
        m_out << "\tNOT" << endl;
        m_out << "\tJMPZERO " << lblRepeatBegin << endl;
        m_out << lblRepeatEnd << ":" << endl;
    }
    virtual void visit(AddNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tADD" << endl;
    }
    virtual void visit(AndLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tAND" << endl;
    }
    virtual void visit(AssignmentNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(AssignmentExpressionNode &node)
    {
        node.getChild(1).accept(*this);
        string name = ((IdentifierNode &)node.getChild(0)).id();
        m_out << "\tPOP ID#" << m_context->get_id_or_throw(name) << endl;
    }
    virtual void visit(DivNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tDIV" << endl;
    }
    virtual void visit(EmptyStatementNode &node) {}
    virtual void visit(ExpressionStatementNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(FloatNode &node)
    {
        m_out << "\tPUSH " << (int)node.value() << endl;
    }
    virtual void visit(ForNode &node)
    {
        create_new_context();
        node.getChild(0).accept(*this);
        string lblCond = nextLabel("FOR_COND_");
        string lblEnd = nextLabel("FOR_END_");
        m_out << lblCond << ":" << endl;
        node.getChild(1).accept(*this);
        m_out << "\tJMPZERO " << lblEnd << endl;
        node.getChild(3).accept(*this);
        node.getChild(2).accept(*this);
        m_out << "\tJMP " << lblCond << endl;
        m_out << lblEnd << ":" << endl;
        restore_parent_context();
    }
    virtual void visit(IdentifierExpressionNode &node)
    {
        string name = ((IdentifierNode &)node.getChild(0)).id();
        m_out << "\tPUSH ID#" << m_context->get_id_or_throw(name) << endl;
    }
    virtual void visit(IdentifierNode &node) {}
    virtual void visit(IfElseNode &node)
    {
        node.getChild(0).accept(*this);
        string lblElse = nextLabel("IF_ELSE_");
        string lblEnd = nextLabel("IF_END_");
        m_out << "\tJMPZERO " << lblElse << endl;
        node.getChild(1).accept(*this);
        m_out << "\tJMP " << lblEnd << endl;
        m_out << lblElse << ":" << endl;
        node.getChild(2).accept(*this);
        m_out << lblEnd << ":" << endl;
    }
    virtual void visit(IfNode &node)
    {
        node.getChild(0).accept(*this);
        string lblEnd = nextLabel("IF_END_");
        m_out << "\tJMPZERO " << lblEnd << endl;
        node.getChild(1).accept(*this);
        m_out << "\tJMP " << lblEnd << endl;
        m_out << lblEnd << ":" << endl;
    }
    virtual void visit(IntegerNode &node)
    {
        m_out << "\tPUSH " << (int)node.value() << endl;
    }
    virtual void visit(MulNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tMUL" << endl;
    }
    virtual void visit(NegNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        m_out << "\tNEG" << endl;
    }
    virtual void visit(OrLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tOR" << endl;
    }
    virtual void visit(PostFor &node)
    {
        if (node.numChildren() == 0)
            return;
        node.getChild(0).accept(*this);
        if (node.numChildren() == 1)
            return;
        node.getChild(1).accept(*this);
    }
    virtual void visit(PreFor &node)
    {
        if (node.numChildren() == 0)
            return;
        node.getChild(0).accept(*this);
    }
    virtual void visit(PrintNode &node)
    {
        node.getChild(0).accept(*this);
        m_out << "\tPRINT" << endl;
    }
    virtual void visit(ProgramNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(StatementBlockNode &node)
    {
        create_new_context();
        node.getChild(0).accept(*this);
        restore_parent_context();
    }
    virtual void visit(StatementNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(StatementsNode &node)
    {
        node.getChild(0).accept(*this);
        if (node.numChildren() == 1)
            return;
        node.getChild(1).accept(*this);
    }
    virtual void visit(SubNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tSUB" << endl;
    }
    virtual void visit(BinaryOperatorNode &node)
    {
    }
    virtual void visit(WhileNode &node)
    {
        string lblBegin = nextLabel("WHILE_BEGIN_");
        string lblEnd = nextLabel("WHILE_END_");
        m_out << lblBegin << ":" << endl;
        node.getChild(0).accept(*this);
        m_out << "\tJMPZERO " << lblEnd << endl;
        node.getChild(1).accept(*this);
        m_out << "\tJMP " << lblBegin << endl;
        m_out << lblEnd << ":" << endl;
    }
    virtual void visit(XorLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tXOR" << endl;
    }
    virtual void visit(LessLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tCMPLT" << endl;
    }
    virtual void visit(LessEqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tCMPLE" << endl;
    }
    virtual void visit(GreaterLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tCMPGT" << endl;
    }
    virtual void visit(GreaterEqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tCMPGE" << endl;
    }
    virtual void visit(EqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        node.getChild(1).accept(*this);
        m_out << "\tCMPEQ" << endl;
    }
    virtual void visit(NegationLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        m_out << "\tNOT" << endl;
    }
    virtual void visit(FnDefinitionNode &node)
    {
        string name = ((IdentifierNode &)node.getChild(0)).id();
        string lblBegin = nextLabel(name + "_BEGIN_");
        string lblEnd = nextLabel(name + "_END_");

        m_context->add_function(name, lblBegin);

        create_new_context();
        m_out << "\tJMP " << lblEnd << endl;

        m_out << lblBegin << ":" << endl;

        auto &params = node.getChild(1);

        for (int i = 0; i < params.numChildren(); ++i)
        {
            string name = ((IdentifierNode &)params.getChild(i)).id();
            m_out << "\tPOP ID#" << m_context->add(name) << endl;
        }

        node.getChild(2).accept(*this);

        m_out << "\tRET0" << endl; // in case return statement doesn't appear in function body
        m_out << lblEnd << ":" << endl;
        restore_parent_context();
    }
    virtual void visit(FnCallNode &node)
    {
        string name = ((IdentifierNode &)node.getChild(0)).id();
        auto &args = node.getChild(1);

        for (int i = args.numChildren() - 1; i >= 0; --i)
        {
            args.getChild(i).accept(*this);
        }

        m_out << "\tCALL " << m_context->get_function_label_or_throw(name) << endl;
    }
    virtual void visit(FnParamsNode &node)
    {
    }
    virtual void visit(FnCallArgsNode &node)
    {
    }
    virtual void visit(ReturnStatementNode &node)
    {
        if (node.numChildren() == 1)
        {
            node.getChild(0).accept(*this);
            m_out << "\tRET" << endl;
        }
        else
        {
            m_out << "\tRET0" << endl;
        }
    }
    virtual void visit(VarDeclarationNode &node)
    {
        node.getChild(1).accept(*this);
        string name = ((IdentifierNode &)node.getChild(0)).id();
        m_out << "\tPOP ID#" << m_context->add(name) << endl;
    }

protected:
    PseudoAssemblerContext *m_context;
    ostream &m_out;
    int m_lastLabel;

    string nextLabel(string prefix = "L_")
    {
        ostringstream out;
        out << prefix << ++m_lastLabel;
        return out.str();
    }

    void create_new_context()
    {
        m_context = new PseudoAssemblerContext(m_context);
    }

    void restore_parent_context()
    {
        PseudoAssemblerContext *outer = m_context;
        m_context = m_context->get_parent();
        delete outer;
    }
};