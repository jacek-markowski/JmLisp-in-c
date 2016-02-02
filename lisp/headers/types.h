typedef struct lval lval;
typedef struct lenv lenv;
typedef struct gc gc;
typedef lval* (*builtin) (lenv* e, lval* a);

enum {LISP_SYM, LISP_FUNC, LISP_SEXP, LISP_QEXP, LISP_NUM, LISP_STR, LISP_ERR, LISP_BUILTIN};

struct lval {
  int type;
  union {
    double num;
    char* symbol; 
    char* err;
    struct {
      /* builtin */
      builtin fun;
    };
    struct {
      /* lambda */
      lval* args;
      lval* body;
      lenv* env;
    };
    struct {
      /* s-expression */
      int count;
      lval* root;
      lval* last;
    };
  };
  lval* next;
  lval* prev;
  /* garbage collector */
  int in_use;
  /* bison helpers */
  int full; /* when full add to parent */
  lval* parent;
};

struct lenv {
  int in_use;
  char* symbol;
  lval* value;
  lenv* parent;
  lenv* next;
  lenv* prev;
};

struct gc {
  lval* elem;
  gc* next;
  gc* prev;
};

#ifndef LISP_GLOBALS
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
