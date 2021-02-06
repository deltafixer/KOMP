
#include "../ast/all.hpp"

#include <deque>

using namespace std;

extern FILE *yyout;

class InterpreterVisitor : public ASTNodeVisitor
{
public:
    InterpreterVisitor() {}
    virtual ~InterpreterVisitor() {}

    virtual void visit(AddNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal + rVal});
    }
    virtual void visit(AndLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal && rVal});
    }
    virtual void visit(ArrayNode &node)
    {
        string id = ((IdentifierNode &)node).id() + to_string(m_idToValue.size());
        ((IdentifierNode &)node.getChild(0)).setId(id);
        node.getChild(0).accept(*this);
        m_idToValue[id].push_back(m_results.back()[0]);
        m_results.pop_back();
        if (node.numChildren() == 1)
        {
            return;
        }
        ((IdentifierNode &)node.getChild(1)).setId(id);
        node.getChild(1).accept(*this);
        m_idToValue[id].push_back(m_results.back()[0]);
        m_results.pop_back();
        m_results.push_back(m_idToValue[id]);
    }
    virtual void visit(AssignmentExpressionNode &node)
    {
        // this case is: identifier ASSIGN array
        IdentifierNode *nodeToIdentifierNode = dynamic_cast<IdentifierNode *>(&node.getChild(1));
        if (nodeToIdentifierNode)
        {
            m_idToValue[((IdentifierNode &)node.getChild(0)).id()] = m_idToValue[nodeToIdentifierNode->id()];
            m_idToValue.erase(nodeToIdentifierNode->id());
        }
        // this case is: identifier LSQUAREBR numericalExpression RSQUAREBR ASSIGN numericalExpression
        else if (node.numChildren() == 3)
        {
            node.getChild(1).accept(*this);
            int index = m_results.back()[0];
            m_results.pop_back();
            node.getChild(2).accept(*this);
            int value = m_results.back()[0];
            m_idToValue[((IdentifierNode &)node).id()].at(index) = value;
        }
        // basic case: identifier ASSIGN expression
        else
        {
            node.getChild(1).accept(*this);
            m_idToValue[((IdentifierNode &)node.getChild(0)).id()][0] = m_results.back()[0];
        }
    }
    virtual void visit(AssignmentNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(BinaryOperatorNode &node)
    {
    }
    virtual void visit(DifferenceLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal != rVal});
    }
    virtual void visit(DivNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal / rVal});
    }
    virtual void visit(EmptyStatementNode &node) {}
    virtual void visit(EqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal == rVal});
    }
    virtual void visit(ExpressionStatementNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(FloatNode &node)
    {
        m_results.push_back({(int)node.value()});
    }
    virtual void visit(ForNode &node)
    {
        node.getChild(0).accept(*this);
        m_results.clear();
        while (true)
        {
            node.getChild(1).accept(*this);
            if (m_results.back()[0] == 0)
                break;
            node.getChild(3).accept(*this);
            m_results.clear();
            node.getChild(2).accept(*this);
        }
        m_results.clear();
    }
    virtual void visit(GreaterEqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal >= rVal});
    }
    virtual void visit(GreaterLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal > rVal});
    }
    virtual void visit(IdentifierArrayNode &node)
    {
        string id = ((IdentifierNode &)node.getChild(0)).id();
        node.getChild(1).accept(*this);
        int index = m_results.back()[0];
        m_results.push_back({m_idToValue[id].at(index)});
    }
    virtual void visit(IdentifierExpressionNode &node)
    {
        m_results.push_back(m_idToValue[((IdentifierNode &)node.getChild(0)).id()]);
    }
    virtual void visit(IdentifierNode &node) {}
    virtual void visit(IfElseNode &node)
    {
        node.getChild(0).accept(*this);
        int condVal = m_results.back()[0];
        m_results.clear();
        if (condVal)
            node.getChild(1).accept(*this);
        else
            node.getChild(2).accept(*this);
    }
    virtual void visit(IfNode &node)
    {
        node.getChild(0).accept(*this);
        int condVal = m_results.back()[0];
        m_results.clear();
        if (condVal)
            node.getChild(1).accept(*this);
    }
    virtual void visit(IncrIdentifierNode &node)
    {
        int old = m_idToValue[((IdentifierNode &)node.getChild(0)).id()][0]++;
        m_results.push_back({old});
    }
    virtual void visit(IntegerNode &node)
    {
        m_results.push_back({(int)node.value()});
    }
    virtual void visit(LessEqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal <= rVal});
    }
    virtual void visit(LessLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal < rVal});
    }
    virtual void visit(MulNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal * rVal});
    }
    virtual void visit(NegationLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        m_results[0].push_back(!lVal);
    }
    virtual void visit(NegNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({-lVal});
    }
    virtual void visit(NumericalExpressionsNode &node)
    {
        node.getChild(0).accept(*this);
        if (node.numChildren() == 1)
            return;
        node.getChild(1).accept(*this);
    }
    virtual void visit(OrLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal || rVal});
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
        int res = m_results.back()[0];
        cout << res << endl;
        fprintf(yyout, "%d\n", res);
        m_results.pop_back();
    }
    virtual void visit(ProgramNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(RepeatUntilNode &node)
    {
        node.getChild(1).accept(*this);
        if (m_results.back()[0] == 1)
            return;
        m_results.pop_back();

        while (true)
        {
            node.getChild(0).accept(*this);
            m_results.pop_back();

            node.getChild(1).accept(*this);
            if (m_results.back()[0] == 1)
                break;
            m_results.pop_back();
        }
        m_results.clear();
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
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal - rVal});
    }
    virtual void visit(WhileNode &node)
    {
        while (true)
        {
            node.getChild(0).accept(*this);
            if (m_results.back()[0] == 0)
                break;
            m_results.clear();
            node.getChild(1).accept(*this);
        }
        m_results.clear();
    }
    virtual void visit(XorLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = m_results.back()[0];
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = m_results.back()[0];
        m_results.pop_back();
        m_results.push_back({lVal ^ rVal});
    }

protected:
    unordered_map<string, vector<int>> m_idToValue;
    deque<vector<int>> m_results;
};