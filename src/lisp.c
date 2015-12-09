#include "lisp.h"
/* Create Number Type lval */
lval lval_num(double x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

/* Create a new error type lval */
lval lval_err(int x, char* msg) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  if (msg == NULL) {
    v.errmsg = LERR_MSGS[x];
  } else {
    v.errmsg = msg;
  }
  return v;
}

/* Print lval */
void lval_print(lval v) {
  switch (v.type) {
  case LVAL_NUM: printf("%.3f", v.num); break;
  case LVAL_ERR: printf("Error %s", v.errmsg); break;
  default: printf("*Unkown JmLisp Error*"); break;
  }
}

void lval_println(lval v) { lval_print(v); putchar('\n'); }


lval lispParseEvaluate(char* expr)
{
  /* Create some parsers */
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lisp = mpc_new("lisp");
  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT,
	    "                                                     \
      number   :  /-?[0-9]+\\.?([0-9]*)/ ;                              \
      operator : '+' | '-' | '*' | '/' | /[a-z!@#$%^&]\\s/ ;                  \
      expr     : <number> | '(' <number> ')' |'(' <operator> <expr>+ ')' ;  \
      lisp    : /^/ <expr>+ /$/ | /^/ <operator> <expr>+ /$/ ;                        \
    ", Number, Operator, Expr, Lisp);


  mpc_result_t r;
  lval result;
  if (mpc_parse("input", expr, Lisp, &r)) {
    mpc_ast_t* t = r.output;
    if (!strstr(t->children[1]->tag, "expr")){
      /* example input lisp> + 1 2 */
      result = eval(t);
    } else {
      /* example input lisp> (+ 1 2) */
      result = eval(t->children[1]);
    }
    mpc_ast_delete(r.output);
  } else {
    char *err = mpc_err_string(r.error);
    err[strcspn(err, "\n")] = '\0';
    mpc_err_delete(r.error);
    mpc_cleanup(4, Number, Operator, Expr, Lisp);
    return lval_err(LERR_PARSE, err);
  }
  mpc_cleanup(4, Number, Operator, Expr, Lisp);
  return result;
}


lval evalOp(lval x, char* op, lval y) {
  if (x.type == LVAL_ERR) {return x;}
  if (y.type == LVAL_ERR) {return y;}

  if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num);}
  if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num);}
  if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num);}
  if (strcmp(op, "/") == 0) {
    return y.num == 0
      ? lval_err(LERR_DIV_ZERO, NULL)
      : lval_num(x.num / y.num);
  }
  return lval_err(LERR_BAD_OP, NULL);
}

lval eval(mpc_ast_t* t) {
  errno = 0;
  int is_number = 0;
  double num;
  if (strstr(t->tag, "number")) {
    num = strtod(t->contents, NULL);
    is_number = 1;
  } else if (strstr(t->children[1]->tag, "number")) {
    num = strtod(t->children[1]->contents, NULL);
    is_number = 1;
  }
  if (is_number == 1) {
    return errno != ERANGE && isfinite(num)
      ? lval_num(num)
      : lval_err(LERR_BAD_NUM, NULL);
  }

  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` */
  lval x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = evalOp(x, op, eval(t->children[i]));
    i++;
  }
  return isfinite(x.num)
    ? x
    : lval_err(LERR_BAD_NUM, NULL);
}


void lispREPL ()
{
  puts("JmLisp Version 0.1.0");
  puts("Press Ctrl+c to Exit");
  while (1) {
    char *input = readline("*JmLisp*> ");
    lval p = lispParseEvaluate(input);
    add_history(input);
    printf(">>");
    lval_println(p);
    free(input);
  }
}
