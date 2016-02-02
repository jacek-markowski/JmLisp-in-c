%{
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include "parser.tab.h"
#include "lisp/headers/lisp.h"
%}
%%
[;]+.*[\n]  {}
[a-zA-Z_]+[a-zA-Z0-9_]* { yylval.str = strdup(yytext) ; return SYMBOL; }
\"(\\.|[^\\"])*\"  { 
  int len = strlen(yytext);
  char* p = malloc(sizeof(char) * (len - 1));
  int j = 0;
  /* removing quotes from string */
  for(int i=1; i < len - 1; i++) {
    p[j++] = yytext[i];
  }
  p[len-2] = '\0';
  yylval.str = strdup(p) ; 
  free(p);
  return STRING; 
} 
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
