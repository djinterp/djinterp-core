/******************************************************************************
* djinterp [test]                                djinterp_tests_sa_array.c
*
* Implementation of array utility macro tests.
*
* Tests:
*   - D_ARRAY_TOTAL_SIZE - Total array size calculation
*   - D_ARRAY_COUNT - Compile-time element counting
*
* path:      \test\djinterp_tests_sa_array.c
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2026.01.15
******************************************************************************/

#include "djinterp_tests_sa.h"


bool
d_tests_sa_macro_array_total_size
(
    struct d_test_counter* _test_info
)
{
    // TODO: Port from djinterp_type_tests_sa.c
    printf("  TODO: d_tests_sa_macro_array_total_size\n");
    return true;
}

bool
d_tests_sa_macro_array_count
(
    struct d_test_counter* _test_info
)
{
    // TODO: Port from djinterp_type_tests_sa.c
    printf("  TODO: d_tests_sa_macro_array_count\n");
    return true;
}

bool
d_tests_sa_array_macros_all
(
    struct d_test_counter* _test_info
)
{
    printf("\n========================================\n");
    printf("  ARRAY UTILITY MACRO TESTS\n");
    printf("========================================\n");

    struct d_test_counter module_counter = {0, 0, 0, 0};

    d_tests_sa_macro_array_total_size(&module_counter);
    d_tests_sa_macro_array_count(&module_counter);

    // Update totals
    _test_info->assertions_total  += module_counter.assertions_total;
    _test_info->assertions_passed += module_counter.assertions_passed;
    _test_info->tests_total       += module_counter.tests_total;
    _test_info->tests_passed      += module_counter.tests_passed;

    bool all_passed = (module_counter.tests_passed == module_counter.tests_total);

    printf("\n--- Array Utility Macro Tests Summary ---\n");
    printf("  Assertions: %zu/%zu passed\n",
           module_counter.assertions_passed, module_counter.assertions_total);
    printf("  Unit Tests: %zu/%zu passed\n",
           module_counter.tests_passed, module_counter.tests_total);
    printf("  Status: %s\n", all_passed ? "[PASS]" : "[FAIL]");
    printf("========================================\n\n");

    return all_passed;
}
