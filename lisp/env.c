#include "headers/lisp.h"

lenv* lisp_lenv(char* symbol, lval* value) {
  lenv* e = malloc(sizeof(lenv));
  e->parent = NULL;
  e->next = NULL;
  e->prev = NULL;
  e->in_use = 1;
  e->symbol = malloc((strlen(symbol) + 1) * sizeof(char));
  strcpy(e->symbol, symbol);
  e->value = value; 
  return e;
}

int lisp_lenv_put(lenv* e, char* sym, lval* val){; 
  lenv* t = e;
  while(t != NULL) {
    if(strcmp(t->symbol, sym) == 0) {
      t->value = val;
      return 0;
    }
    t = t->next;
  }
  lenv* new = lisp_lenv(sym, val);
  lenv* old_next = e->next;
  e->next = new;
  new->prev = e;
  new->next = old_next;
  if(old_next) old_next->prev = new;
  return 0;
}

lval* lisp_lenv_get(lenv* e, char* sym) {
  lenv* t = e;
  while(t != NULL) {
    if(strcmp(t->symbol, sym) == 0) return t->value;
    t = t->next;
  }
  if(e->parent != NULL) return lisp_lenv_get(e->parent, sym);
  return lisp_err("Symbol %s not defined", sym);
}

lval* lisp_lenv_pop(lenv* e, char* sym) {
  lenv* to_del = e;
  lval* val;
  while(e != NULL) {
    if(strcmp(e->symbol, sym) == 0) {
      val = e->value;
      break;
    }
    e = e->next;
  }
  lenv* prev = e->prev;
  lenv* next = e->next;
  if(prev) prev->next = next;
  if(next) next->prev = prev;
  if(to_del == e) return lisp_err("Can't remove memory element");
  free(e->symbol);
  free(e);
  return val;
}

void  lisp_lenv_add_builtin(lenv* e, char* name, builtin func) {
  lval* f = lisp_builtin(func);
  f->in_use = 1;
  lisp_lenv_put(e, name, f);
}
