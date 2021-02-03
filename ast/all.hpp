#include <cstdio>
#include <string>
#include <unistd.h>
#include <cstring>

#include <iostream>
#include <unordered_map> 

using namespace std;

#include "AddNumericalExpressionNode.hpp"
#include "AndLogicalExpressionNode.hpp"
#include "AssignmentNode.hpp"
#include "AssignmentExpressionNode.hpp"
#include "ASTNode.hpp"
#include "ASTNodeVisitor.hpp"
#include "BinaryOperatorNode.hpp"
#include "DivNumericalExpressionNode.hpp"
#include "EmptyStatementNode.hpp"
#include "ExpressionStatementNode.hpp"
#include "FloatNode.hpp"
#include "ForNode.hpp"
#include "IdentifierExpressionNode.hpp"
#include "IdentifierNode.hpp"
#include "IfElseNode.hpp"
#include "IfNode.hpp"
#include "IntegerNode.hpp"
#include "MulNumericalExpressionNode.hpp"
#include "NegNumericalExpressionNode.hpp"
#include "OrLogicalExpressionNode.hpp"
#include "PostFor.hpp"
#include "PreFor.hpp"
#include "PrintNode.hpp"
#include "ProgramNode.hpp"
#include "StatementBlockNode.hpp"
#include "StatementNode.hpp"
#include "StatementsNode.hpp"
#include "SubNumericalExpressionNode.hpp"
#include "WhileNode.hpp"
#include "XorLogicalExpressionNode.hpp"
#include "LessLogicalExpression.hpp"
#include "LessEqualLogicalExpression.hpp"
#include "GreaterLogicalExpression.hpp"
#include "GreaterEqualLogicalExpression.hpp"
#include "EqualLogicalExpression.hpp"
#include "NegationLogicalExpressionNode.hpp"