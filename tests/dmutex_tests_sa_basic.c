#include ".\dmutex_tests_sa.h"


/*
d_tests_sa_mutex_init
  Tests the d_mutex_init function.
  Tests the following:
  - NULL mutex pointer handling
  - successful initialization
  - mutex is usable after initialization
*/
bool
d_tests_sa_mutex_init
(
    struct d_test_counter* _counter
)
{
    bool      result;
    d_mutex_t mutex;
    int       init_result;

    result = true;

    // test 1: NULL mutex should fail
    init_result = d_mutex_init(NULL);
    result      = d_assert_standalone(
        init_result != D_MUTEX_SUCCESS,
        "init_null_mutex",
        "NULL mutex should fail to initialize",
        _counter) && result;

    // test 2: successful initialization
    init_result = d_mutex_init(&mutex);
    result      = d_assert_standalone(
        init_result == D_MUTEX_SUCCESS,
        "init_success",
        "Mutex initialization should succeed",
        _counter) && result;

    // test 3: mutex is usable (can lock/unlock)
    if (init_result == D_MUTEX_SUCCESS)
    {
        int lock_result = d_mutex_lock(&mutex);
        result          = d_assert_standalone(
            lock_result == D_MUTEX_SUCCESS,
            "init_usable_lock",
            "Initialized mutex should be lockable",
            _counter) && result;

        if (lock_result == D_MUTEX_SUCCESS)
        {
            int unlock_result = d_mutex_unlock(&mutex);
            result            = d_assert_standalone(
                unlock_result == D_MUTEX_SUCCESS,
                "init_usable_unlock",
                "Initialized mutex should be unlockable",
                _counter) && result;
        }

        d_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_mutex_destroy
  Tests the d_mutex_destroy function.
  Tests the following:
  - NULL mutex pointer handling
  - successful destruction
  - destruction after use
*/
bool
d_tests_sa_mutex_destroy
(
    struct d_test_counter* _counter
)
{
    bool      result;
    d_mutex_t mutex;
    int       destroy_result;

    result = true;

    // test 1: NULL mutex handling
    destroy_result = d_mutex_destroy(NULL);
    result         = d_assert_standalone(
        destroy_result != D_MUTEX_SUCCESS,
        "destroy_null_mutex",
        "NULL mutex destroy should fail gracefully",
        _counter) && result;

    // test 2: destroy unused mutex
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        destroy_result = d_mutex_destroy(&mutex);
        result         = d_assert_standalone(
            destroy_result == D_MUTEX_SUCCESS,
            "destroy_unused",
            "Destroying unused mutex should succeed",
            _counter) && result;
    }

    // test 3: destroy after use
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_mutex_lock(&mutex);
        d_mutex_unlock(&mutex);

        destroy_result = d_mutex_destroy(&mutex);
        result         = d_assert_standalone(
            destroy_result == D_MUTEX_SUCCESS,
            "destroy_after_use",
            "Destroying used mutex should succeed",
            _counter) && result;
    }

    return result;
}


/*
d_tests_sa_mutex_lock
  Tests the d_mutex_lock function.
  Tests the following:
  - NULL mutex pointer handling
  - successful lock acquisition
  - mutex provides mutual exclusion
*/
bool
d_tests_sa_mutex_lock
(
    struct d_test_counter* _counter
)
{
    bool      result;
    d_mutex_t mutex;
    int       lock_result;

    result = true;

    // test 1: NULL mutex should fail
    lock_result = d_mutex_lock(NULL);
    result      = d_assert_standalone(
        lock_result != D_MUTEX_SUCCESS,
        "lock_null_mutex",
        "Locking NULL mutex should fail",
        _counter) && result;

    // test 2: successful lock
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        lock_result = d_mutex_lock(&mutex);
        result      = d_assert_standalone(
            lock_result == D_MUTEX_SUCCESS,
            "lock_success",
            "Locking mutex should succeed",
            _counter) && result;

        d_mutex_unlock(&mutex);
        d_mutex_destroy(&mutex);
    }

    // test 3: lock provides mutual exclusion (tested with trylock)
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_mutex_lock(&mutex);

        int try_result = d_mutex_trylock(&mutex);
        result         = d_assert_standalone(
            try_result == D_MUTEX_BUSY,
            "lock_exclusion",
            "Locked mutex should reject trylock",
            _counter) && result;

        d_mutex_unlock(&mutex);
        d_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_mutex_unlock
  Tests the d_mutex_unlock function.
  Tests the following:
  - NULL mutex pointer handling
  - successful unlock
  - unlock allows reacquisition
*/
bool
d_tests_sa_mutex_unlock
(
    struct d_test_counter* _counter
)
{
    bool      result;
    d_mutex_t mutex;
    int       unlock_result;

    result = true;

    // test 1: NULL mutex should fail
    unlock_result = d_mutex_unlock(NULL);
    result        = d_assert_standalone(
        unlock_result != D_MUTEX_SUCCESS,
        "unlock_null_mutex",
        "Unlocking NULL mutex should fail",
        _counter) && result;

    // test 2: successful unlock after lock
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_mutex_lock(&mutex);

        unlock_result = d_mutex_unlock(&mutex);
        result        = d_assert_standalone(
            unlock_result == D_MUTEX_SUCCESS,
            "unlock_success",
            "Unlocking locked mutex should succeed",
            _counter) && result;

        d_mutex_destroy(&mutex);
    }

    // test 3: unlock allows reacquisition
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_mutex_lock(&mutex);
        d_mutex_unlock(&mutex);

        int relock_result = d_mutex_trylock(&mutex);
        result            = d_assert_standalone(
            relock_result == D_MUTEX_SUCCESS,
            "unlock_allows_reacquire",
            "Unlocked mutex should be acquirable",
            _counter) && result;

        if (relock_result == D_MUTEX_SUCCESS)
        {
            d_mutex_unlock(&mutex);
        }

        d_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_mutex_trylock
  Tests the d_mutex_trylock function.
  Tests the following:
  - NULL mutex pointer handling
  - successful trylock on available mutex
  - trylock fails on locked mutex
  - trylock doesn't block
*/
bool
d_tests_sa_mutex_trylock
(
    struct d_test_counter* _counter
)
{
    bool      result;
    d_mutex_t mutex;
    int       try_result;

    result = true;

    // test 1: NULL mutex should fail
    try_result = d_mutex_trylock(NULL);
    result     = d_assert_standalone(
        try_result != D_MUTEX_SUCCESS,
        "trylock_null_mutex",
        "Trylock on NULL mutex should fail",
        _counter) && result;

    // test 2: successful trylock on available mutex
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        try_result = d_mutex_trylock(&mutex);
        result     = d_assert_standalone(
            try_result == D_MUTEX_SUCCESS,
            "trylock_available",
            "Trylock on available mutex should succeed",
            _counter) && result;

        if (try_result == D_MUTEX_SUCCESS)
        {
            d_mutex_unlock(&mutex);
        }

        d_mutex_destroy(&mutex);
    }

    // test 3: trylock fails on locked mutex
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_mutex_lock(&mutex);

        try_result = d_mutex_trylock(&mutex);
        result     = d_assert_standalone(
            try_result == D_MUTEX_BUSY,
            "trylock_locked",
            "Trylock on locked mutex should return BUSY",
            _counter) && result;

        d_mutex_unlock(&mutex);
        d_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_mutex_timedlock
  Tests the d_mutex_timedlock function.
  Tests the following:
  - NULL mutex pointer handling
  - NULL timespec handling
  - successful timedlock on available mutex
  - timeout on locked mutex
*/
bool
d_tests_sa_mutex_timedlock
(
    struct d_test_counter* _counter
)
{
    bool             result;
    d_mutex_t        mutex;
    struct timespec  timeout;
    int              timed_result;

    result = true;

    // test 1: NULL mutex should fail
    d_clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_sec += 1;

    timed_result = d_mutex_timedlock(NULL, &timeout);
    result       = d_assert_standalone(
        timed_result != D_MUTEX_SUCCESS,
        "timedlock_null_mutex",
        "Timedlock on NULL mutex should fail",
        _counter) && result;

    // test 2: NULL timespec handling
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        timed_result = d_mutex_timedlock(&mutex, NULL);
        result       = d_assert_standalone(
            timed_result != D_MUTEX_SUCCESS,
            "timedlock_null_timeout",
            "Timedlock with NULL timeout should fail",
            _counter) && result;

        d_mutex_destroy(&mutex);
    }

    // test 3: successful timedlock on available mutex
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 1;

        timed_result = d_mutex_timedlock(&mutex, &timeout);
        result       = d_assert_standalone(
            timed_result == D_MUTEX_SUCCESS,
            "timedlock_available",
            "Timedlock on available mutex should succeed",
            _counter) && result;

        if (timed_result == D_MUTEX_SUCCESS)
        {
            d_mutex_unlock(&mutex);
        }

        d_mutex_destroy(&mutex);
    }

    // test 4: timeout on locked mutex
    if (d_mutex_init(&mutex) == D_MUTEX_SUCCESS)
    {
        d_mutex_lock(&mutex);

        // set very short timeout (100ms)
        d_clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_nsec += 100000000;  // 100ms
        if (timeout.tv_nsec >= 1000000000)
        {
            timeout.tv_sec++;
            timeout.tv_nsec -= 1000000000;
        }

        timed_result = d_mutex_timedlock(&mutex, &timeout);
        result       = d_assert_standalone(
            timed_result == D_MUTEX_TIMEDOUT,
            "timedlock_timeout",
            "Timedlock on locked mutex should timeout",
            _counter) && result;

        d_mutex_unlock(&mutex);
        d_mutex_destroy(&mutex);
    }

    return result;
}


/*
d_tests_sa_mutex_basic_all
  Aggregation function that runs all basic mutex tests.
*/
bool
d_tests_sa_mutex_basic_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Basic Mutex Operations\n");
    printf("  ---------------------------------\n");

    result = d_tests_sa_mutex_init(_counter) && result;
    result = d_tests_sa_mutex_destroy(_counter) && result;
    result = d_tests_sa_mutex_lock(_counter) && result;
    result = d_tests_sa_mutex_unlock(_counter) && result;
    result = d_tests_sa_mutex_trylock(_counter) && result;
    result = d_tests_sa_mutex_timedlock(_counter) && result;

    return result;
}
