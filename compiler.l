%{

#include <cstdio>

#define YYDEBUG 1

#include "ast/all.hpp"
#include "compiler.y.hpp"

extern void DBG(int, string);

#define RETURN_TOKEN(val, debugMsg) DBG(val, debugMsg); return val;
%}


digit           [0-9]
identifierStart [a-zA-Z_]
identifierChars [a-zA-Z0-9_]

%%

[ \t]           {}

\n              { yylineno++; }

";"             { RETURN_TOKEN (SEMICOL, "SEMICOL"); }
","             { RETURN_TOKEN (COMMA, "COMMA"); }

"{"             { RETURN_TOKEN (LCURLY, "LCURLY"); }
"}"             { RETURN_TOKEN (RCURLY, "RCURLY"); }
"("             { RETURN_TOKEN (LPAREN, "LPAREN"); }
")"             { RETURN_TOKEN (RPAREN, "RPAREN"); }

"++"             { RETURN_TOKEN (INCR, "INCR"); }

"+"             { RETURN_TOKEN (ADD, "ADD"); }
"-"             { RETURN_TOKEN (SUB, "SUB"); }
"*"             { RETURN_TOKEN (MUL, "MUL"); }
"/"             { RETURN_TOKEN (DIV, "DIV"); }

">="             { RETURN_TOKEN (GREQ, "GREQ"); }
"<="             { RETURN_TOKEN (LSEQ, "LSEQ"); }
"=="             { RETURN_TOKEN (EQ, "EQ"); }
">"             { RETURN_TOKEN (GR, "GR"); }
"<"             { RETURN_TOKEN (LS, "LS"); }
"!="             { RETURN_TOKEN (DIFF, "DIFF"); }

"&&"             { RETURN_TOKEN (AND, "AND"); }
"||"             { RETURN_TOKEN (OR, "OR"); }
"^"             { RETURN_TOKEN (XOR, "XOR"); }
"!"             { RETURN_TOKEN (NOT, "NOT"); }

"="             { RETURN_TOKEN (ASSIGN, "ASSIGN"); }

{digit}+        { yylval.intVal = atoi(yytext); RETURN_TOKEN (INTEGER, "INTEGER"); }

{digit}+\.{digit}+  { yylval.floatVal = atof(yytext); RETURN_TOKEN (FLOAT, "FLOAT"); }

(?i:if)            { RETURN_TOKEN (IF, "IF"); }
(?i:else)          { RETURN_TOKEN (ELSE, "ELSE"); }
(?i:for)           { RETURN_TOKEN (FOR, "FOR"); }
(?i:print)         { RETURN_TOKEN (PRINT, "PRINT"); }
(?i:while)         { RETURN_TOKEN (WHILE, "WHILE"); }
(?i:repeat)        { RETURN_TOKEN (REPEAT, "REPEAT"); }
(?i:until)         { RETURN_TOKEN (UNTIL, "UNTIL"); }
(?i:fn)            { RETURN_TOKEN (FN, "FN"); }
(?i:return)        { RETURN_TOKEN (RETURN, "RETURN"); }
(?i:var)           { RETURN_TOKEN (VAR, "VAR"); }

{identifierStart}{identifierChars}* { yylval.stringVal = yytext; RETURN_TOKEN (IDENTIFIER, "IDENTIFIER"); }

[\0]                {}

.                   { RETURN_TOKEN (*yytext, "REST"); }

%%

int yywrap() {
    RETURN_TOKEN (1, "");
}