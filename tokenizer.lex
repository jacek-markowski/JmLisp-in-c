%{
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include "parser.tab.h"
#include "lisp/headers/lisp.h"
 char buf[100];
 char *s;
%}
%x STRING_
%%
[;]+.*[\n]  {}
[a-zA-Z_]+[a-zA-Z0-9_]* { yylval.str = strdup(yytext) ; return SYMBOL; }

\"              { BEGIN STRING_; s = buf; }
<STRING_>\\n     { *s++ = '\n'; }
<STRING_>\\t     { *s++ = '\t'; }
<STRING_>\\\"    { *s++ = '\"'; }
<STRING_>\"      {
  *s = 0;
  BEGIN 0;
  yylval.str = strdup(buf);
  return STRING;
 }
<STRING_>.       { *s++ = *yytext; }


[-+]?[0-9]+([.]?[0-9]*)? { yylval.n = atof(yytext); return NUMBER;}
"+" { yylval.str = strdup(yytext); return SYMBOL; }
"\\" { yylval.str = strdup(yytext); return SYMBOL; }
"*" { yylval.str = strdup(yytext); return SYMBOL; }
"&" { yylval.str = strdup(yytext); return SYMBOL; }
"-" { yylval.str = strdup(yytext); return SYMBOL; }
"/" { yylval.str = strdup(yytext); return SYMBOL; }
">" { yylval.str = strdup(yytext); return SYMBOL; }
"<" { yylval.str = strdup(yytext); return SYMBOL; }
">=" { yylval.str = strdup(yytext); return SYMBOL; }
"<=" { yylval.str = strdup(yytext); return SYMBOL; }
"==" { yylval.str = strdup(yytext); return SYMBOL; }
"!=" { yylval.str = strdup(yytext); return SYMBOL; }
"(" { return OPS; }
"{" { return OPQ; }
")" { return CPS; }
"}" { return CPQ; }
[\s\t]* {}
. {}
%%

void call_readline()
{
  YY_BUFFER_STATE bp;
  char *f = readline("*JMLisp*> ");
  JMLisp.echo = 1;
  if (f) { 
    if(strlen(f)) {
      add_history(f);
      bp = yy_scan_string(f);
      yy_switch_to_buffer(bp);
      yyparse();  /* eat the input */
      yy_delete_buffer(bp);
      printf("\n");
    }
    free(f);
  }
  else {
    free(f);
    JMLisp.repl = 0;
  }
  yylex_destroy();
}
