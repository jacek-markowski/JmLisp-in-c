#include "lisp.h"

double evalOp(double x, char *op, double y)
{
  if (strcmp(op, "+") == 0) {
    return x + y;
  }
  if (strcmp(op, "-") == 0) {
    return x - y;
  }
  if (strcmp(op, "*") == 0) {
    return x * y;
  }
  if (strcmp(op, "/") == 0) {
    return x / y;
  }
  if (strcmp(op, "%") == 0) {
    return (long)x % (long)y;
  }
  return 0;
}


double eval(mpc_ast_t* t) {
  /* If tagged as number return it directly. */
  if (strstr(t->tag, "number")) {
    return atof(t->contents);
  } else if (strstr(t->children[1]->tag, "number")) {
    return atof(t->children[1]->contents);
  }

  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` */
  double x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = evalOp(x, op, eval(t->children[i]));
    i++;
  }
  return x;
}

char* lispParseEvaluate(char* expr)
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
      operator : '+' | '-' | '*' | '/' ;                  \
      expr     : <number> | '(' <number> ')' |'(' <operator> <expr>+ ')' ;  \
      lisp    : /^/ <expr>+ /$/ | /^/ <operator> <expr>+ /$/ ;                        \
    ", Number, Operator, Expr, Lisp);


  mpc_result_t r;
  char* parsed;
  if (mpc_parse("input", expr, Lisp, &r)) {
    mpc_ast_t* t = r.output;
    double result;
    if (!strstr(t->children[1]->tag, "expr")){
      /* example input lisp> + 1 2 */
      result = eval(t);
    } else {
      /* example input lisp> (+ 1 2) */
      result = eval(t->children[1]);
    }
    asprintf(&parsed, "%.3f", result);
    mpc_ast_delete(r.output);
  } else {
    char *err = mpc_err_string(r.error);
    err[strcspn(err, "\n")] = '\0';
    mpc_err_delete(r.error);
    mpc_cleanup(4, Number, Operator, Expr, Lisp);
    return err;
  }
  mpc_cleanup(4, Number, Operator, Expr, Lisp);
  return parsed;
}

void lispREPL ()
{
  puts("JmLisp Version 0.1.0");
  puts("Press Ctrl+c to Exit");
  while (1) {
    char *input = readline("*JmLisp*> ");
    char* p = lispParseEvaluate(input);
    add_history(input);
    printf(">> %s\n", p);
    free(input);
    free(p);
  }
}
