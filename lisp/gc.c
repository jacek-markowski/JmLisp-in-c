#include "headers/lisp.h"

gc* gc_new(lval* v) {
  gc* g = malloc(sizeof(gc));
  g->elem = v; 
  g->next = NULL;
  g->prev = NULL;
  return g;
}

int gc_add_value(lval* v) {
  gc* g = JMLisp.gc_root; 
  gc* n = gc_new(v);
  gc* old_next = g->next;
  g->next = n;
  n->prev = g;
  n->next = old_next;
  if(old_next) old_next->prev = n;
  return 0;
}

int gc_pop(gc* g) {
  gc* prev = g->prev;
  gc* next = g->next;
  if(prev) prev->next = next;
  if(next) next->prev = prev;
  if(g == JMLisp.gc_root) JMLisp.gc_root = next;
  return 0;
}


int gc_mark_lval_elems(lval* v, int in_use) {
  v->in_use = in_use;
  switch(v->type) {
  case LISP_FUNC:
    gc_mark_lval_elems(v->args, in_use);
    gc_mark_lval_elems(v->body, in_use);
    gc_mark(v->env, in_use);
    break;
  case LISP_SEXP:
  case LISP_QEXP:
    v = v->root;
    while(v){
      gc_mark_lval_elems(v, in_use);
      v = v->next;
    }
  }
  return 0;
}

int gc_mark(lenv* env, int in_use) {
  env->in_use = in_use;
  while(env != NULL) {
    gc_mark_lval_elems(env->value, in_use);
    env = env->next;
  }
  return 0;
}

int gc_sweep(int now) {
  static int rate = 0;
  if(now) rate = JMLisp.gc_rate;
  if (rate == JMLisp.gc_rate) { rate=0;}
  else { rate++; return 0;}
  DEBUG_PRINT("\n*garbage collector*\n");
  gc* v = JMLisp.gc_root; 
  while(v != NULL) {
    gc* next = v->next;
    if(v->elem->in_use == 0) {
      gc_pop(v);
      lisp_lval_del(v->elem); 
      free(v);
    }
    else {v->elem->in_use = 0;}
    v = next;
  }
  fflush(stdout);
  return 0;
}

int gc_mark_sweep(lenv* env) {
  gc_mark(env, 1);
  gc_sweep(0);
  return 0;
}

int gc_mark_sweep_now(lenv* env) {
  gc_mark(env, 1);
  gc_sweep(1);
  return 0;
}

int gc_stop(lenv* env) {
  gc* v = JMLisp.gc_root;;
  while(v != NULL) {
    lisp_lval_del(v->elem);
    gc* next = v->next;
    free(v);
    v = next;
  }
  lisp_lenv_del(env);
}

int gc_count() {
  int counter = 0;
  gc* v = JMLisp.gc_root; 
  while (v  != NULL) {
    counter++;
    v = v->next;
  }
  printf("\nElements in memory: %d", counter);
  fflush(stdout);
}
