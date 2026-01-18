/******************************************************************************
* djinterp [test]                                       dstring_tests_sa_runner.c
*
*   Master test runner for dstring module tests. Aggregates all test categories
*   and provides the main entry point for running all d_string tests.
*
*
* path:      \src\test\dstring_tests_sa_runner.c
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.12.30
******************************************************************************/

#include "..\tests\dstring_tests_sa.h"


/******************************************************************************
* d_tests_sa_dstring_all
******************************************************************************/

/*
d_tests_sa_dstring_all
  Master test runner for all d_string unit tests. Runs all test categories
  and returns an aggregate test object containing all results.

Test categories run:
  - Creation & Destruction
  - Capacity Management
  - Access Functions
  - Safe Copy
  - Duplication
  - Comparison
  - Reversal
  - Error Functions

Parameter(s):
  (none)
Return:
  Test object containing all d_string test results.
*/
struct d_test_object*
d_tests_sa_dstring_all
(
    void
)
{
    struct d_test_object* group;
    size_t                child_idx;

    // create master group with all implemented test categories
    // adjust count based on how many test categories are implemented
    group     = d_test_object_new_interior("d_string Module Tests", 8);
    child_idx = 0;

    if (!group)
    {
        return NULL;
    }

    // I. CREATION & DESTRUCTION TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_creation_all();

    // II. CAPACITY MANAGEMENT TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_capacity_all();

    // III. ACCESS FUNCTION TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_access_all();

    // IV. SAFE COPY TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_copy_all();

    // VI. DUPLICATION TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_dup_all();

    // VII. COMPARISON TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_compare_all();

    // XI. REVERSAL TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_reversal_all();

    // XVI. ERROR STRING TESTS
    group->elements[child_idx++] = d_tests_sa_dstring_error_all();

    return group;
}
