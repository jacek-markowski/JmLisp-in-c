%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lisp/headers/lisp.h"
void yyerror (char const *s);
int yylex();
void call_readline();
lval* parent = NULL;
lval* old_parent = NULL;
%}
%union {
    double n;
    char* str;
    struct lval* a;
}			
%token	<n> OPS CPS OPQ CPQ
%token	<n> EOL NUMBER 
%token  <str> SYMBOL STRING
%type <a> list members s_exp ops qps atom

%%
input:          /* empty */
|	line input 
	;
line:   s_exp {             
	                    JMLisp.root = lisp_root($1);;
			    lval* temp = lisp_lenv_get(JMLisp.env, "1temp1");
			    lisp_cons(temp, JMLisp.root);
			    lval* e = lisp_eval(JMLisp.env, JMLisp.root);
			    if(JMLisp.echo) lisp_print(e);
			    parent = NULL;
			    old_parent = NULL;
			    lisp_lenv_put(JMLisp.env, "1temp1", lisp_list());
			    GC_COUNT(); // only when LISP_DEBUG
			    gc_mark_sweep_now(JMLisp.env);
			    GC_COUNT();
		          }
	;
list:           OPS CPS { $$ = lisp_list(); $$->parent = parent; $$->full = 1;}
	|	OPQ CPQ { $$ = lisp_list(); $$->parent = parent; $$->full = 1;
		    	  $$->type = LISP_QEXP;
			}
	|       ops members CPS { if ($1 != $2) $$ = lisp_cons($1, $2);
				  else $$ = $2;
				  $$->full = 1;
				}
	|	qps members CPQ { if ($1 != $2) $$ = lisp_cons($1, $2);
				  else $$ = $2;
				  $$->full = 1;
				}
	;
ops:		OPS { old_parent = parent;
		      parent = lisp_list();
		      parent->parent = old_parent;
		      $$ = parent;
		    }
	;
qps:		OPQ { old_parent = parent;
		      parent = lisp_list();
		      parent->type = LISP_QEXP;
		      parent->parent = old_parent;
		      $$ = parent;
		    }
        ;
members:	s_exp 
	|	members s_exp { $$ = lisp_cons($1, $2);}
	;
atom:		NUMBER { $$ = lisp_num($1); $$->parent = parent; }
	|	SYMBOL { $$ = lisp_sym($1); $$->parent = parent; free($1); }
	|	STRING { $$ = lisp_str($1); $$->parent = parent; free($1); }
	;
	;
s_exp:   	list 
        |       atom 
	;

%%
void yyerror (char const *s) {
    extern int yylineno;
    printf("Line %d: %s", yylineno, s);
}

void repl() {
    JMLisp.repl = 1;
    while(JMLisp.repl) call_readline(); 
}

int main (int argc, char **argv) {
    extern FILE* yyin;
    int yylex_destroy();
    JMLisp.gc_root = gc_new(lisp_empty()); 
    JMLisp.env = lisp_lenv("1init1", lisp_list());
    JMLisp.gc_rate = 100;
    lisp_lenv_add_builtins(JMLisp.env);
    lisp_lenv_put(JMLisp.env, "1temp1", lisp_list());
    parent = NULL;
    old_parent = NULL;
    JMLisp.echo = 0;
    if (argc == 1) {
	JMLisp.echo = 0;
	yyin = fopen("prelude.jml", "r");
	if(yyin) {yyparse(); fclose(yyin); yylex_destroy();}
	repl();
    }
    else {
	JMLisp.echo = 1;
	yyin = fopen(argv[1], "r");
	yyparse();
	fclose(yyin);
    }
    gc_stop(JMLisp.env);
    printf("\nGoodbye!\n");
    return 1;
}
