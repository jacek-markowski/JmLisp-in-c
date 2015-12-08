#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <editline/readline.h>
#ifdef LISP_SEATEST
#include "../src/mpc/mpc.h"
#else
#include "mpc/mpc.h"
#endif

double evalOp(double x, char *op, double y);
double eval(mpc_ast_t* t);
char* lispParseEvaluate(char* expr);
void lispREPL (void);
