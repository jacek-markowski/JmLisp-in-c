#include "headers/lisp.h"

lval* lisp_elem() {
  lval* a = lisp_empty(); 
  gc_add_value(a);
  return a;
}

lval* lisp_empty() {
  lval* a = malloc(sizeof(lval));
  a->type = -1;
  a->in_use = 0;
  a->full = 1;
  a->next = NULL;
  a->prev = NULL;
  a->parent = NULL;
  return a;
}

lval* lisp_num (double num){
  lval* a = lisp_elem(); 
  a->type = LISP_NUM;
  a->num = num;
  return a;
}

lval* lisp_sym (char* symbol){
  lval* a = lisp_elem(); 
  a->type = LISP_SYM;
  a->symbol = malloc(strlen(symbol) + 1);
  strcpy(a->symbol, symbol);;
  return a;
}

lval* lisp_builtin(builtin f) {
  lval* a = lisp_elem(); 
  a->type = LISP_BUILTIN;
  a->fun = f;
  return a;
}

lval* lisp_lambda(lval* args, lval* body) {
  lval* a = lisp_elem(); 
  a->type = LISP_FUNC;
  a->args = args;
  a->body = body;
  a->env = lisp_lenv("1init1", lisp_list());
  return a;
}

lval* lisp_str (char* symbol){
  lval* a = lisp_elem();
  a->type = LISP_STR;
  a->symbol = malloc(strlen(symbol) + 1);
  strcpy(a->symbol, symbol);;
  return a;
}

lval* lisp_list (){
  lval* ls = lisp_elem();
  ls->type = LISP_SEXP;
  ls->count = 0;
  ls->last = NULL;
  ls->root = NULL;
  ls->full = 0;
  return ls;
}

lval* lisp_err(char* fmt, ...) {
  lval* a = lisp_elem(); 
  a->type = LISP_ERR;
  va_list va;
  va_start(va, fmt);  
  a->err = malloc(512);
  vsnprintf(a->err, 511, fmt, va);  
  va_end(va);  
  return a;
}

lval* lisp_cons (lval *x, lval *y) {;
  if(x->type == LISP_NUM || x->type == LISP_SYM) lisp_cons(x->parent, x);
  if(x->full) {
    if (x->type == LISP_SEXP || x->type == LISP_QEXP) lisp_cons(x->parent, x);
    return lisp_cons(x->parent, y);
  }
  if(x->last == NULL) { x->last = y; x->root = y; x->count = 1;}
  else {
    y->prev = x->last;
    x->last->next = y;
    x->last = y;
    x->count+= 1;}
  return x;
}


int lisp_remove_last(lval* v) {
  lval* prev = v->last->prev;
  prev->next = NULL;
  v->last = prev;
  v->count--;
  return 0;
}

int lisp_count(lval* v) {
  int c = 0;
  lval* t = v->root;
  while(t) {
    c++;
    t = t->next;
  }
  return c;
}

lval* lisp_index(lval* v, int index) {
  lval* t = v->root;
  while(index) {
    index--;
    t = t->next;
  }
  return t;

}
