 #define LISP_SEATEST
 #include "seatest/seatest.h"
 #include "../src/lisp.h"
 //
 // create a test...
 //
 char* parseToString(char * expr) {
   static char p[1000];
   lval v = lispParseEvaluate(expr);
   if (v.err == LERR_DIV_ZERO) {
     strcpy(p, LERR_MSGS[LERR_DIV_ZERO]);
   }
   else if(v.err == LERR_BAD_OP) {
     strcpy(p, LERR_MSGS[LERR_BAD_OP]);
   }
   else if (v.err == LERR_BAD_NUM) {
     strcpy(p, LERR_MSGS[LERR_BAD_NUM]);
   }
   else {
     sprintf(p, "%.3f", v.num);
   }
   return p;
 }

 void test_naked_expression()
 {
   puts(">> test_naked_expression");
   assert_string_equal(parseToString("+ 1 2"), "3.000");
   assert_string_equal(parseToString("+ 1.1 2"), "3.100");
   assert_string_equal(parseToString("+ 1 2.1"), "3.100");
   assert_string_equal(parseToString("+ 1. 2"), "3.000");
   assert_string_equal(parseToString("+ 1 2."), "3.000");
   assert_string_equal(parseToString("+ 1. 2."), "3.000");
   assert_string_equal(parseToString("+ 1.1 2.0"), "3.100");
 }

 void test_paren_expression()
 {
   puts(">> test_paren_expression");
   assert_string_equal(parseToString("(+ 1 2)"), "3.000");
   assert_string_equal(parseToString("(+ 1.1 2)"), "3.100");
   assert_string_equal(parseToString("(+ 1 2.1)"), "3.100");
   assert_string_equal(parseToString("(+ 1. 2)"), "3.000");
   assert_string_equal(parseToString("(+ 1 2.)"), "3.000");
   assert_string_equal(parseToString("(+ 1. 2.)"), "3.000");
   assert_string_equal(parseToString("(+ 1.1 2.0)"), "3.100");
 }

 void test_error_handling()
 {
   puts(">> test_err_handling");
   assert_string_equal(parseToString("(/ 1 0)"), LERR_MSGS[LERR_DIV_ZERO]);
   assert_string_equal(parseToString("/ 1 0"), LERR_MSGS[LERR_DIV_ZERO]);
   assert_string_equal(parseToString("(& 1. 2)"),LERR_MSGS[LERR_BAD_OP]);
   assert_string_equal(parseToString("& 1 2."), LERR_MSGS[LERR_BAD_OP]);
   char* infi;
   asprintf(&infi, "* %f %f %f %f %f", pow(2,162), pow(2,262), pow(2,262), pow(2,262), pow(2,262));
   assert_string_equal(parseToString(infi), LERR_MSGS[LERR_BAD_NUM]);
   free(infi);
 }
 //
 // put the test into a fixture...
 //
 void test_fixture( void )
 {
   test_fixture_start();
   run_test(test_naked_expression);
   run_test(test_paren_expression);
   run_test(test_error_handling);
   test_fixture_end();
 }

 //
 // put the fixture into a suite...
 //
 void all_tests( void )
 {
   test_fixture();
 }

 //
 // run the suite!
 //
 int main( int argc, char** argv )
 {
   run_tests(all_tests);
   return 0;
 }
