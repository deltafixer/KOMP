#pragma once

class AddNumericalExpressionNode;
class AndLogicalExpressionNode;
class AssignmentNode;
class AssignmentExpressionNode;
class ASTNode;
class ASTNodeVisitor;
class BinaryOperatorNode;
class DivNumericalExpressionNode;
class EmptyStatementNode;
class ExpressionStatementNode;
class FloatNode;
class ForNode;
class IdentifierExpressionNode;
class IdentifierNode;
class IfElseNode;
class IfNode;
class IntegerNode;
class MulNumericalExpressionNode;
class NegNumericalExpressionNode;
class OrLogicalExpressionNode;
class PostFor;
class PreFor;
class PrintNode;
class ProgramNode;
class StatementBlockNode;
class StatementNode;
class StatementsNode;
class SubNumericalExpressionNode;
class WhileNode;
class XorLogicalExpressionNode;
class LessLogicalExpression;
class LessEqualLogicalExpression;
class GreaterLogicalExpression;
class GreaterEqualLogicalExpression;
class EqualLogicalExpression;
class NegationLogicalExpressionNode;
class RepeatUntilNode;
class IncrIdentifierNode;
class DifferenceLogicalExpression;
class FnDefinitionNode;
class FnCallNode;
class FnParamsNode;
class FnCallArgsNode;
class ReturnStatementNode;
class VarDeclarationNode;

class ASTNodeVisitor
{
public:
    ASTNodeVisitor() {}
    virtual ~ASTNodeVisitor() {}

    virtual void visit(AddNumericalExpressionNode &node) = 0;
    virtual void visit(AndLogicalExpressionNode &node) = 0;
    virtual void visit(AssignmentNode &node) = 0;
    virtual void visit(AssignmentExpressionNode &node) = 0;
    virtual void visit(BinaryOperatorNode &node) = 0;
    virtual void visit(DivNumericalExpressionNode &node) = 0;
    virtual void visit(EmptyStatementNode &node) = 0;
    virtual void visit(ExpressionStatementNode &node) = 0;
    virtual void visit(FloatNode &node) = 0;
    virtual void visit(ForNode &node) = 0;
    virtual void visit(IdentifierExpressionNode &node) = 0;
    virtual void visit(IdentifierNode &node) = 0;
    virtual void visit(IfElseNode &node) = 0;
    virtual void visit(IfNode &node) = 0;
    virtual void visit(IntegerNode &node) = 0;
    virtual void visit(MulNumericalExpressionNode &node) = 0;
    virtual void visit(NegNumericalExpressionNode &node) = 0;
    virtual void visit(OrLogicalExpressionNode &node) = 0;
    virtual void visit(PostFor &node) = 0;
    virtual void visit(PreFor &node) = 0;
    virtual void visit(PrintNode &node) = 0;
    virtual void visit(ProgramNode &node) = 0;
    virtual void visit(StatementBlockNode &node) = 0;
    virtual void visit(StatementNode &node) = 0;
    virtual void visit(StatementsNode &node) = 0;
    virtual void visit(SubNumericalExpressionNode &node) = 0;
    virtual void visit(WhileNode &node) = 0;
    virtual void visit(XorLogicalExpressionNode &node) = 0;
    virtual void visit(LessLogicalExpression &node) = 0;
    virtual void visit(LessEqualLogicalExpression &node) = 0;
    virtual void visit(GreaterLogicalExpression &node) = 0;
    virtual void visit(GreaterEqualLogicalExpression &node) = 0;
    virtual void visit(EqualLogicalExpression &node) = 0;
    virtual void visit(NegationLogicalExpressionNode &node) = 0;
    virtual void visit(RepeatUntilNode &node) = 0;
    virtual void visit(IncrIdentifierNode &node) = 0;
    virtual void visit(DifferenceLogicalExpression &node) = 0;
    virtual void visit(FnDefinitionNode &node) = 0;
    virtual void visit(FnCallNode &node) = 0;
    virtual void visit(FnParamsNode &node) = 0;
    virtual void visit(FnCallArgsNode &node) = 0;
    virtual void visit(ReturnStatementNode &node) = 0;
    virtual void visit(VarDeclarationNode &node) = 0;
};