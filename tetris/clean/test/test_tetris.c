
/* CUnit files */
#include "CUnit/Basic.h"

/* Tetris files */
#include "../grid.h"

/* Standard library files */
#include <stdio.h>
#include <string.h>

static size_t random_row_index;
static size_t const ZERO = 0;

/* Suite initialization */
int init_suite()
{
   random_row_index = rand() % NUMBER_OF_ROWS;
   return 0;
}

/* Suite termination */
int clean_suite()
{
   return 0;
}

void test_set_row_to_zero()
{
  size_t columnIndex;
  set_row_to_zero(random_row_index);
  for ( columnIndex = 0; columnIndex < NUMBER_OF_COLUMNS; ++columnIndex )
    CU_ASSERT( grid[random_row_index][columnIndex] == ZERO )
  
}

void test_set_grid_to_zero()
{
  int row_index, column_index;

  set_grid_to_zero();

  for ( row_index = 0; row_index < NUMBER_OF_ROWS; ++row_index ) {
    for ( column_index = 0; column_index < NUMBER_OF_COLUMNS;
                             ++column_index ) {
      CU_ASSERT( grid[row_index][column_index] == ZERO );
    }
  }
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main()
{
   CU_pSuite Suite_grid = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   Suite_grid = CU_add_suite("Suite_grid", init_suite, clean_suite);
   if ( Suite_grid == NULL ) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(Suite_grid, "test of set_row_to_zero()", test_set_row_to_zero)) ||
       (NULL == CU_add_test(Suite_grid, "test of set_grid_to_zero()", test_set_grid_to_zero)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}

