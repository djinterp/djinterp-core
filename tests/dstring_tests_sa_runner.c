/******************************************************************************
* djinterp [test]                                       dstring_tests_sa_runner.c
*
*   Master test runner and utility functions for dstring module tests.
*
*
* path:      \src\test\dstring_tests_sa_runner.c
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.12.30
******************************************************************************/
#include ".\dstring_tests_sa.h"


/******************************************************************************
 * TEST UTILITY FUNCTIONS
 *****************************************************************************/

/*
d_tests_dstring_setup
  Sets up test environment for string tests.
*/
bool
d_tests_dstring_setup
(
    void
)
{
    // no specific setup needed for string tests
    // this function is here for consistency and potential future use
    
    return true;
}


/*
d_tests_dstring_teardown
  Cleans up test environment.
*/
bool
d_tests_dstring_teardown
(
    void
)
{
    // no specific teardown needed for string tests
    // this function is here for consistency and potential future use
    
    return true;
}


/*
d_tests_dstring_fill_buffer
  Fills a buffer with a specific character pattern.
*/
void
d_tests_dstring_fill_buffer
(
    char*  _buffer,
    size_t _size,
    char   _pattern
)
{
    size_t i;
    
    if ( (!_buffer) || 
         (_size == 0) )
    {
        return;
    }
    
    for (i = 0; i < _size - 1; i++)
    {
        _buffer[i] = _pattern;
    }
    
    _buffer[_size - 1] = '\0';
    
    return;
}


/*
d_tests_dstring_compare_buffers
  Compares two buffers byte-by-byte.
*/
bool
d_tests_dstring_compare_buffers
(
    const char* _buf1,
    const char* _buf2,
    size_t      _size
)
{
    size_t i;
    
    if ( (!_buf1) || 
         (!_buf2) )
    {
        return (_buf1 == _buf2);
    }
    
    if (_size == 0)
    {
        return true;
    }
    
    for (i = 0; i < _size; i++)
    {
        if (_buf1[i] != _buf2[i])
        {
            return false;
        }
    }
    
    return true;
}


/******************************************************************************
 * MASTER TEST RUNNER
 *****************************************************************************/

/*
d_tests_dstring_run_all
  Master test runner for all dstring tests.
  Tests the following:
  - Safe string copy operations
  - String duplication
  - Case-insensitive comparison
  - String tokenization
  - String length operations
  - String search
  - Case conversion
  - String manipulation
  - Error handling
  - NULL parameter handling
  - Boundary conditions
*/
struct d_test_object*
d_tests_dstring_run_all
(
    void
)
{
    struct d_test_object* group;
    bool                  setup_success;
    size_t                idx;

    // setup test environment
    setup_success = d_tests_dstring_setup();
    
    if (!setup_success)
    {
        printf("Failed to setup dstring test environment\n");
        return NULL;
    }

    // create master group
    group = d_test_object_new_interior("dstring Module Tests", 11);

    if (!group)
    {
        d_tests_dstring_teardown();
        return NULL;
    }

    // run all test categories
    idx = 0;
    group->elements[idx++] = d_tests_dstring_safe_copy_all();
    group->elements[idx++] = d_tests_dstring_duplication_all();
    group->elements[idx++] = d_tests_dstring_case_comparison_all();
    group->elements[idx++] = d_tests_dstring_tokenization_all();
    group->elements[idx++] = d_tests_dstring_length_all();
    group->elements[idx++] = d_tests_dstring_search_all();
    group->elements[idx++] = d_tests_dstring_case_conversion_all();
    group->elements[idx++] = d_tests_dstring_manipulation_all();
    group->elements[idx++] = d_tests_dstring_error_handling_all();
    group->elements[idx++] = d_tests_dstring_null_params_all();
    group->elements[idx++] = d_tests_dstring_boundary_conditions_all();

    // cleanup
    d_tests_dstring_teardown();

    return group;
}


