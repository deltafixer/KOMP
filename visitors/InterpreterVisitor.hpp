class Object;

#include "../ast/all.hpp"
#include "Return.hpp"

#include <deque>
#include <optional>

using namespace std;

extern FILE *yyout;

class Object
{
public:
    friend ostream &operator<<(ostream &os, Object const &v)
    {
        return os;
    }
    virtual ~Object() = 0;
};

inline Object::~Object() {}

class Context
{
protected:
    unordered_map<string, Object *> m_idToValue;
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

    optional<Object *> get_value(const string &id)
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

    Object *get_value_or_throw(const string &id)
    {
        auto val = get_value(id);

        if (!val)
        {
            cout << "Variable with identifier '" << id << "' not found" << endl;
            throw new runtime_error("Identifier not found");
        }

        return *val;
    }

    void set_value(const string &id, Object *value)
    {
        m_idToValue[id] = value;
    }

    bool update_value(const string &id, Object *newValue)
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

    void update_value_or_throw(const string &id, Object *newValue)
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

    void addArray(vector<int> array)
    {
        this->array.insert(this->array.end(), array.begin(), array.end());
    }

    void addValueToIndex(int value, int index)
    {
        array[index] = value;
    }

    int getValue(int index)
    {
        return array.at(index);
    }

    vector<int> getArray()
    {
        return this->array;
    }

    int getSize()
    {
        return array.size();
    }

    friend ostream &operator<<(ostream &os, Array const &v)
    {
        os << "[";
        for (int i = 0; i < v.array.size(); ++i)
        {
            os << v.array.at(i);
            if (i != v.array.size() - 1)
            {
                os << ",";
            }
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
    InterpreterVisitor()
    {
        m_context = new Context();
        m_in_function = false;
    }

    virtual ~InterpreterVisitor()
    {
        delete m_context;

        for (auto it = m_results.begin(); it != m_results.end(); ++it)
        {
            delete *it;
        }
    }

    void showAndThrowError(string message)
    {
        cout << "ERROR: " << message << endl;
        throw new runtime_error("ERROR: " + message);
    }

    virtual void visit(AddExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        Array *firstArray = dynamic_cast<Array *>(m_results.back());
        Integer *firstInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();
        node.getChild(1).accept(*this);
        Array *secondArray = dynamic_cast<Array *>(m_results.back());
        Integer *secondInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();

        if (firstArray && secondArray)
        {
            if (firstArray->getSize() != secondArray->getSize())
            {
                showAndThrowError("Arrays are of different lengths");
            }
            Array *res = new Array();
            int arrSize = firstArray->getSize();
            for (int i = 0; i < firstArray->getSize(); ++i)
            {
                res->addValue(firstArray->getValue(i) + secondArray->getValue(i));
            }
            m_results.push_back(res);
        }
        else if (firstInteger && secondInteger)
        {
            int lVal = firstInteger->getNumber();
            int rVal = secondInteger->getNumber();
            m_results.push_back(new Integer(lVal + rVal));
        }
        else
        {
            showAndThrowError("Cannot ADD different types");
        }
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
        string id = ((IdentifierNode &)node.getChild(0)).id();

        // this case is: identifier LSQUAREBR numericalExpression RSQUAREBR ASSIGN numericalExpression
        if (node.numChildren() == 3)
        {
            node.getChild(1).accept(*this);
            int index = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            node.getChild(2).accept(*this);
            int value = ((Integer *)m_results.back())->getNumber();

            Array *arr = (Array *)m_context->get_value_or_throw(id);
            arr->addValueToIndex(value, index);
        }
        // this case is: identifier ASSIGN array | identifier ASSIGN expression
        else
        {
            node.getChild(1).accept(*this);
            m_context->update_value_or_throw(id, m_results.back());
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
    virtual void visit(DivExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        Array *firstArray = dynamic_cast<Array *>(m_results.back());
        Integer *firstInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();
        node.getChild(1).accept(*this);
        Array *secondArray = dynamic_cast<Array *>(m_results.back());
        Integer *secondInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();

        if (firstArray && secondArray)
        {
            if (firstArray->getSize() != secondArray->getSize())
            {
                showAndThrowError("Arrays are of different lengths");
            }
            Array *res = new Array();
            int arrSize = firstArray->getSize();
            for (int i = 0; i < firstArray->getSize(); ++i)
            {
                res->addValue(firstArray->getValue(i) / secondArray->getValue(i));
            }
            m_results.push_back(res);
        }
        else if (firstInteger && secondInteger)
        {
            int lVal = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            node.getChild(1).accept(*this);
            int rVal = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            m_results.push_back(new Integer(lVal / rVal));
        }
        else
        {
            showAndThrowError("Cannot ADD different types");
        }
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
        create_new_context();
        try
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
        catch (Return *r)
        {
            restore_parent_context();
            throw r;
        }
        restore_parent_context();
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
        Array *arr = (Array *)m_context->get_value_or_throw(id);
        m_results.push_back(new Integer(arr->getValue(index)));
    }
    virtual void visit(IdentifierExpressionNode &node)
    {
        string id = ((IdentifierNode &)node.getChild(0)).id();
        m_results.push_back(m_context->get_value_or_throw(id));
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
        string id = ((IdentifierNode &)node.getChild(0)).id();
        Integer *old = (Integer *)m_context->get_value_or_throw(id);
        m_context->update_value(id, new Integer(old->getNumber() + 1));

        m_results.push_back(old);
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
    virtual void visit(MulExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        Array *firstArray = dynamic_cast<Array *>(m_results.back());
        Integer *firstInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();
        node.getChild(1).accept(*this);
        Array *secondArray = dynamic_cast<Array *>(m_results.back());
        Integer *secondInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();

        if (firstArray && secondArray)
        {
            if (firstArray->getSize() != secondArray->getSize())
            {
                showAndThrowError("Arrays are of different lengths");
            }
            Array *res = new Array();
            int arrSize = firstArray->getSize();
            for (int i = 0; i < firstArray->getSize(); ++i)
            {
                res->addValue(firstArray->getValue(i) * secondArray->getValue(i));
            }
            m_results.push_back(res);
        }
        else if (firstInteger && secondInteger)
        {
            int lVal = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            node.getChild(1).accept(*this);
            int rVal = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            m_results.push_back(new Integer(lVal * rVal));
        }
        else
        {
            showAndThrowError("Cannot ADD different types");
        }
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
        Array *arrayRes = dynamic_cast<Array *>(res);

        if (arrayRes)
        {
            cout << *arrayRes << endl;
        }
        else
        {
            cout << *(Integer *)res << endl;
        }

        if (arrayRes)
        {
            fprintf(yyout, "[");
            int n = arrayRes->getSize();
            for (int i = 0; i < n - 1; ++i)
            {
                fprintf(yyout, "%d, ", arrayRes->getValue(i));
            }
            fprintf(yyout, "%d]\n", arrayRes->getValue(n - 1));
        }
        else
        {
            fprintf(yyout, "%d\n", ((Integer *)res)->getNumber());
        }
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
    virtual void visit(SubExpressionNode &node)
    {
        node.getChild(0).accept(*this);
        Array *firstArray = dynamic_cast<Array *>(m_results.back());
        Integer *firstInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();
        node.getChild(1).accept(*this);
        Array *secondArray = dynamic_cast<Array *>(m_results.back());
        Integer *secondInteger = dynamic_cast<Integer *>(m_results.back());
        m_results.pop_back();

        if (firstArray && secondArray)
        {
            if (firstArray->getSize() != secondArray->getSize())
            {
                showAndThrowError("Arrays are of different lengths");
            }
            Array *res = new Array();
            int arrSize = firstArray->getSize();
            for (int i = 0; i < firstArray->getSize(); ++i)
            {
                res->addValue(firstArray->getValue(i) - secondArray->getValue(i));
            }
            m_results.push_back(res);
        }
        else if (firstInteger && secondInteger)
        {
            int lVal = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            node.getChild(1).accept(*this);
            int rVal = ((Integer *)m_results.back())->getNumber();
            m_results.pop_back();
            m_results.push_back(new Integer(lVal - rVal));
        }
        else
        {
            showAndThrowError("Cannot ADD different types");
        }
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

        unordered_map<string, Object *> param_to_arg;

        for (int i = 0; i < params_num; ++i)
        {
            args.getChild(i).accept(*this);
            Object *res = m_results.back();
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
            Object *res = m_results.back();
            m_results.pop_back();
            throw new Return(res);
        }

        throw new Return();
    }
    virtual void visit(VarDeclarationNode &node)
    {
        string id = ((IdentifierNode &)node.getChild(0)).id();
        node.getChild(1).accept(*this);
        Object *res = m_results.back();
        m_results.pop_back();

        m_context->set_value(id, res);
    }

protected:
    Context *m_context;
    deque<Object *> m_results;
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
