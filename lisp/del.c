#include "headers/lisp.h"

lval* lisp_root(lval* v) {
  while(v->parent) {
    v = v->parent;
  }
  return v;
}

int lisp_lval_del(lval* v) {
  switch (v->type) {
  case LISP_NUM: break;
  case LISP_BUILTIN: break;
  case LISP_FUNC: 
    if(v->env) lisp_lenv_del(v->env);
    break;
  case LISP_ERR: free(v->err); break;
  case LISP_SYM: free(v->symbol); break;
  case LISP_STR: free(v->symbol); break;
  case LISP_QEXP:  break;
  case LISP_SEXP:  break;
  }
  free(v);
}

int lisp_lenv_del(lenv* e) {
  while(e != NULL) {
    lenv* next = e->next;
    free(e->symbol);
    free(e);
    e = next;
  }  
}

int lisp_exp_elems_free(lval* v) {
  free(v);
  v = NULL;
  return 0;
}
