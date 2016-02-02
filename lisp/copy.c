#include "headers/lisp.h"

lval* lisp_lval_copy(lval* v) {
  lval* x = lisp_elem(); 
  x->type = v->type;
  x->in_use = 0;
  x->parent = NULL;
  lval* t;
  switch (v->type) {
  case LISP_FUNC:
    x->env = lisp_lenv_copy(v->env);
    x->args = lisp_lval_copy(v->args);
    x->body = lisp_lval_copy(v->body);
    break;
  case LISP_BUILTIN:
    x->fun = v->fun;
    break;
  case LISP_NUM: x->num = v->num; break;
  case LISP_ERR: x->err = malloc(strlen(v->err) + 1);
    strcpy(x->err, v->err);
    break;
  case LISP_STR:
  case LISP_SYM: x->symbol = malloc(strlen(v->symbol) + 1);
    strcpy(x->symbol, v->symbol);
    break;
  case LISP_SEXP:
  case LISP_QEXP:
    x->last = NULL;
    x->root = NULL;
    x->count = v->count;
    x->full = 0;
    t = v->root;
    while(t) {
      lisp_cons(x, lisp_lval_copy(t));
      t = t->next;
    }
    break;
  }
  return x;
}

lenv* lisp_lenv_copy(lenv* e) {
  lenv* n = lisp_lenv("1init1", lisp_list());
  n->parent = e->parent;
  n->in_use = 1;
  while(e != NULL) {
    lisp_lenv_put(n, e->symbol, lisp_lval_copy(e->value));
    e = e->next;
  }
  return n;
}
