#include ".\datomic_tests_sa.h"


bool d_tests_sa_atomic_compare_exchange_strong_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    int expected;

    d_atomic_init_int(&val, 100);
    expected = 100;

    result = d_assert_standalone(
        d_atomic_compare_exchange_strong_int(&val, &expected, 200),
        "cas_strong_int_success",
        "CAS should succeed when expected matches",
        _counter) && result;

    result = d_assert_standalone(
        d_atomic_load_int(&val) == 200,
        "cas_strong_int_new_value",
        "CAS should set new value to 200",
        _counter) && result;

    expected = 100;
    result = d_assert_standalone(
        !d_atomic_compare_exchange_strong_int(&val, &expected, 300),
        "cas_strong_int_fail",
        "CAS should fail when expected doesn't match",
        _counter) && result;

    result = d_assert_standalone(
        expected == 200,
        "cas_strong_int_expected_updated",
        "Expected should be updated to current value on failure",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_compare_exchange_weak_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    int expected;
    bool cas_result;
    int attempts = 0;

    d_atomic_init_int(&val, 50);
    expected = 50;

    // weak may spuriously fail, so retry
    do {
        expected = 50;
        cas_result = d_atomic_compare_exchange_weak_int(&val, &expected, 75);
        attempts++;
    } while (!cas_result && attempts < 100);

    result = d_assert_standalone(
        cas_result && d_atomic_load_int(&val) == 75,
        "cas_weak_int_eventually_succeeds",
        "CAS weak should eventually succeed",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_compare_exchange_strong_uint(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_uint val;
    unsigned int expected;

    d_atomic_init_uint(&val, 10);
    expected = 10;

    result = d_assert_standalone(
        d_atomic_compare_exchange_strong_uint(&val, &expected, 20),
        "cas_strong_uint",
        "CAS strong uint should work",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_compare_exchange_strong_long(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_long val;
    long expected;

    d_atomic_init_long(&val, 30L);
    expected = 30L;

    result = d_assert_standalone(
        d_atomic_compare_exchange_strong_long(&val, &expected, 40L),
        "cas_strong_long",
        "CAS strong long should work",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_compare_exchange_strong_llong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_llong val;
    long long expected;

    d_atomic_init_llong(&val, 1234LL);
    expected = 1234LL;

    result = d_assert_standalone(
        d_atomic_compare_exchange_strong_llong(&val, &expected, 5678LL),
        "cas_strong_llong",
        "CAS strong llong should work",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_compare_exchange_strong_ptr(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ptr val;
    int dummy1 = 1, dummy2 = 2;
    void* expected;

    d_atomic_init_ptr(&val, &dummy1);
    expected = &dummy1;

    result = d_assert_standalone(
        d_atomic_compare_exchange_strong_ptr(&val, &expected, &dummy2),
        "cas_strong_ptr",
        "CAS strong ptr should work",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_compare_exchange_strong_size(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_size_t val;
    size_t expected;

    d_atomic_init_size(&val, 999);
    expected = 999;

    result = d_assert_standalone(
        d_atomic_compare_exchange_strong_size(&val, &expected, 888),
        "cas_strong_size",
        "CAS strong size should work",
        _counter) && result;

    return result;
}

struct cas_thread_data
{
    d_atomic_int counter;
    int target;
};

static d_thread_result_t cas_thread_func(void* _arg)
{
    struct cas_thread_data* data = (struct cas_thread_data*)_arg;
    int i;

    for (i = 0; i < 1000; i++)
    {
        int expected = d_atomic_load_int(&data->counter);
        while (!d_atomic_compare_exchange_weak_int(&data->counter, &expected, expected + 1))
        {
            expected = d_atomic_load_int(&data->counter);
        }
    }

#if D_MUTEX_HAS_C11_THREADS
    return 0;
#else
    return NULL;
#endif
}

bool d_tests_sa_atomic_compare_exchange_multithreaded(struct d_test_counter* _counter)
{
    bool result = true;
    struct cas_thread_data data;
    d_thread_t threads[4];
    int i;

    d_atomic_init_int(&data.counter, 0);
    data.target = 4000;

    for (i = 0; i < 4; i++)
    {
        d_thread_create(&threads[i], cas_thread_func, &data);
    }

    for (i = 0; i < 4; i++)
    {
        d_thread_join(threads[i], NULL);
    }

    int final = d_atomic_load_int(&data.counter);
    result = d_assert_standalone(
        final == 4000,
        "cas_multithreaded",
        "CAS should provide lock-free synchronization (count should be 4000)",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_compare_exchange_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Atomic Compare-and-Exchange\n");
    printf("  --------------------------------------\n");

    result = d_tests_sa_atomic_compare_exchange_strong_int(_counter) && result;
    result = d_tests_sa_atomic_compare_exchange_weak_int(_counter) && result;
    result = d_tests_sa_atomic_compare_exchange_strong_uint(_counter) && result;
    result = d_tests_sa_atomic_compare_exchange_strong_long(_counter) && result;
    result = d_tests_sa_atomic_compare_exchange_strong_llong(_counter) && result;
    result = d_tests_sa_atomic_compare_exchange_strong_ptr(_counter) && result;
    result = d_tests_sa_atomic_compare_exchange_strong_size(_counter) && result;
    result = d_tests_sa_atomic_compare_exchange_multithreaded(_counter) && result;

    return result;
}
