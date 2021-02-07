
#include "../ast/all.hpp"
#include "Return.hpp"

#include <deque>
#include <optional>

using namespace std;

extern FILE *yyout;

class Context
{
protected:
    unordered_map<string, int> m_idToValue;
    unordered_map<string, FnDefinitionNode *> m_functions;

    Context *m_parent;

public:
    Context()
    {
        m_parent = nullptr;
    }

    Context(Context *parent)
    {
        this->m_parent = parent;
    }

    Context *get_parent()
    {
        return m_parent;
    }

    optional<int> get_value(const string &id)
    {
        if (m_idToValue.find(id) == m_idToValue.end())
        {
            if (m_parent != nullptr)
            {
                return m_parent->get_value(id);
            }

            return {};
        }

        return m_idToValue[id];
    }

    int get_value_or_throw(const string &id)
    {
        auto val = get_value(id);

        if (!val)
        {
            cout << "Variable with identifier '" << id << "' not found" << endl;
            throw new runtime_error("Identifier not found");
        }

        return *val;
    }

    void set_value(const string &id, int value)
    {
        m_idToValue[id] = value;
    }

    bool update_value(const string &id, int newValue)
    {
        if (m_idToValue.find(id) == m_idToValue.end())
        {
            if (m_parent != nullptr)
            {
                return m_parent->update_value(id, newValue);
            }

            return false;
        }

        m_idToValue[id] = newValue;
        return true;
    }

    void update_value_or_throw(const string &id, int newValue)
    {
        bool exists = update_value(id, newValue);
        if (!exists)
        {
            cout << "Variable with identifier '" << id << "' not found" << endl;
            throw new runtime_error("Identifier not found");
        }
    }

    optional<FnDefinitionNode *> get_function(const string &name)
    {
        if (m_functions.find(name) == m_functions.end())
        {
            if (m_parent != nullptr)
            {
                return m_parent->get_function(name);
            }

            return {};
        }

        return m_functions[name];
    }

    FnDefinitionNode *get_function_or_throw(const string &name)
    {
        optional<FnDefinitionNode *> fn = get_function(name);
        if (!fn)
        {
            cout << "Function with name '" << name << "' not found in current scope" << endl;
            throw new runtime_error("Function with name '" + name + "' not found in current scope");
        }

        return *fn;
    }

    void set_function(const string &name, FnDefinitionNode *fn)
    {
        m_functions[name] = fn;
    }
};

class InterpreterVisitor : public ASTNodeVisitor
{
public:
    InterpreterVisitor()
    {
        m_context = new Context();
        m_in_function = false;
    }

    virtual ~InterpreterVisitor()
    {
        delete m_context;
    }

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
        string id = ((IdentifierNode &)node.getChild(0)).id();
        int old = m_context->get_value_or_throw(id);
        bool exists = m_context->update_value(id, old + 1);

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
        string id = ((IdentifierNode &)node.getChild(0)).id();
        m_context->update_value_or_throw(id, m_results.back());
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
        create_new_context();
        try
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
        catch (Return *r)
        {
            restore_parent_context();
            throw r;
        }
        restore_parent_context();
    }
    virtual void visit(IdentifierExpressionNode &node)
    {
        string id = ((IdentifierNode &)node.getChild(0)).id();
        int val = m_context->get_value_or_throw(id);
        m_results.push_back(val);
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
        create_new_context();
        try
        {
            node.getChild(0).accept(*this);
        }
        catch (Return *ret)
        {
            restore_parent_context();
            throw ret;
        }

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
        string id = ((IdentifierNode &)node.getChild(0)).id();
        m_context->set_function(id, &node);
    }
    virtual void visit(FnCallNode &node)
    {
        bool were_in_function = m_in_function;
        string fnName = ((IdentifierNode &)node.getChild(0)).id();
        FnDefinitionNode *fn = m_context->get_function_or_throw(fnName);

        auto &params = fn->getChild(1);
        auto &args = node.getChild(1);

        int params_num = params.numChildren();
        int args_num = args.numChildren();

        if (params_num != args_num)
        {
            cout << "Function with name " << fnName << " expects " << params_num << " arguments, but " << args_num << " passed" << endl;
            throw new runtime_error("Wrong number of arguments");
        }

        unordered_map<string, int> param_to_arg;

        for (int i = 0; i < params_num; ++i)
        {
            args.getChild(i).accept(*this);
            int res = m_results.back();
            m_results.pop_back();
            string param_id = ((IdentifierNode &)params.getChild(i)).id();
            param_to_arg[param_id] = res;
        }

        create_new_context();
        for (auto it = param_to_arg.begin(); it != param_to_arg.end(); it++)
        {
            m_context->set_value(it->first, it->second);
        }

        m_in_function = true;
        try
        {
            fn->getChild(2).accept(*this);
        }
        catch (Return *ret)
        {
            if (ret->result)
            {
                m_results.push_back(*(ret->result));
            }
        }
        restore_parent_context();
        m_in_function = were_in_function;
    }
    virtual void visit(FnParamsNode &node)
    {
    }
    virtual void visit(FnCallArgsNode &node)
    {
    }
    virtual void visit(ReturnStatementNode &node)
    {
        if (!m_in_function)
        {
            cout << "Return must be used only inside a function" << endl;
            throw new runtime_error("Return must be used only inside a function");
        }

        if (node.numChildren() == 1)
        {
            node.getChild(0).accept(*this);
            int res = m_results.back();
            m_results.pop_back();
            throw new Return(res);
        }

        throw new Return();
    }
    virtual void visit(VarDeclarationNode &node)
    {
        string id = ((IdentifierNode &)node.getChild(0)).id();
        node.getChild(1).accept(*this);
        int res = m_results.back();
        m_results.pop_back();

        m_context->set_value(id, res);
    }

protected:
    Context *m_context;
    deque<int> m_results;
    bool m_in_function;

    void create_new_context()
    {
        m_context = new Context(m_context);
    }

    void restore_parent_context()
    {
        Context *outer = m_context;
        m_context = m_context->get_parent();
        delete outer;
    }
};