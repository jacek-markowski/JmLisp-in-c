#include "types.h"
#include "asserts.h"
#include "cons.h"
#include "eval.h"
#include "print.h"
#include "del.h"
#include "builtins.h"
#include "env.h"
#include "gc.h"
#include "copy.h"

#ifndef LISP_GLOBALS
int lassert_count(lval* v) {
  int count = 0;
  while(v) { count++; v = v->next; }
  return count;
}

lval* lassert_index(lval* v, int index) {
  v = v->root;
  while(index) v = v->next; 
  return v;
}
struct {
    int repl;
    int echo;
    gc* gc_root;
    int gc_rate;
    lenv* env;
    lval* root;
} JMLisp;
#define LISP_GLOBALS
#endif
