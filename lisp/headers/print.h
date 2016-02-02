#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void  lisp_print (lval* v);
void  lisp_print_exp(lval* v, char* open, char* close);
void  lisp_print_lenv(lenv* e);
char* lisp_print_type(int type);
