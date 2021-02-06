
#include "../ast/all.hpp"

#include <deque>

using namespace std;

extern FILE *yyout;

class Object
{
public:
    friend ostream &operator<<(ostream &os, Object const &v)
    {
        return os;
    }
};

class Integer : public Object
{
public:
    Integer(int number) : number(number){};
    int getNumber() { return number; }
    void setNumber(int nunber) { this->number = number; }

    friend ostream &operator<<(ostream &os, Integer const &v)
    {
        os << v.number;
        return os;
    }

protected:
    int number;
};

class Array : public Object
{

public:
    Array(){};
    void addValue(int value)
    {
        array.push_back(value);
    }

    void addValueToIndex(int value, int index)
    {
        auto indexPtr = array.begin() + index;
        array.insert(indexPtr, value);
    }

    int getValue(int index)
    {
        return array.at(index);
    }

    friend ostream &operator<<(ostream &os, Array const &v)
    {
        os << "[";
        for (auto it = v.array.begin(); it != v.array.end(); ++it)
        {
            os << *it << ", ";
        }
        os << "]";
        return os;
    }

protected:
    vector<int> array;
};

class InterpreterVisitor : public ASTNodeVisitor
{
public:
    InterpreterVisitor() {}
    virtual ~InterpreterVisitor()
    {
        for (auto it = m_results.begin(); it != m_results.end(); ++it)
        {
            delete *it;
        }
    }

    virtual void visit(AddNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal + rVal));
    }
    virtual void visit(AndLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal && rVal));
    }
    virtual void visit(ArrayNode &node)
    {
        Array *array = new Array();
        for (int i = 0; i < node.numChildren(); ++i)
        {
            node.getChild(i).accept(*this);
            array->addValue(((Integer *)m_results.back())->getNumber());
            m_results.pop_back();
        }
        m_results.push_back(array);
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
            int index = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            node.getChild(2).accept(*this);
            int value = ((Integer *)m_results.back())->getNumber();
            ((Array *)m_idToValue[((IdentifierNode &)node).id()])->addValueToIndex(index, value);
        }
        // basic case: identifier ASSIGN expression
        else
        {
            node.getChild(1).accept(*this);
            m_idToValue[((IdentifierNode &)node.getChild(0)).id()] = m_results.back();
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
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal != rVal));
    }
    virtual void visit(DivNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal / rVal));
    }
    virtual void visit(EmptyStatementNode &node) {}
    virtual void visit(EqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal == rVal));
    }
    virtual void visit(ExpressionStatementNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(FloatNode &node)
    {
        m_results.push_back(new Integer((int)node.value()));
    }
    virtual void visit(ForNode &node)
    {
        node.getChild(0).accept(*this);
        m_results.clear();
        while (true)
        {
            node.getChild(1).accept(*this);
            if (((Integer *)m_results.back())->getNumber() == 0)
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
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal >= rVal));
    }
    virtual void visit(GreaterLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal > rVal));
    }
    virtual void visit(IdentifierArrayNode &node)
    {
        string id = ((IdentifierNode &)node.getChild(0)).id();
        node.getChild(1).accept(*this);
        int index = ((Integer *)m_results.back())->getNumber();
        m_results.push_back(new Integer(((Array *)m_idToValue[id])->getValue(index)));
    }
    virtual void visit(IdentifierExpressionNode &node)
    {
        m_results.push_back(m_idToValue[((IdentifierNode &)node.getChild(0)).id()]);
    }
    virtual void visit(IdentifierNode &node) {}
    virtual void visit(IfElseNode &node)
    {
        node.getChild(0).accept(*this);
        int condVal = ((Integer *)m_results.back())->getNumber();
        m_results.clear();
        if (condVal)
            node.getChild(1).accept(*this);
        else
            node.getChild(2).accept(*this);
    }
    virtual void visit(IfNode &node)
    {
        node.getChild(0).accept(*this);
        int condVal = ((Integer *)m_results.back())->getNumber();
        m_results.clear();
        if (condVal)
            node.getChild(1).accept(*this);
    }
    virtual void visit(IncrIdentifierNode &node)
    {
        int old = ((Integer *)(m_idToValue[((IdentifierNode &)node.getChild(0)).id()]))->getNumber();
        ((Integer *)(m_idToValue[((IdentifierNode &)node.getChild(0)).id()]))->setNumber(old + 1);
        m_results.push_back(new Integer(old));
    }
    virtual void visit(IntegerNode &node)
    {
        m_results.push_back(new Integer((int)node.value()));
    }
    virtual void visit(LessEqualLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal <= rVal));
    }
    virtual void visit(LessLogicalExpression &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal < rVal));
    }
    virtual void visit(MulNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal * rVal));
    }
    virtual void visit(NegationLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(!lVal));
    }
    virtual void visit(NegNumericalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(-lVal));
    }
    virtual void visit(ExpressionsNode &node)
    {
        node.getChild(0).accept(*this);
        if (node.numChildren() == 1)
            return;
        node.getChild(1).accept(*this);
    }
    virtual void visit(OrLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal || rVal));
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
        Object *res = m_results.back();
        cout << *res << endl;
        fprintf(yyout, "%d\n", *res);
        m_results.pop_back();
    }
    virtual void visit(ProgramNode &node)
    {
        node.getChild(0).accept(*this);
    }
    virtual void visit(RepeatUntilNode &node)
    {
        node.getChild(1).accept(*this);
        if (((Integer *)m_results.back())->getNumber() == 1)
            return;
        m_results.pop_back();

        while (true)
        {
            node.getChild(0).accept(*this);
            m_results.pop_back();

            node.getChild(1).accept(*this);
            if (((Integer *)m_results.back())->getNumber() == 1)
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
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal - rVal));
    }
    virtual void visit(WhileNode &node)
    {
        while (true)
        {
            node.getChild(0).accept(*this);
            if (((Integer *)m_results.back())->getNumber() == 0)
                break;
            m_results.clear();
            node.getChild(1).accept(*this);
        }
        m_results.clear();
    }
    virtual void visit(XorLogicalExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        int lVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        node.getChild(1).accept(*this);
        int rVal = ((Integer *)m_results.back())->getNumber();
        m_results.pop_back();
        m_results.push_back(new Integer(lVal ^ rVal));
    }

protected:
    unordered_map<string, Object *> m_idToValue;
    deque<Object *> m_results;
};
