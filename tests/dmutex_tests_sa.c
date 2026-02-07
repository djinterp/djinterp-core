#include ".\dmutex_tests_sa.h"


/*
d_tests_sa_mutex_run_all
  Module-level aggregation function that runs all mutex tests.
  Executes tests for all categories:
  - Basic mutex operations
  - Recursive mutex operations
  - Thread operations
  - Condition variables
  - Read-write locks
  - Thread-specific storage
  - Synchronization primitives
  - Utility functions
*/
bool
d_tests_sa_mutex_run_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // run all test categories
    result = d_tests_sa_mutex_basic_all(_counter) && result;
    result = d_tests_sa_mutex_recursive_all(_counter) && result;
    result = d_tests_sa_mutex_thread_all(_counter) && result;
    result = d_tests_sa_mutex_cond_all(_counter) && result;
    result = d_tests_sa_mutex_rwlock_all(_counter) && result;
    result = d_tests_sa_mutex_tss_all(_counter) && result;
    result = d_tests_sa_mutex_sync_all(_counter) && result;
    result = d_tests_sa_mutex_utility_all(_counter) && result;

    return result;
}
