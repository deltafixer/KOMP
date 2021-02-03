%{

#include <cstdio>

#define YYDEBUG 1

#include "ast/all.hpp"
#include "compiler.y.hpp"

extern void DBG(int, string);

#define RETURN(val, debugMsg) DBG(val, debugMsg); return val;
%}


digit           [0-9]
identifierStart [a-zA-Z_]
identifierChars [a-zA-Z0-9_]

%%

[ \t]           {}

\n              { yylineno++; }

";"             { RETURN (SEMICOL, "SEMICOL"); }
","             { RETURN (COMMA, "COMMA"); }

"{"             { RETURN (LCURLY, "LCURLY"); }
"}"             { RETURN (RCURLY, "RCURLY"); }
"("             { RETURN (LPAREN, "LPAREN"); }
")"             { RETURN (RPAREN, "RPAREN"); }

"++"             { RETURN (INCR, "INCR"); }

"+"             { RETURN (ADD, "ADD"); }
"-"             { RETURN (SUB, "SUB"); }
"*"             { RETURN (MUL, "MUL"); }
"/"             { RETURN (DIV, "DIV"); }

">="             { RETURN (GREQ, "GREQ"); }
"<="             { RETURN (LSEQ, "LSEQ"); }
"=="             { RETURN (EQ, "EQ"); }
">"             { RETURN (GR, "GR"); }
"<"             { RETURN (LS, "LS"); }
"!="             { RETURN (DIFF, "DIFF"); }

"&&"             { RETURN (AND, "AND"); }
"||"             { RETURN (OR, "OR"); }
"^"             { RETURN (XOR, "XOR"); }
"!"             { RETURN (NOT, "NOT"); }

"?"                { RETURN (QUESTION, "QUESTION"); }
":"                { RETURN (COLUMN, "COLUMN"); }

"="             { RETURN (ASSIGN, "ASSIGN"); }

{digit}+        { yylval.intVal = atoi(yytext); RETURN (INTEGER, "INTEGER"); }

{digit}+\.{digit}+  { yylval.floatVal = atof(yytext); RETURN (FLOAT, "FLOAT"); }

(?i:if)            { RETURN (IF, "IF"); }
(?i:else)          { RETURN (ELSE, "ELSE"); }
(?i:for)           { RETURN (FOR, "FOR"); }
(?i:print)         { RETURN (PRINT, "PRINT"); }
(?i:while)         { RETURN (WHILE, "WHILE"); }
(?i:repeat)         { RETURN (REPEAT, "REPEAT"); }
(?i:until)         { RETURN (UNTIL, "UNTIL"); }

{identifierStart}{identifierChars}* { yylval.stringVal = yytext; RETURN (IDENTIFIER, "IDENTIFIER"); }

[\0]                  { RETURN (END, "END"); }

.                   { RETURN (*yytext, "REST"); }

%%

int yywrap() {
    RETURN (1, "");
}