//#define LISP_DEBUG
// uncomment last line to have debug messages

#ifdef LISP_DEBUG
#define DEBUG_PRINT(...) do{ fprintf( stderr, __VA_ARGS__ ); } while( 0 )
#define GC_COUNT() gc_count();
#else
#define DEBUG_PRINT(...) do{ } while ( 0 )
#define GC_COUNT() do {} while (0)
#endif


#define LASSERT(args, cond, fmt, ...) \
  if (!(cond)) { lval* err = lisp_err(fmt, ##__VA_ARGS__); return err; }

#define LASSERT_TYPE(func, args, index, expect)	\
  LASSERT(args, args->type == expect,		\
    "Function '%s' passed incorrect type for argument %i. Got %s, Expected %s.", \
	  func, index, lisp_print_type(args->type), lisp_print_type(expect))

#define LASSERT_NUM(func, count, num)		\
  LASSERT(args, count == num,			\
    "Function '%s' passed incorrect number of arguments. Got %i, Expected %i.", \
    func, count , num)

#define LASSERT_NOT_EMPTY(func, args, index) \
  LASSERT(args, args->root, \
    "Function '%s' passed {} for argument %i.", func, index);
