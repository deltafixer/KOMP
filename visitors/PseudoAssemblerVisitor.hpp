#include "../ast/all.hpp"

#include <deque>
#include <iostream>
#include <sstream>

using namespace std;

class PseudoAssemblerVisitor : public ASTNodeVisitor
{
public:
    PseudoAssemblerVisitor(ostream &out = cout) : m_out(out), m_lastLabel(0) {}
    virtual ~PseudoAssemblerVisitor() {}

    virtual void visit(IncrIdentifierNode &node)
    {
        string lblIncr = nextLabel("INCR_");

        string identifier = ((IdentifierNode &)node.getChild(0)).id();
        if (m_name2id.count(identifier) == 0)
            m_name2id[identifier] = m_name2id.size();

        m_out << lblIncr << ":" << endl;
        m_out << "\tPUSH ID#" << m_name2id[identifier] << endl;
        m_out << "\tPOP tmp" << endl;

        m_out << "\tPUSH tmp" << endl;
        m_out << "\tPUSH 1" << endl;
        m_out << "\tADD" << endl;

        m_out << "\tPOP ID#" << m_name2id[identifier] << endl;
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
    virtual void visit(TripleQuestionNode &node)
    {
        string lblLt = nextLabel("QUESTION_LT_");
        string lblEq = nextLabel("QUESTION_EQ_");
        string lblEnd = nextLabel("QUESTION_END_");
        node.getChild(0).accept(*this);
        m_out << "\tPOP tmp" << endl;
        m_out << "\tPUSH tmp" << endl;
        m_out << "\tPUSH 0" << endl;
        m_out << "\tCMPGE" << endl;
        m_out << "\tJMPZERO " << lblLt << endl;
        m_out << "\tPUSH tmp" << endl;
        m_out << "\tPUSH 0" << endl;
        m_out << "\tCMPEQ" << endl;
        m_out << "\tJMPZERO " << lblEq << endl;
        node.getChild(1).accept(*this);
        m_out << "\tJMP " << lblEnd << endl;
        m_out << lblLt << ":" << endl;
        node.getChild(2).accept(*this);
        m_out << "\tJMP " << lblEnd << endl;
        m_out << lblEq << ":" << endl;
        node.getChild(3).accept(*this);
        m_out << lblEnd << ":" << endl;
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
        string identifier = ((IdentifierNode &)node.getChild(0)).id();
        if (m_name2id.count(identifier) == 0)
            m_name2id[identifier] = m_name2id.size();
        m_out << "\tPOP ID#" << m_name2id[identifier] << endl;
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
    }
    virtual void visit(IdentifierExpressionNode &node)
    {
        string identifier = ((IdentifierNode &)node.getChild(0)).id();
        if (m_name2id.count(identifier) == 0)
            m_name2id[identifier] = m_name2id.size();
        m_out << "\tPUSH ID#" << m_name2id[identifier] << endl;
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
        node.getChild(0).accept(*this);
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

protected:
    unordered_map<string, int> m_name2id;
    ostream &m_out;
    int m_lastLabel;

    string nextLabel(string prefix = "L_")
    {
        ostringstream out;
        out << prefix << ++m_lastLabel;
        return out.str();
    }
};