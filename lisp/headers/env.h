#include <stdlib.h>
#include <string.h>
extern lenv* ENVIRONMENT;
lenv*  lisp_lenv (char* sym, lval* val);
int  lisp_lenv_put (lenv* e, char* symbol, lval* value);
lval* lisp_lenv_get (lenv* e, char* symbol);
lval* lisp_lenv_pop (lenv* e, char* symbol);
void  lisp_lenv_add_builtin(lenv* e, char* name, builtin func);
