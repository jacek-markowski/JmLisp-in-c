#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
// constructors
lval* lisp_elem();
lval* lisp_empty();
lval* lisp_num (double symbol);
lval* lisp_sym (char* symbol);
lval* lisp_str (char* str);
lval* lisp_builtin (builtin f);
lval* lisp_lambda(lval* args, lval* body);
lval* lisp_err (char* ftm, ...);
lval* lisp_list (void);
lval* lisp_cons (lval* x, lval* y);
int lisp_count(lval* v); 
lval* lisp_index(lval* v, int index);
int lisp_remove_last(lval* v);

