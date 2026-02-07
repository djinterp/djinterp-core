#include ".\dmutex_tests_sa.h"


/*
d_tests_sa_recursive_mutex_init
  Tests the d_recursive_mutex_init function.
  Tests the following:
  - NULL mutex pointer handling
  - successful initialization
*/
bool
d_tests_sa_recursive_mutex_init
(
    struct d_test_counter* _counter
)
{
    bool                 result;
    d_recursive_mutex_t mutex;
    int                  init_result;

    result = true;

    // test 1: NULL mutex should fail
    init_result = d_recursive_mutex_init(NULL);
    result      = d_assert_standalone(
        init_result != D_MUTEX_SUCCESS,
        "recursive_init_null",
        "NULL recursive mutex should fail to initialize",
        _counter) && result;

    // test 2: successful initialization
    init_result = d_recursive_mutex_init(&mutex);
    result      = d_assert_standalone(
        init_result == D_MUTEX_SUCCESS,
        "recursive_init_success",
        "Recursive mutex initialization should succeed",
        _counter) && result;

    if (init_result == D_MUTEX_SUCCESS)
    {
        d_recursive_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_recursive_mutex_destroy
  Tests the d_recursive_mutex_destroy function.
  Tests the following:
  - NULL mutex pointer handling
  - successful destruction
*/
bool
d_tests_sa_recursive_mutex_destroy
(
    struct d_test_counter* _counter
)
{
    bool                 result;
    d_recursive_mutex_t mutex;
    int                  destroy_result;

    result = true;

    // test 1: NULL mutex handling
    destroy_result = d_recursive_mutex_destroy(NULL);
    result         = d_assert_standalone(
        destroy_result != D_MUTEX_SUCCESS,
        "recursive_destroy_null",
        "NULL recursive mutex destroy should fail gracefully",
        _counter) && result;

    // test 2: successful destruction
    if (d_recursive_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        destroy_result = d_recursive_mutex_destroy(&mutex);
        result         = d_assert_standalone(
            destroy_result == D_MUTEX_SUCCESS,
            "recursive_destroy_success",
            "Destroying recursive mutex should succeed",
            _counter) && result;
    }

    return result;
}


/*
d_tests_sa_recursive_mutex_lock
  Tests the d_recursive_mutex_lock function.
  Tests the following:
  - NULL mutex pointer handling
  - successful lock acquisition
*/
bool
d_tests_sa_recursive_mutex_lock
(
    struct d_test_counter* _counter
)
{
    bool                 result;
    d_recursive_mutex_t mutex;
    int                  lock_result;

    result = true;

    // test 1: NULL mutex should fail
    lock_result = d_recursive_mutex_lock(NULL);
    result      = d_assert_standalone(
        lock_result != D_MUTEX_SUCCESS,
        "recursive_lock_null",
        "Locking NULL recursive mutex should fail",
        _counter) && result;

    // test 2: successful lock
    if (d_recursive_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        lock_result = d_recursive_mutex_lock(&mutex);
        result      = d_assert_standalone(
            lock_result == D_MUTEX_SUCCESS,
            "recursive_lock_success",
            "Locking recursive mutex should succeed",
            _counter) && result;

        if (lock_result == D_MUTEX_SUCCESS)
        {
            d_recursive_mutex_unlock(&mutex);
        }

        d_recursive_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_recursive_mutex_unlock
  Tests the d_recursive_mutex_unlock function.
  Tests the following:
  - NULL mutex pointer handling
  - successful unlock
*/
bool
d_tests_sa_recursive_mutex_unlock
(
    struct d_test_counter* _counter
)
{
    bool                 result;
    d_recursive_mutex_t mutex;
    int                  unlock_result;

    result = true;

    // test 1: NULL mutex should fail
    unlock_result = d_recursive_mutex_unlock(NULL);
    result        = d_assert_standalone(
        unlock_result != D_MUTEX_SUCCESS,
        "recursive_unlock_null",
        "Unlocking NULL recursive mutex should fail",
        _counter) && result;

    // test 2: successful unlock
    if (d_recursive_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_recursive_mutex_lock(&mutex);

        unlock_result = d_recursive_mutex_unlock(&mutex);
        result        = d_assert_standalone(
            unlock_result == D_MUTEX_SUCCESS,
            "recursive_unlock_success",
            "Unlocking recursive mutex should succeed",
            _counter) && result;

        d_recursive_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_recursive_mutex_trylock
  Tests the d_recursive_mutex_trylock function.
  Tests the following:
  - NULL mutex pointer handling
  - successful trylock
*/
bool
d_tests_sa_recursive_mutex_trylock
(
    struct d_test_counter* _counter
)
{
    bool                 result;
    d_recursive_mutex_t mutex;
    int                  try_result;

    result = true;

    // test 1: NULL mutex should fail
    try_result = d_recursive_mutex_trylock(NULL);
    result     = d_assert_standalone(
        try_result != D_MUTEX_SUCCESS,
        "recursive_trylock_null",
        "Trylock on NULL recursive mutex should fail",
        _counter) && result;

    // test 2: successful trylock
    if (d_recursive_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        try_result = d_recursive_mutex_trylock(&mutex);
        result     = d_assert_standalone(
            try_result == D_MUTEX_SUCCESS,
            "recursive_trylock_success",
            "Trylock on available recursive mutex should succeed",
            _counter) && result;

        if (try_result == D_MUTEX_SUCCESS)
        {
            d_recursive_mutex_unlock(&mutex);
        }

        d_recursive_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_recursive_mutex_recursive_locking
  Tests the recursive locking capability.
  Tests the following:
  - same thread can lock multiple times
  - must unlock same number of times
*/
bool
d_tests_sa_recursive_mutex_recursive_locking
(
    struct d_test_counter* _counter
)
{
    bool                 result;
    d_recursive_mutex_t mutex;
    int                  lock_result1;
    int                  lock_result2;
    int                  lock_result3;

    result = true;

    if (d_recursive_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        // test 1: lock multiple times
        lock_result1 = d_recursive_mutex_lock(&mutex);
        lock_result2 = d_recursive_mutex_lock(&mutex);
        lock_result3 = d_recursive_mutex_lock(&mutex);

        result = d_assert_standalone(
            lock_result1 == D_MUTEX_SUCCESS &&
            lock_result2 == D_MUTEX_SUCCESS &&
            lock_result3 == D_MUTEX_SUCCESS,
            "recursive_multiple_locks",
            "Same thread should be able to lock recursively 3 times",
            _counter) && result;

        // test 2: unlock same number of times
        d_recursive_mutex_unlock(&mutex);
        d_recursive_mutex_unlock(&mutex);

        // after 2 unlocks, should still be locked (need 3rd unlock)
        int try_result = d_recursive_mutex_trylock(&mutex);
        result         = d_assert_standalone(
            try_result == D_MUTEX_BUSY,
            "recursive_partial_unlock",
            "After 2 unlocks of 3 locks, should still be locked",
            _counter) && result;

        // final unlock
        d_recursive_mutex_unlock(&mutex);

        // now should be unlockable
        try_result = d_recursive_mutex_trylock(&mutex);
        result     = d_assert_standalone(
            try_result == D_MUTEX_SUCCESS,
            "recursive_full_unlock",
            "After all unlocks, should be acquirable",
            _counter) && result;

        if (try_result == D_MUTEX_SUCCESS)
        {
            d_recursive_mutex_unlock(&mutex);
        }

        d_recursive_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_mutex_recursive_all
  Aggregation function that runs all recursive mutex tests.
*/
bool
d_tests_sa_mutex_recursive_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Recursive Mutex Operations\n");
    printf("  -------------------------------------\n");

    result = d_tests_sa_recursive_mutex_init(_counter) && result;
    result = d_tests_sa_recursive_mutex_destroy(_counter) && result;
    result = d_tests_sa_recursive_mutex_lock(_counter) && result;
    result = d_tests_sa_recursive_mutex_unlock(_counter) && result;
    result = d_tests_sa_recursive_mutex_trylock(_counter) && result;
    result = d_tests_sa_recursive_mutex_recursive_locking(_counter) && result;

    return result;
}
