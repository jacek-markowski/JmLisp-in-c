#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  lisp_lval_del(lval* v);
int  lisp_lenv_del(lenv* e);
int  lisp_exp_elems_free(lval* v);
lval* lisp_root (lval* v);
