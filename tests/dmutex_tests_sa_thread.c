#include ".\dmutex_tests_sa.h"


// helper thread function for testing
static d_thread_result_t
simple_thread_func
(
    void* _arg
)
{
    int* value = (int*)_arg;
    *value     = 42;

#if D_MUTEX_HAS_C11_THREADS
    return 0;
#else
    return NULL;
#endif
}

// helper thread function that returns a value
static d_thread_result_t
return_value_thread_func
(
    void* _arg
)
{
    (void)_arg;

#if D_MUTEX_HAS_C11_THREADS
    return 123;
#else
    return (void*)(uintptr_t)123;
#endif
}

// helper thread function that sleeps
static d_thread_result_t
sleep_thread_func
(
    void* _arg
)
{
    struct timespec sleep_time;

    (void)_arg;

    sleep_time.tv_sec  = 0;
    sleep_time.tv_nsec = 100000000;  // 100ms

    d_thread_sleep(&sleep_time, NULL);

#if D_MUTEX_HAS_C11_THREADS
    return 0;
#else
    return NULL;
#endif
}


/*
d_tests_sa_thread_create
  Tests the d_thread_create function.
  Tests the following:
  - NULL thread pointer handling
  - NULL function pointer handling
  - successful thread creation
  - thread executes function
*/
bool
d_tests_sa_thread_create
(
    struct d_test_counter* _counter
)
{
    bool       result;
    d_thread_t thread;
    int        value;
    int        create_result;

    result = true;

    // test 1: NULL thread pointer should fail
    create_result = d_thread_create(NULL, simple_thread_func, &value);
    result        = d_assert_standalone(
        create_result != D_MUTEX_SUCCESS,
        "thread_create_null_thread",
        "NULL thread pointer should fail",
        _counter) && result;

    // test 2: NULL function pointer should fail
    create_result = d_thread_create(&thread, NULL, &value);
    result        = d_assert_standalone(
        create_result != D_MUTEX_SUCCESS,
        "thread_create_null_func",
        "NULL function pointer should fail",
        _counter) && result;

    // test 3: successful thread creation and execution
    value         = 0;
    create_result = d_thread_create(&thread, simple_thread_func, &value);
    result        = d_assert_standalone(
        create_result == D_MUTEX_SUCCESS,
        "thread_create_success",
        "Thread creation should succeed",
        _counter) && result;

    if (create_result == D_MUTEX_SUCCESS)
    {
        d_thread_join(thread, NULL);

        // test 4: thread executed function
        result = d_assert_standalone(
            value == 42,
            "thread_create_executed",
            "Thread should have executed function and set value to 42",
            _counter) && result;
    }

    return result;
}


/*
d_tests_sa_thread_join
  Tests the d_thread_join function.
  Tests the following:
  - successful join
  - retrieves return value
*/
bool
d_tests_sa_thread_join
(
    struct d_test_counter* _counter
)
{
    bool               result;
    d_thread_t         thread;
    d_thread_result_t  ret_val;
    int                join_result;

    result = true;

    // test 1: successful join
    if (d_thread_create(&thread, sleep_thread_func, NULL) == D_MUTEX_SUCCESS)
    {
        join_result = d_thread_join(thread, NULL);
        result      = d_assert_standalone(
            join_result == D_MUTEX_SUCCESS,
            "thread_join_success",
            "Thread join should succeed",
            _counter) && result;
    }

    // test 2: join retrieves return value
    if (d_thread_create(&thread, return_value_thread_func, NULL) == D_MUTEX_SUCCESS)
    {
        ret_val = D_THREAD_ERROR;

        d_thread_join(thread, &ret_val);

#if D_MUTEX_HAS_C11_THREADS
        result = d_assert_standalone(
            ret_val == 123,
            "thread_join_return_value",
            "Join should retrieve return value 123",
            _counter) && result;
#else
        result = d_assert_standalone(
            (uintptr_t)ret_val == 123,
            "thread_join_return_value",
            "Join should retrieve return value 123",
            _counter) && result;
#endif
    }

    return result;
}


/*
d_tests_sa_thread_detach
  Tests the d_thread_detach function.
  Tests the following:
  - successful detach
  - thread runs independently after detach
*/
bool
d_tests_sa_thread_detach
(
    struct d_test_counter* _counter
)
{
    bool       result;
    d_thread_t thread;
    int        value;
    int        detach_result;

    result = true;

    // test 1: successful detach
    value = 0;

    if (d_thread_create(&thread, simple_thread_func, &value) == D_MUTEX_SUCCESS)
    {
        detach_result = d_thread_detach(thread);
        result        = d_assert_standalone(
            detach_result == D_MUTEX_SUCCESS,
            "thread_detach_success",
            "Thread detach should succeed",
            _counter) && result;

        // give detached thread time to run
        {
            struct timespec sleep_time;

            sleep_time.tv_sec  = 0;
            sleep_time.tv_nsec = 200000000;  // 200ms

            d_thread_sleep(&sleep_time, NULL);
        }

        // test 2: thread ran independently
        result = d_assert_standalone(
            value == 42,
            "thread_detach_independent",
            "Detached thread should have executed",
            _counter) && result;
    }

    return result;
}


/*
d_tests_sa_thread_exit
  Tests the d_thread_exit function.
  Tests the following:
  - thread can exit with value
  - exit value is retrievable
*/
static d_thread_result_t
exit_thread_func
(
    void* _arg
)
{
    (void)_arg;

#if D_MUTEX_HAS_C11_THREADS
    d_thread_exit(99);
#else
    d_thread_exit((void*)(uintptr_t)99);
#endif

    // should not reach here
#if D_MUTEX_HAS_C11_THREADS
    return -1;
#else
    return (void*)-1;
#endif
}

bool
d_tests_sa_thread_exit
(
    struct d_test_counter* _counter
)
{
    bool               result;
    d_thread_t         thread;
    d_thread_result_t  ret_val;

    result = true;

    // test 1: thread exits with value
    if (d_thread_create(&thread, exit_thread_func, NULL) == D_MUTEX_SUCCESS)
    {
        ret_val = D_THREAD_ERROR;

        d_thread_join(thread, &ret_val);

#if D_MUTEX_HAS_C11_THREADS
        result = d_assert_standalone(
            ret_val == 99,
            "thread_exit_value",
            "Thread exit should return value 99",
            _counter) && result;
#else
        result = d_assert_standalone(
            (uintptr_t)ret_val == 99,
            "thread_exit_value",
            "Thread exit should return value 99",
            _counter) && result;
#endif
    }

    return result;
}


/*
d_tests_sa_thread_yield
  Tests the d_thread_yield function.
  Tests the following:
  - yield doesn't crash
  - yield can be called multiple times
*/
bool
d_tests_sa_thread_yield
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: yield doesn't crash
    d_thread_yield();

    result = d_assert_standalone(
        true,
        "thread_yield_no_crash",
        "Thread yield should not crash",
        _counter) && result;

    // test 2: can be called multiple times
    d_thread_yield();
    d_thread_yield();
    d_thread_yield();

    result = d_assert_standalone(
        true,
        "thread_yield_multiple",
        "Multiple yields should not crash",
        _counter) && result;

    return result;
}


/*
d_tests_sa_thread_sleep
  Tests the d_thread_sleep function.
  Tests the following:
  - NULL duration handling
  - sleep actually delays execution
  - sleep returns correct status
*/
bool
d_tests_sa_thread_sleep
(
    struct d_test_counter* _counter
)
{
    bool            result;
    struct timespec duration;
    struct timespec remaining;
    struct timespec start;
    struct timespec end;
    int             sleep_result;
    int64_t         elapsed_ms;

    result = true;

    // test 1: NULL duration handling
    sleep_result = d_thread_sleep(NULL, NULL);
    result       = d_assert_standalone(
        sleep_result != D_MUTEX_SUCCESS,
        "thread_sleep_null_duration",
        "Sleep with NULL duration should fail",
        _counter) && result;

    // test 2: sleep delays execution
    duration.tv_sec  = 0;
    duration.tv_nsec = 100000000;  // 100ms

    d_clock_gettime(CLOCK_MONOTONIC, &start);
    sleep_result = d_thread_sleep(&duration, &remaining);
    d_clock_gettime(CLOCK_MONOTONIC, &end);

    result = d_assert_standalone(
        sleep_result == D_MUTEX_SUCCESS,
        "thread_sleep_success",
        "Sleep should succeed",
        _counter) && result;

    // verify sleep duration (allow some tolerance)
    elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 +
                 (end.tv_nsec - start.tv_nsec) / 1000000;

    result = d_assert_standalone(
        elapsed_ms >= 90 && elapsed_ms <= 200,
        "thread_sleep_duration",
        "Sleep should delay for approximately 100ms",
        _counter) && result;

    return result;
}


/*
d_tests_sa_thread_current
  Tests the d_thread_current function.
  Tests the following:
  - returns valid thread ID
  - same thread returns same ID
*/
bool
d_tests_sa_thread_current
(
    struct d_test_counter* _counter
)
{
    bool       result;
    d_thread_t current1;
    d_thread_t current2;

    result = true;

    // test 1: returns valid thread ID (doesn't crash)
    current1 = d_thread_current();

    result = d_assert_standalone(
        true,
        "thread_current_valid",
        "Thread current should return valid ID",
        _counter) && result;

    // test 2: same thread returns same ID
    current2 = d_thread_current();

    result = d_assert_standalone(
        d_thread_equal(current1, current2),
        "thread_current_consistent",
        "Same thread should return consistent ID",
        _counter) && result;

    return result;
}


/*
d_tests_sa_thread_equal
  Tests the d_thread_equal function.
  Tests the following:
  - thread equals itself
  - different threads are not equal
*/
bool
d_tests_sa_thread_equal
(
    struct d_test_counter* _counter
)
{
    bool       result;
    d_thread_t thread1;
    d_thread_t thread2;
    d_thread_t current;
    int        value1;
    int        value2;

    result = true;

    // test 1: thread equals itself
    current = d_thread_current();

    result = d_assert_standalone(
        d_thread_equal(current, current),
        "thread_equal_self",
        "Thread should equal itself",
        _counter) && result;

    // test 2: different threads are not equal
    value1 = 0;
    value2 = 0;

    if ( (d_thread_create(&thread1, simple_thread_func, &value1) == D_MUTEX_SUCCESS) &&
         (d_thread_create(&thread2, simple_thread_func, &value2) == D_MUTEX_SUCCESS) )
    {
        result = d_assert_standalone(
            !d_thread_equal(thread1, thread2),
            "thread_equal_different",
            "Different threads should not be equal",
            _counter) && result;

        d_thread_join(thread1, NULL);
        d_thread_join(thread2, NULL);
    }

    return result;
}


/*
d_tests_sa_mutex_thread_all
  Aggregation function that runs all thread operation tests.
*/
bool
d_tests_sa_mutex_thread_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Thread Operations\n");
    printf("  ---------------------------\n");

    result = d_tests_sa_thread_create(_counter) && result;
    result = d_tests_sa_thread_join(_counter) && result;
    result = d_tests_sa_thread_detach(_counter) && result;
    result = d_tests_sa_thread_exit(_counter) && result;
    result = d_tests_sa_thread_yield(_counter) && result;
    result = d_tests_sa_thread_sleep(_counter) && result;
    result = d_tests_sa_thread_current(_counter) && result;
    result = d_tests_sa_thread_equal(_counter) && result;

    return result;
}
