#include "headers/lisp.h"

void lisp_print(lval *v) {
  switch(v->type) {
  case LISP_NUM: printf("%f", v->num);break;
  case LISP_SYM: printf("%s", v->symbol);break;
  case LISP_STR: printf("%s", v->symbol);break;
  case LISP_ERR: printf("%s", v->err); break;
  case LISP_BUILTIN:  printf("*builtin function*"); break;
  case LISP_FUNC:
    printf("lambda ");
    lisp_print(v->args);
    printf(" -> ");
    lisp_print(v->body);
    break;
  case LISP_SEXP: lisp_print_exp(v, "(", ")");break;
  case LISP_QEXP: lisp_print_exp(v, "{", "}");break;
  }
}

void lisp_print_exp(lval* v, char* open, char* close){
  printf(open);
  lval* t = v->root;
  while(t != NULL) {
    lisp_print(t);
    if(t->next != NULL) putchar(' ');
    t = t->next;
  }
  printf(close);
  fflush(stdout);
} 

void lisp_print_lenv(lenv* e) {
  printf("---syms in env---\n ");
  while(e != NULL) {
    printf("%s:", e->symbol);  
    printf(lisp_print_type(e->value->type));
    printf(" %s", e->value->symbol);
    printf(" %s\n", e->value->env);
    e = e->next;
  }
  printf("-----------------\n");
  fflush(stdout);
} 


char* lisp_print_type(int type) {
  switch (type) {
  case LISP_NUM: return "<number";
  case LISP_STR: return "<string>";
  case LISP_FUNC:return "<function>";
  case LISP_BUILTIN:return "<builtin>";
  case LISP_SYM:return "<symbol>";
  case LISP_ERR: return "<error>";
  case LISP_SEXP: return "<s-expression>";
  case LISP_QEXP: return "<q-expression>";
  default: return "<unknown type>";
  }
}
