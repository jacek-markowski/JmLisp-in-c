#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include "mpc.h"

/* Use operator string to see which operation to perform */
long eval_op(long x, char *op, long y)
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
    return x % y;
  }
  return 0;
}

long print_ast(mpc_ast_t * t)
{
  if (t->children_num == 0) return 1;
  for (int i=0; i < t->children_num; i++) {
    char* tag = t->children[i]->tag;
    char* contents = t->children[i]->contents;
    printf("%d :: Tag: %s, Content: %s\n", i,  tag, contents);
    print_ast(t->children[i]);
  }
  return 1;
}


long eval(mpc_ast_t* t) {

  /* If tagged as number return it directly. */
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }

  /* The operator is always second child. */
  char* op = t->children[1]->contents;

  /* We store the third child in `x` */
  long x = eval(t->children[2]);

  /* Iterate the remaining children and combining. */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}


int main(int argc, char **argv)
{
  /* Create some parsers */
  mpc_parser_t *Number = mpc_new("number");
  mpc_parser_t *Operator = mpc_new("operator");
  mpc_parser_t *Expr = mpc_new("expr");
  mpc_parser_t *Lispy = mpc_new("lispy");
  /* Define them with the following Language */
  mpca_lang(MPCA_LANG_DEFAULT,
	    "                                                     \
      number   : /-?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' ;                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      lispy    : /^/ <expr>+ /$/ | /^/ <operator> <expr>+ /$/ ;                        \
    ", Number, Operator, Expr, Lispy);
  /* Print version and exit information */
  puts("JmLisp Version 0.1.0");
  puts("Press Ctrl+c to Exit");
  /* In a never ending loop */
  while (1) {
    /* Output our prompt */
    char *input = readline("[JmLisp]> ");
    /* add input to history */
    add_history(input);

    mpc_result_t r;
    /* Echo input back to user */
    if (mpc_parse("input", input, Lispy, &r)) {
      /* On Success Print the AST */
      //print_ast(r.output);
      mpc_ast_t* t = r.output;
      long result;
      if (!strstr(t->children[1]->tag, "expr")){
	/* example input lisp> + 1 2 */
	result = eval(t);
      } else {
	/* example input lisp> (+ 1 2) */
	result = eval(t->children[1]);
      }

      printf("=> %li\n", result);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise print error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    /* Free retrived input */
    free(input);
  }
  mpc_cleanup(4, Number, Operator, Expr, Lispy);
  return 0;
}
