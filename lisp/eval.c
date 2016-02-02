#include "headers/lisp.h"

lval* lisp_eval(lenv* e, lval* v) {
  if (v->type == LISP_SYM) {
    lval* x = lisp_lenv_get(e, v->symbol);
    return lisp_lval_copy(x);
  }
  if (v->type == LISP_SEXP) { return lisp_eval_sexp(e, v); }
  return lisp_lval_copy(v);
}

lval* lisp_eval_sexp(lenv* e, lval* v) {
  /* eval all elements */
  lval* t = v->root;
  lval* new = v;
  int set_root = 0;
  GC_COUNT();
  gc_mark_sweep(JMLisp.env);
  GC_COUNT();
  while(t) {
    lval* prev = t->prev;
    lval* next = t->next;
    lval* new_t = lisp_eval(e, t);
    if(prev) prev->next = new_t;
    if(next) next->prev = new_t;
    new_t->prev = prev;
    new_t->next = next;
    t->next = NULL;
    t->prev = NULL;
    t = next;
    if(!set_root) {set_root++; v->root = new_t;}
    if(!next) v->last = new_t;
  }
  
  /* check for errros */
  t = new->root;
  while(t) {
    if(t->type == LISP_ERR) return t;
    t = t->next;
  }

  /* empty s-expression */
  if(new->root == NULL) return new;
  /* one element s-expression */
  if(new->root->next == NULL) return new->root;

  /* first element must be a function */
  lval* f = new->root;
  if (f->type != LISP_FUNC && f->type != LISP_BUILTIN) {
    return lisp_err("First element is not a function. %d", f->type);
  }

  /* apply arguments to function */
  lval* result;
  if(new->root->type == LISP_FUNC) {
    new->root->env->parent = e;
    result = lisp_call(new->root->env, new);
  }
  else result = lisp_call(e, new);
  return result;
}

lval* lisp_call(lenv* e, lval* v) {
  lval* f = v->root;
  /* Builtin */
  if (f->type == LISP_BUILTIN) { return f->fun(e, v); }

  /* user function */
  lval* new = lisp_lval_copy(f);
  new->env->parent= e->parent;
  lval* temp = lisp_lenv_get(JMLisp.env, "1temp1"); 
  lisp_cons(temp, new); 
  lval* args_v = v->root->next;
  lval* args_new = new->args->root;

  /* bind arguments */
  int has_varargs = 0;
  while(args_new && args_v) {
    if(strcmp(args_new->symbol, "&") != 0) {
      lisp_lenv_put(new->env, args_new->symbol, args_v);
      args_v = args_v->next;
      args_new = args_new->next;
    } else {
      has_varargs = 1;
      break;
    }
  }
  if (has_varargs && args_v) {
    args_new = args_new->next; // skip &
    if(args_new) {
      lval* args = lisp_list();
      args->type = LISP_QEXP;
      lisp_lenv_put(new->env, args_new->symbol, args);
      while(args_v) {
  	lisp_cons(args, args_v);
	args_v = args_v->next;
      }
      new->body->type = LISP_SEXP;
      lval* r = lisp_eval(new->env, new->body);
      // for garbage collector */
      lisp_remove_last(temp);
      return r;
    }
    else return lisp_err("Function format invalid.\nSymbol '&' not followed by single symbol.");
  }

  /* To many arguments */
  if(!args_new && args_v) return lisp_err("To many arguments.");

  /* all arguments passed */
  if((!args_new || has_varargs) || !args_v) {
    new->body->type = LISP_SEXP;
    new->env->parent = JMLisp.env;
    lval* r = lisp_eval(new->env, new->body);
    // for garbage collector */
    lisp_remove_last(temp);
    return r;
  }
  /* too few arguments */
  if(args_new && !args_v && !has_varargs) {
    lval* args = lisp_list();
    args->type = LISP_SEXP;
    while(args_new) {
      lisp_cons(args, args_new);
      args_new = args_new->next;
    }
    new->args = args;
    lisp_remove_last(temp);
    return new;
  }
}
