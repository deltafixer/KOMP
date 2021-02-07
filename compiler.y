%{

#include "ast/all.hpp"
#include "visitors/InterpreterVisitor.hpp"
#include "visitors/PseudoAssemblerVisitor.hpp" 

ASTNode *root;
unordered_map<string, int> varNameToID;

extern int yylex(void);
extern void yyerror(string);
extern int yylineno;
extern FILE *yyin, *yyout;

#define DEBUG

void DBG(int id, string msg) {
    #ifdef DEBUG
    fprintf(stderr, "Line #%d, Rule #%d: %s\n", yylineno, id, msg.c_str());
    #endif
}

%}

%union {
    int     intVal;
    float   floatVal;
    char*   stringVal;
    ASTNode* node;
}

%token<intVal> INTEGER
%token<floatVal> FLOAT
%token<stringVal> IDENTIFIER

%token LPAREN RPAREN LCURLY RCURLY SEMICOL COMMA
%token ADD SUB OR MUL DIV AND XOR NOT INCR
%token EQ GR GREQ LS LSEQ DIFF
%token IF ELSE FOR PRINT WHILE REPEAT UNTIL FN RETURN VAR

%precedence IFX
%precedence ELSE

%left ADD SUB OR XOR
%left MUL DIV AND LS LSEQ GR GREQ
%nonassoc NEG ASSIGN INCR
%left EQ DIFF

%type<node> program statements statement statementBlock simpleStatement expressionStatement expression fnParams fnCallArgs varDeclaration
%type<node> numericalExpression logicalExpression assignmentExpression preFor postFor 
%type<node> identifier constant

%%

program:
            statements {
                root = new ProgramNode($1);
                root->accept(InterpreterVisitor());
                root->accept(PseudoAssemblerVisitor());
                //cout << "PRINTING ROOT:" << endl;
                //root->toStream(cout);
                DBG(1, "program->statements"); 
            }
        ;

statements:
            statements statement { $$ = new StatementsNode($1, $2); DBG(1, "statements->statements statement"); }
        |
            statement { $$ = new StatementsNode($1); DBG(2, "statements->statement "); }
        ;

statement:
            statementBlock  { $$ = new StatementNode($1); DBG(3, "statement->statementBlock"); }
        |
            simpleStatement { $$ = new StatementNode($1); DBG(4, "statement->simpleStatement"); }
        |
            expressionStatement { $$ = new StatementNode($1); DBG(58, "statement->expressionStatement"); }
        ;

statementBlock:
            LCURLY statements RCURLY { $$ = new StatementBlockNode($2); DBG(5, "statementBlock->LCURLY statements RCURLY"); }
        ;

simpleStatement:
            PRINT expressionStatement  { $$ = new PrintNode($2); DBG(6, "simpleStatement->PRINT expressionStatement"); }
        |
            IF LPAREN expression RPAREN statement  %prec IFX              { $$ = new IfNode($3, $5); DBG(7, "simpleStatement->IF expression statement  %prec IFX"); }
        |
            IF LPAREN expression RPAREN statement ELSE statement { $$ = new IfElseNode($3, $5, $7); DBG(8, "simpleStatement->IF LPAREN expression RPAREN statement ELSE statement"); }
        |
            FOR LPAREN preFor SEMICOL expression SEMICOL postFor RPAREN statement { $$ = new ForNode($3, $5, $7, $9); DBG(9, "simpleStatement->IF LPAREN expression RPAREN statement ELSE statement"); }
        |
            WHILE LPAREN expression RPAREN statement { $$ = new WhileNode($3, $5); DBG(10, "simpleStatement->WHILE LPAREN expression RPAREN statement"); }
        |
            REPEAT statement UNTIL logicalExpression SEMICOL { $$ = new RepeatUntilNode($2, $4); DBG(56, "simpleStatement->REPEAT statementBlock UNTIL expression SEMICOL"); }
        |
            RETURN expressionStatement { $$ = new ReturnStatementNode($2); DBG(58, "statement->RETURN expressionStatement"); }
        |
            FN identifier LPAREN fnParams RPAREN LCURLY statements RCURLY { $$ = new FnDefinitionNode($2, $4, $7); DBG(56, "FN identifier LPAREN fnParams RPAREN statementBlock"); }
        |
            varDeclaration SEMICOL {$$ = $1; DBG(57, "simpleStatement->varDeclaration SEMICOL"); }
        |
            assignmentExpression SEMICOL { $$ = new AssignmentNode($1); DBG(11, "simpleStatement->assignmentExpression"); }   
        ;

varDeclaration:
        VAR identifier ASSIGN expression { $$ = new VarDeclarationNode($2, $4); DBG(57, "varDeclaration->VAR identifier ASSIGN expression SEMICOL"); }   

fnParams:
            fnParams COMMA identifier { $$ = new FnParamsNode($1, $3); DBG(38, "fnParams->fnParams COMMA identifier"); }
        |
            identifier                { $$ = new FnParamsNode($1); DBG(38, "fnParams->epsilon"); }
        |
            %empty                    { $$ = new FnParamsNode(); DBG(38, "fnParams->epsilon"); }
        ;

expressionStatement:
            expression SEMICOL  { $$ = new ExpressionStatementNode($1); DBG(12, "expressionStatement->expression SEMICOL"); }
        |
            SEMICOL             { $$ = new EmptyStatementNode(); DBG(13, "expressionStatement->SEMICOL"); }
        ;

expression:
            numericalExpression { $$ = $1; DBG(14, "expression->numericalExpression"); }
        |
            logicalExpression { $$ = $1; DBG(15, "expression->logicalExpression"); }
        ;

numericalExpression:
            numericalExpression ADD numericalExpression { $$ = new AddNumericalExpressionNode($1, $3); DBG(16, "numericalExpression->numericalExpression ADD numericalExpression"); }
        |
            numericalExpression SUB numericalExpression { $$ = new SubNumericalExpressionNode($1, $3); DBG(17, "numericalExpression->numericalExpression SUB numericalExpression"); }
        |
            numericalExpression MUL numericalExpression { $$ = new MulNumericalExpressionNode($1, $3); DBG(18, "numericalExpression->numericalExpression MUL numericalExpression"); }
        |
            numericalExpression DIV numericalExpression { $$ = new DivNumericalExpressionNode($1, $3); DBG(19, "numericalExpression->numericalExpression DIV numericalExpression"); }
        |
            LPAREN numericalExpression RPAREN { $$ = $2; DBG(20, "numericalExpression->LPAREN numericalExpression RPAREN"); }
        |
            SUB numericalExpression %prec NEG { $$ = new NegNumericalExpressionNode($2); DBG(21, "numericalExpression->SUB numericalExpression %%prec NEG"); }
        |
            identifier { $$ = new IdentifierExpressionNode($1); DBG(22, "numericalExpression->identifier"); }
        |
            identifier INCR { $$ = new IncrIdentifierNode($1);  DBG(57, "identifier INCR"); }
        |
            identifier LPAREN fnCallArgs RPAREN { $$ = new FnCallNode($1, $3); DBG(60, "numericalExpression->identifier LPAREN fnCallArgs RPAREN"); }
        |
            constant { $$ = $1;  DBG(37, "numericalExpression->identifier");  }
        ;

identifier: 
            IDENTIFIER { $$ = new IdentifierNode($1); DBG(36, "identifier->IDENTIFIER"); }
        ;

constant:   INTEGER { $$ = new IntegerNode($1); DBG(23, "numericalExpression->INTEGER"); }
        |
            FLOAT { $$ = new FloatNode($1); DBG(24, "numericalExpression->FLOAT"); }
        ;

fnCallArgs:
            fnCallArgs COMMA expression { $$ = new FnCallArgsNode($1, $3); DBG(40, "fnCallArgs->fnCallArgs COMMA expression"); }
        |
            expression { $$ = new FnCallArgsNode($1); DBG(40, "fnCallArgs->expression"); }
        |
            %empty { $$ = new FnCallArgsNode(); DBG(40, "fnCallArgs->epsilon"); }
        ;

logicalExpression:
            logicalExpression AND logicalExpression { $$ = new AndLogicalExpressionNode($1, $3); DBG(25, "a->b"); }
        |
            logicalExpression OR logicalExpression { $$ = new OrLogicalExpressionNode($1, $3);DBG(26, "a->b");}
        |
            logicalExpression XOR logicalExpression { $$ = new XorLogicalExpressionNode($1, $3);DBG(27, "a->b"); }
        |
            NOT logicalExpression %prec NEG { $$ = new NegationLogicalExpressionNode($2); DBG(28, "a->b"); }
        |
            LPAREN logicalExpression RPAREN { $$ = $2; DBG(29, "a->b"); }
        |
            numericalExpression LS numericalExpression { $$ = new LessLogicalExpression($1, $3); DBG(35, "logicalExpression->numericalExpression LS numericalExpression"); }
        |
            numericalExpression LSEQ numericalExpression { $$ = new LessEqualLogicalExpression($1, $3); DBG(35, "logicalExpression->numericalExpression LSEQ numericalExpression"); }
        |
            numericalExpression GR numericalExpression { $$ = new GreaterLogicalExpression($1, $3); DBG(35, "logicalExpression->numericalExpression GR numericalExpression"); }
        |
            numericalExpression GREQ numericalExpression { $$ = new GreaterEqualLogicalExpression($1, $3); DBG(35, "logicalExpression->numericalExpression GREQ numericalExpression"); }
        |
            numericalExpression EQ numericalExpression { $$ = new EqualLogicalExpression($1, $3); DBG(35, "logicalExpression->numericalExpression EQ numericalExpression"); }
        |
            numericalExpression DIFF numericalExpression { $$ = new DifferenceLogicalExpression($1, $3); DBG(35, "logicalExpression->numericalExpression DIFF numericalExpression"); }
        |
            logicalExpression DIFF logicalExpression { $$ = new DifferenceLogicalExpression($1, $3); DBG(35, "logicalExpression->logicalExpression DIFF logicalExpression"); }
        ;

assignmentExpression:
            identifier ASSIGN expression { $$ = new AssignmentExpressionNode($1, $3); DBG(30, "assignmentExpression->identifier ASSIGN expression"); }
        ;

preFor:
            assignmentExpression   { $$ = new PreFor($1); DBG(31, "preFor->assignmentExpression"); }
        | 
            varDeclaration { $$ = new PreFor($1); DBG(31, "preFor->varDeclaration"); }
        ;

postFor:
            assignmentExpression  { $$ = new PostFor($1); DBG(32, "postFor->assignmentExpression"); }
        |
            postFor COMMA assignmentExpression  { $$ = new PostFor($1, $3); DBG(33, "postFor->postFor COMMA assignmentExpression"); }
        |
            %empty  { new PostFor(); DBG(34, "postFor->%empty"); }
        ;

%%

void yyerror(string errMessage) {
    fprintf(stderr, "Error in line %d: %s\n", yylineno, errMessage.c_str());
}

int testCase = 0;

int compare(const char *checkFile, const char *resFile) {
    FILE *fCheck = fopen(checkFile, "r");
    if (fCheck == NULL) { perror("Error opening check file"); return 2; }
    FILE *fRes = fopen(resFile, "r");
    if (fRes == NULL) { perror("Error opening res file"); return 3; }
    while (!feof(fCheck)) {
        char chCheck = fgetc(fCheck);
        if (feof(fRes)) { fprintf(stderr, "res file EOF"); return 4; }
        char chRes = fgetc(fRes);
        if (chRes != chCheck) { fprintf(stderr, "check file is different than res"); return 5; }
    }
    fclose(fCheck);
    fclose(fRes);
    return 0;
}

void test_case(const char *testFile) {
    printf("TEST CASE #%d (%s):\n", ++testCase, testFile);
    yyin = fopen(testFile, "r");
    if (yyin == NULL) perror("yyin is NULL\n");
    const char *tmpFile = "/tmp/compiler.out";
    yyout = fopen(tmpFile, "w");
    yyparse();
    fclose(yyout);
    fclose(yyin);

    char checkFile[10000];
    strcpy(checkFile, testFile);
    strcat(checkFile, ".res");
    if (compare(checkFile, tmpFile)) printf("test #%d: failed!\n", testCase);
    else printf("test #%d: success!\n", testCase);

    yyin = stdin;
    yyout = stdout;
}

int main() {
     char cwd[10000];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n", cwd);
   } else {
       perror("getcwd() error");
       return 1;
   }

    printf("Starting parser...\n");
    yyin = stdin;
    test_case(strcat(cwd, "/test_files/test4.dex"));
    
    //yyparse();

    return 0;
}