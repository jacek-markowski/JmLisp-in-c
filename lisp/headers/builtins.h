#include <string.h>


lval* builtin_op(lenv* e, lval* a, char* op);
lval* builtin_cmp(lenv* e, lval* a, char* op);
lval* builtin_ord(lenv* e, lval* a, char* op);
int  lval_eq(lval* x, lval* y);
lval* builtin_add(lenv* e, lval* a);
lval* builtin_mult(lenv* e, lval* a);
lval* builtin_subt(lenv* e, lval* a);
lval* builtin_div(lenv* e, lval* a);
lval* builtin_typeof(lenv* e, lval* a);
lval* builtin_print(lenv* e, lval* a);
lval* builtin_eval(lenv* e, lval* a);
lval* builtin_def(lenv* e, lval* a);
lval* builtin_del(lenv* e, lval* a);
lval* builtin_lambda(lenv* e, lval* a);
lval* builtin_let(lenv* e, lval* a);
lval* builtin_car(lenv* e, lval* a);
lval* builtin_cdr(lenv* e, lval* a);
lval* builtin_append(lenv* e, lval* a);
lval* builtin_prepend(lenv* e, lval* a);
lval* builtin_join(lenv* e, lval* a);
lval* builtin_list(lenv* e, lval* a);
lval* builtin_load(lenv* e, lval* a);
lval* builtin_error(lenv* e, lval* a);

lval* builtin_if(lenv* e, lval* a);
lval* builtin_eq(lenv* e, lval* a);
lval* builtin_ne(lenv* e, lval* a);
lval* builtin_gt(lenv* e, lval* a);
lval* builtin_lt(lenv* e, lval* a);
lval* builtin_ge(lenv* e, lval* a);
lval* builtin_le(lenv* e, lval* a);
void  lisp_lenv_add_builtins(lenv* e);
