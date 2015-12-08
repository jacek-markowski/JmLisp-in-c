#define LISP_SEATEST
#include "seatest/seatest.h"
#include "../src/lisp.h"

//
// create a test...
//
void test_naked_expression()
{
  puts(">> test_naked_expression");
  assert_string_equal(lispParseEvaluate("+ 1 2"), "3.000");
  assert_string_equal(lispParseEvaluate("+ 1.1 2"), "3.100");
  assert_string_equal(lispParseEvaluate("+ 1 2.1"), "3.100");
  assert_string_equal(lispParseEvaluate("+ 1. 2"), "3.000");
  assert_string_equal(lispParseEvaluate("+ 1 2."), "3.000");
  assert_string_equal(lispParseEvaluate("+ 1. 2."), "3.000");
  assert_string_equal(lispParseEvaluate("+ 1.1 2.0"), "3.100");
}

void test_paren_expression()
{
  puts(">> test_paren_expression");
  assert_string_equal(lispParseEvaluate("(+ 1 2)"), "3.000");
  assert_string_equal(lispParseEvaluate("(+ 1.1 2)"), "3.100");
  assert_string_equal(lispParseEvaluate("(+ 1 2.1)"), "3.100");
  assert_string_equal(lispParseEvaluate("(+ 1. 2)"), "3.000");
  assert_string_equal(lispParseEvaluate("(+ 1 2.)"), "3.000");
  assert_string_equal(lispParseEvaluate("(+ 1. 2.)"), "3.000");
  assert_string_equal(lispParseEvaluate("(+ 1.1 2.0)"), "3.100");
}


//
// put the test into a fixture...
//
void test_fixture( void )
{
  test_fixture_start();
  run_test(test_naked_expression);
  run_test(test_paren_expression);
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
