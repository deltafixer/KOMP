
#include "../ast/all.hpp"

#include <deque>

using namespace std;

extern FILE *yyout;

class InterpreterVisitor : public ASTNodeVisitor
{
public:
    InterpreterVisitor() {}
    virtual ~InterpreterVisitor() {}

    virtual void visit(DifferenceLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal != rVal);
    }
    virtual void visit(IncrIdentifierNode &node)
    {
        int old = m_idToValue[((IdentifierNode &)node.getChild(0)).id()]++;
        m_results.push_back(old);
    }
    virtual void visit(RepeatUntilNode &node)
    {
        node.getChild(1).accept(*this);
        if (m_results.back() == 1)
            return;
        m_results.pop_back();

        while (true)
        {
            node.getChild(0).accept(*this);
            m_results.pop_back();

            node.getChild(1).accept(*this);
            if (m_results.back() == 1)
                break;
            m_results.pop_back();
        }
        m_results.clear();
    }
    virtual void visit(AddNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal + rVal);
    }
    virtual void visit(AndLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal && rVal);
    }
    virtual void visit(AssignmentNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(AssignmentExpressionNode &node)
    {
        node.getChild(1).accept(*this);
        m_idToValue[((IdentifierNode &)node.getChild(0)).id()] = m_results.back();
    }
    virtual void visit(DivNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal / rVal);
    }
    virtual void visit(EmptyStatementNode &node) {}
    virtual void visit(ExpressionStatementNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(FloatNode &node)
    {
        m_results.push_back((int)node.value());
    }
    virtual void visit(ForNode &node)
    {
        node.getChild(0).accept(*this);
        m_results.clear();
        while (true)
        {
            node.getChild(1).accept(*this);
            if (m_results.back() == 0)
                break;
            node.getChild(3).accept(*this);
            m_results.clear();
            node.getChild(2).accept(*this);
        }
        m_results.clear();
    }
    virtual void visit(IdentifierExpressionNode &node)
    {
        m_results.push_back(m_idToValue[((IdentifierNode &)node.getChild(0)).id()]);
    }
    virtual void visit(IdentifierNode &node) {}
    virtual void visit(IfElseNode &node)
    {
        node.getChild(0).accept(*this);
        int condVal = m_results.back();
        m_results.clear();
        if (condVal)
            node.getChild(1).accept(*this);
        else
            node.getChild(2).accept(*this);
    }
    virtual void visit(IfNode &node)
    {
        node.getChild(0).accept(*this);
        int condVal = m_results.back();
        m_results.clear();
        if (condVal)
            node.getChild(1).accept(*this);
    }
    virtual void visit(IntegerNode &node)
    {
        m_results.push_back((int)node.value());
    }
    virtual void visit(MulNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal * rVal);
    }
    virtual void visit(NegNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(-lVal);
    }
    virtual void visit(OrLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal || rVal);
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
        int res = m_results.back();
        cout << res << endl;
        fprintf(yyout, "%d\n", res);
        m_results.pop_back();
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
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal - rVal);
    }
    virtual void visit(BinaryOperatorNode &node)
    {
    }
    virtual void visit(WhileNode &node)
    {
        while (true)
        {
            node.getChild(0).accept(*this);
            if (m_results.back() == 0)
                break;
            m_results.clear();
            node.getChild(1).accept(*this);
        }
        m_results.clear();
    }
    virtual void visit(XorLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal ^ rVal);
    }
    virtual void visit(LessLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal < rVal);
    }
    virtual void visit(LessEqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal <= rVal);
    }
    virtual void visit(GreaterLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal > rVal);
    }
    virtual void visit(GreaterEqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal >= rVal);
    }
    virtual void visit(EqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(lVal == rVal);
    }
    virtual void visit(NegationLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back();
        m_results.pop_back();
        m_results.push_back(!lVal);
    }
    virtual void visit(FnDefinitionNode &node)
    {
    }
    virtual void visit(FnCallNode &node)
    {
    }
    virtual void visit(FnParamsNode &node)
    {
    }
    virtual void visit(FnCallArgsNode &node)
    {
    }
    virtual void visit(ReturnStatementNode &node)
    {
    }

protected:
    unordered_map<string, int> m_idToValue;
    deque<int> m_results;
};