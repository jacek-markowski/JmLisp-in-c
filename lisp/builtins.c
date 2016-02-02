#include "headers/lisp.h"

void lisp_lenv_add_builtins(lenv* e) {
  lisp_lenv_add_builtin(e, "+", builtin_add);
  lisp_lenv_add_builtin(e, "-", builtin_subt);
  lisp_lenv_add_builtin(e, "*", builtin_mult);
  lisp_lenv_add_builtin(e, "/", builtin_div);
  lisp_lenv_add_builtin(e, "typeof", builtin_typeof);
  lisp_lenv_add_builtin(e, "print", builtin_print);
  lisp_lenv_add_builtin(e, "lambda", builtin_lambda);
  lisp_lenv_add_builtin(e, "def", builtin_def);
  lisp_lenv_add_builtin(e, "eval", builtin_eval);
  lisp_lenv_add_builtin(e, "let", builtin_let);
  lisp_lenv_add_builtin(e, "del", builtin_del);
  lisp_lenv_add_builtin(e, "car", builtin_car);
  lisp_lenv_add_builtin(e, "cdr", builtin_cdr);
  lisp_lenv_add_builtin(e, "append", builtin_append);
  lisp_lenv_add_builtin(e, "prepend", builtin_prepend);
  lisp_lenv_add_builtin(e, "join", builtin_join);
  lisp_lenv_add_builtin(e, "list", builtin_list);
  lisp_lenv_add_builtin(e, "load", builtin_load);
  lisp_lenv_add_builtin(e, "error", builtin_error);
  lisp_lenv_add_builtin(e, "if", builtin_if);
  lisp_lenv_add_builtin(e, "do", builtin_do);
  lisp_lenv_add_builtin(e, "==", builtin_eq);
  lisp_lenv_add_builtin(e, "!=", builtin_ne);
  lisp_lenv_add_builtin(e, ">", builtin_gt);
  lisp_lenv_add_builtin(e, "<", builtin_lt);
  lisp_lenv_add_builtin(e, ">=", builtin_ge);
  lisp_lenv_add_builtin(e, "<=", builtin_le);
}

lval* builtin_do(lenv* e, lval* a) {
  LASSERT_TYPE("do", lisp_index(a, 1), 1, LISP_QEXP);
  lval* t = a->root->next;
  lval* last;
  lenv* new_env = lisp_lenv("1null1", lisp_list());
  new_env->parent = e;
  while(t) {
    t->type = LISP_SEXP;
    last = lisp_eval(new_env, t);
    t = t->next;
  }
  return last;
}

lval* builtin_op(lenv* e, lval* a, char* op) {
  lval* t = a->root->next;
  while(t) {
    if(t->type != LISP_NUM) {
      return lisp_err("Function %s accepts only num args!", op);
    }
    t = t->next;
  }

  double r = a->root->next->num;
  if (a->root->next->next == NULL && (strcmp(op, "-") == 0)){
    r = r * (-1);
  }
  double y;
  t = a->root->next->next;
  while(t) {
    y = t->num;
    if (strcmp(op, "+") == 0) { r += y; }
    if (strcmp(op, "-") == 0) { r -= y; }
    if (strcmp(op, "*") == 0) { r *= y; }
    if (strcmp(op, "/") == 0) {
      if (y) r /= y;
      else return lisp_err("Dividing By Zero!");
    }
    t = t->next;

  }
  return lisp_num(r);
}

lval* builtin_add(lenv* e, lval* a) {
  return builtin_op(e, a, "+");
}

lval* builtin_mult(lenv* e, lval* a) {
  return builtin_op(e, a, "*");
}

lval* builtin_subt(lenv* e, lval* a) {
  return builtin_op(e, a, "-");
}

lval* builtin_div(lenv* e, lval* a) {
  return builtin_op(e, a, "/");
}

lval* builtin_lambda(lenv* e, lval* a ) {
  LASSERT_NUM("lambda", a->count, 3);
  LASSERT_TYPE("lambda", lisp_index(a, 1), 1, LISP_QEXP);
  LASSERT_TYPE("lambda", lisp_index(a, 2), 2, LISP_QEXP);
  lval* args = a->root->next;
  lval* body = a->root->next->next;
  return lisp_lambda(args, body);
}

lval* builtin_def(lenv* e, lval* a) {
  LASSERT_NUM("def", a->count, 3);
  LASSERT_TYPE("def", lisp_index(a, 1), 1, LISP_QEXP);
  LASSERT_TYPE("def { symbol }", lisp_index(a, 1)->root, 1,LISP_SYM);
  LASSERT_NUM("def { symbol num }", lisp_count(lisp_index(a, 1)), 1);
  lval* sym = lisp_index(a, 1)->root;
  lval* val = lisp_index(a, 2);
  lisp_lenv_put(JMLisp.env, sym->symbol, val);
  return sym;
}

lval* builtin_eval(lenv* e, lval* a) {
  LASSERT_NUM("eval", a->count, 2);
  LASSERT_TYPE("eval", lisp_index(a, 1), 1, LISP_QEXP);
  a->root->next->type = LISP_SEXP;
  return lisp_eval(e, a->root->next);
}

lval* builtin_let(lenv* e, lval* a) {
  LASSERT_NUM("let", a->count, 3);
  LASSERT_TYPE("let", lisp_index(a, 1), 1, LISP_QEXP);
  LASSERT_TYPE("let { symbol }", lisp_index(lisp_index(a, 1), 0), 0, LISP_SYM);
  LASSERT_NUM("let { symbol num }", lisp_count(lisp_index(a,1)), 1);
  lval* sym = lisp_index(lisp_index(a, 1), 0);
  lval* val = lisp_index(a, 2);
  lisp_lenv_put(e, sym->symbol, val);
  return sym;
}


lval* builtin_del(lenv* e, lval* a) {
  LASSERT_NUM("del", a->count, 2);
  LASSERT_NUM("del {...}", lisp_count(lisp_index(a, 1)), 1);
  LASSERT_TYPE("del", lisp_index(a, 1), 1, LISP_QEXP);
  LASSERT_TYPE("del {..}", lisp_index(lisp_index(a, 1), 0), 0, LISP_SYM);
  char* el = lisp_index(lisp_index(a, 1), 0)->symbol;
  lisp_lenv_pop(e, el);
  return lisp_list();
}

lval* builtin_car(lenv* e, lval* a) {
  LASSERT_NUM("car", a->count, 2);
  LASSERT_NOT_EMPTY("car", lisp_index(a, 1), 1);
  LASSERT_TYPE("car", lisp_index(a, 1), 1, LISP_QEXP);
  return lisp_lval_copy(lisp_index(a,1)->root);
}

lval* builtin_cdr(lenv* e, lval* a) {
  LASSERT_NUM("cdr", a->count, 2);
  LASSERT_TYPE("cdr", lisp_index(a, 1), 1, LISP_QEXP);
  LASSERT_NOT_EMPTY("cdr", lisp_index(a, 1), 1);
  lval* new_list = lisp_list();
  new_list->type = LISP_QEXP;
  lval* t = lisp_lval_copy(a);
  new_list->root = t->root->next->root->next;
  new_list->last = t->root->next->last;
  return new_list;
}

lval* builtin_append(lenv* e, lval* a) {
  LASSERT_NUM("append", a->count, 3);
  LASSERT_TYPE("append", lisp_index(a, 1), 1, LISP_QEXP);
  lval* x = lisp_index(a, 1);
  lval* y = lisp_index(a, 2);
  x->last->next = y;
  return lisp_lval_copy(x);
}

lval* builtin_prepend(lenv* e, lval* a) {
  LASSERT_NUM("prepend", a->count, 3);
  LASSERT_TYPE("prepend", lisp_index(a, 1), 1, LISP_QEXP);
  lval* x = lisp_index(a, 1);
  lval* y = lisp_index(a, 2);
  lval* new_list = lisp_list();
  new_list->type = LISP_QEXP;
  lisp_cons(new_list, y);
  new_list->last->next = x->root;;
  return lisp_lval_copy(new_list);
}

lval* builtin_join(lenv* e, lval* a) {
  LASSERT_NUM("join", a->count, 3);
  LASSERT_TYPE("join", lisp_index(a, 1), 1, LISP_QEXP);
  LASSERT_TYPE("join", lisp_index(a, 2), 2, LISP_QEXP);
  lval* x = lisp_index(a, 1);
  lval* y = lisp_index(a, 2);
  x->last->next = y->root;
  return lisp_lval_copy(x);
}

lval* builtin_load(lenv* e, lval* a) {
  extern FILE* yyin;
  LASSERT_NUM("load", a->count, 2);
  LASSERT_TYPE("load", lisp_index(a, 1), 1, LISP_STR);
  JMLisp.echo = 0;
  lval* t = lisp_index(a, 1);
  printf("%s", t->symbol);
  yyin = fopen(t->symbol, "r");
  fseek(yyin, 0, SEEK_END);
  long fsize = ftell(yyin);
  fseek(yyin, 0, SEEK_SET);
  yypush_buffer_state(yy_create_buffer(yyin, fsize));
  return lisp_list();
}

lval* builtin_error(lenv* e, lval* a) {
  LASSERT_NUM("error", a->count, 2);
  LASSERT_TYPE("error", lisp_index(a, 1), 1, LISP_STR);
  lval* t = lisp_index(a, 1);
  return lisp_err(t->symbol);
}

lval* builtin_if(lenv* e, lval* a) {
  LASSERT_NUM("if", a->count, 4);
  LASSERT_TYPE("if", lisp_index(a, 1), 1, LISP_NUM);
  LASSERT_TYPE("if", lisp_index(a, 2), 2, LISP_QEXP);
  LASSERT_TYPE("if", lisp_index(a, 3), 3, LISP_QEXP);
  lval* _cond = lisp_index(a,1);
  lval* _then = lisp_index(a,2);
  lval* _else = lisp_index(a,3);
  _then->type = LISP_SEXP;
  _else->type = LISP_SEXP;
  if(_cond->num) {
    return lisp_eval(e, _then);
  }
  else {
    return lisp_eval(e, _else);
  }
}

lval* builtin_ord(lenv* e, lval* a, char* op) {
  LASSERT_NUM(op, a->count, 3);
  LASSERT_TYPE(op, lisp_index(a,1), 1, LISP_NUM);
  LASSERT_TYPE(op, lisp_index(a,2), 2, LISP_NUM);

  lval* x = lisp_index(a, 1);
  lval* y = lisp_index(a, 2);
  int r;
  if (strcmp(op, ">")  == 0) {
    r = (x->num >  y->num);
  }
  if (strcmp(op, "<")  == 0) {
    r = (x->num < y->num);
  }
  if (strcmp(op, ">=")  == 0) {
    r = (x->num >=  y->num);
  }
  if (strcmp(op, "<=")  == 0) {
    r = (x->num <=  y->num);
  }
  return lisp_num(r);
}

lval* builtin_gt(lenv* e, lval* a) {
  return builtin_ord(e, a, ">");
}

lval* builtin_lt(lenv* e, lval* a) {
  return builtin_ord(e, a, "<");
}

lval* builtin_ge(lenv* e, lval* a) {
  return builtin_ord(e, a, ">=");
}

lval* builtin_le(lenv* e, lval* a) {
  return builtin_ord(e, a, "<=");
}

lval* builtin_cmp(lenv* e, lval* a, char* op) {
  LASSERT_NUM(op, a->count, 3);
  lval* x = lisp_index(a, 1);
  lval* y = lisp_index(a, 2);
  int r;
  if (strcmp(op, "==") == 0) {
    r =  lval_eq(x, y);
  }
  if (strcmp(op, "!=") == 0) {
    r = !lval_eq(x, y);
  }
  return lisp_num(r);
}

lval* builtin_eq(lenv* e, lval* a) {
  return builtin_cmp(e, a, "==");
}

lval* builtin_ne(lenv* e, lval* a) {
  return builtin_cmp(e, a, "!=");
}

int lval_eq(lval* x, lval* y) {
  /* Different Types are always unequal */
  if (x->type != y->type) { return 0; }
  
  /* Compare Based upon type */
  switch (x->type) {
    /* Compare Number Value */
  case LISP_NUM: return (x->num == y->num);
    
    /* Compare String Values */
  case LISP_ERR: return (strcmp(x->err, y->err) == 0);
  case LISP_SYM: return (strcmp(x->symbol, y->symbol) == 0);
  case LISP_STR: return (strcmp(x->symbol, y->symbol) == 0);
    
    /* If Builtin compare functions, otherwise compare formals and body */
  case LISP_BUILTIN: return x->fun == y->fun;
  case LISP_FUNC: return lval_eq(x->args, y->args) && lval_eq(x->body, y->body);
    
    /* If list compare every individual element */
  case LISP_QEXP:
  case LISP_SEXP:
    if (lisp_count(x) != lisp_count(y)) { return 0; }
    x = x->root;
    y = y->root;
    while(x && y) {
      if(!lval_eq(x, y)) return 0;
      x = x->next;
      y = y->next;
    }
    return 1;
    break;
  }
  return 0;
}

lval* builtin_list(lenv* e, lval* a) {
  lval* new_list = lisp_list();
  new_list->type = LISP_QEXP;
  return lisp_cons(new_list, lisp_lval_copy(lisp_index(a,1)));
}


lval* builtin_typeof(lenv* e, lval* a) {
  LASSERT_NUM(a, a->count, 2);
  lval* elem = lisp_index(a, 1);
  return lisp_str(lisp_print_type(elem->type));
}

lval* builtin_print(lenv* e, lval* a) {
  LASSERT_NUM(a, a->count, 2);
  lval* elem = lisp_index(a, 1);
  if(elem->type == LISP_STR) { printf("%s", elem->symbol); return lisp_list() ; }
  else if(elem->type == LISP_NUM) {printf("%f", elem->num); return lisp_list();  }
  else return lisp_err("Can't print!");

}
