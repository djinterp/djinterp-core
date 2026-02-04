/******************************************************************************
* djinterp [test]                                            type_info_tests_sa.c
*
*   Module-level aggregation for type_info.h unit tests.
*   Runs all test categories for the type_info module.
*
*
* path:      \tests\meta\type_info_tests_sa.c
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.02.04
******************************************************************************/
#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_run_all
    Module-level aggregation function that runs all type_info tests.
    Executes tests for all categories:
    - Type definitions and bit layout constants
    - Builder macros
    - Modifier macros
    - Accessor and test macros
    - Predefined constants
    - Composite builders
    - Utility macros
*/

bool
d_tests_sa_type_info_run_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // run all test categories
    result = d_tests_sa_type_info_definitions_all(_counter) && result;
    result = d_tests_sa_type_info_builders_all(_counter)    && result;
    result = d_tests_sa_type_info_modifiers_all(_counter)   && result;
    result = d_tests_sa_type_info_accessors_all(_counter)   && result;
    result = d_tests_sa_type_info_predefined_all(_counter)  && result;
    result = d_tests_sa_type_info_composite_all(_counter)   && result;
    result = d_tests_sa_type_info_utility_all(_counter)     && result;

    return result;
}
