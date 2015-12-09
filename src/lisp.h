#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#ifdef LISP_SEATEST
#include "../src/mpc/mpc.h"
#else
#include "mpc/mpc.h"
#endif
#include <readline/readline.h>
#include <readline/history.h>
/* Error Types */
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM, LERR_PARSE };
# ifndef LISP_MAIN
static  char* LERR_MSGS[] = {"Division By Zero!", "Bad Operator!", "Invalid Number!"};
#endif
/* LVAL Types */
enum { LVAL_NUM, LVAL_ERR };
/* LVAL STRUCT */
typedef struct {
  int type;
  double num;
  int err;
  char * errmsg;
} lval;

lval evalOp(lval x, char *op, lval y);
lval eval(mpc_ast_t* t);
lval lispParseEvaluate(char* expr);
void lispREPL (void);
