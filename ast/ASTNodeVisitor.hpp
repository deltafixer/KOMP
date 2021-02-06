#pragma once

class AddNumericalExpressionNode;
class AndLogicalExpressionNode;
class IdentifierNode;
class ArrayNode;
class AssignmentExpressionNode;
class AssignmentNode;
class ASTNode;
class ASTNodeVisitor;
class BinaryOperatorNode;
class DifferenceLogicalExpression;
class DivNumericalExpressionNode;
class EmptyStatementNode;
class EqualLogicalExpression;
class ExpressionStatementNode;
class FloatNode;
class ForNode;
class GreaterEqualLogicalExpression;
class GreaterLogicalExpression;
class IdentifierArrayNode;
class IdentifierExpressionNode;
class IfElseNode;
class IfNode;
class IncrIdentifierNode;
class IntegerNode;
class LessEqualLogicalExpression;
class LessLogicalExpression;
class MulNumericalExpressionNode;
class NegationLogicalExpressionNode;
class NegNumericalExpressionNode;
class ExpressionsNode;
class OrLogicalExpressionNode;
class PostFor;
class PreFor;
class PrintNode;
class ProgramNode;
class RepeatUntilNode;
class StatementBlockNode;
class StatementNode;
class StatementsNode;
class SubNumericalExpressionNode;
class WhileNode;
class XorLogicalExpressionNode;

class ASTNodeVisitor
{
public:
    ASTNodeVisitor() {}
    virtual ~ASTNodeVisitor() {}

    virtual void visit(AddNumericalExpressionNode &node) = 0;
    virtual void visit(AndLogicalExpressionNode &node) = 0;
    virtual void visit(ArrayNode &node) = 0;
    virtual void visit(AssignmentExpressionNode &node) = 0;
    virtual void visit(AssignmentNode &node) = 0;
    virtual void visit(BinaryOperatorNode &node) = 0;
    virtual void visit(DifferenceLogicalExpression &node) = 0;
    virtual void visit(DivNumericalExpressionNode &node) = 0;
    virtual void visit(EmptyStatementNode &node) = 0;
    virtual void visit(EqualLogicalExpression &node) = 0;
    virtual void visit(ExpressionStatementNode &node) = 0;
    virtual void visit(FloatNode &node) = 0;
    virtual void visit(ForNode &node) = 0;
    virtual void visit(GreaterEqualLogicalExpression &node) = 0;
    virtual void visit(GreaterLogicalExpression &node) = 0;
    virtual void visit(IdentifierArrayNode &node) = 0;
    virtual void visit(IdentifierExpressionNode &node) = 0;
    virtual void visit(IdentifierNode &node) = 0;
    virtual void visit(IfElseNode &node) = 0;
    virtual void visit(IfNode &node) = 0;
    virtual void visit(IncrIdentifierNode &node) = 0;
    virtual void visit(IntegerNode &node) = 0;
    virtual void visit(LessEqualLogicalExpression &node) = 0;
    virtual void visit(LessLogicalExpression &node) = 0;
    virtual void visit(MulNumericalExpressionNode &node) = 0;
    virtual void visit(NegationLogicalExpressionNode &node) = 0;
    virtual void visit(NegNumericalExpressionNode &node) = 0;
    virtual void visit(ExpressionsNode &node) = 0;
    virtual void visit(OrLogicalExpressionNode &node) = 0;
    virtual void visit(PostFor &node) = 0;
    virtual void visit(PreFor &node) = 0;
    virtual void visit(PrintNode &node) = 0;
    virtual void visit(ProgramNode &node) = 0;
    virtual void visit(RepeatUntilNode &node) = 0;
    virtual void visit(StatementBlockNode &node) = 0;
    virtual void visit(StatementNode &node) = 0;
    virtual void visit(StatementsNode &node) = 0;
    virtual void visit(SubNumericalExpressionNode &node) = 0;
    virtual void visit(WhileNode &node) = 0;
    virtual void visit(XorLogicalExpressionNode &node) = 0;
};