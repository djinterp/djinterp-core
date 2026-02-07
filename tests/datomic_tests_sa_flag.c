#include ".\datomic_tests_sa.h"


bool d_tests_sa_atomic_flag_test_and_set(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_flag flag = D_ATOMIC_FLAG_INIT;

    // test 1: initial test_and_set should return false
    result = d_assert_standalone(
        !d_atomic_flag_test_and_set(&flag),
        "flag_test_and_set_initial",
        "Initial flag should be clear (return false)",
        _counter) && result;

    // test 2: second test_and_set should return true
    result = d_assert_standalone(
        d_atomic_flag_test_and_set(&flag),
        "flag_test_and_set_set",
        "Flag should be set (return true)",
        _counter) && result;

    // test 3: flag remains set
    result = d_assert_standalone(
        d_atomic_flag_test_and_set(&flag),
        "flag_test_and_set_remains",
        "Flag should remain set",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_flag_clear(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_flag flag = D_ATOMIC_FLAG_INIT;

    // set the flag first
    d_atomic_flag_test_and_set(&flag);

    // test 1: clear the flag
    d_atomic_flag_clear(&flag);

    // test 2: test_and_set after clear should return false
    result = d_assert_standalone(
        !d_atomic_flag_test_and_set(&flag),
        "flag_clear_works",
        "Flag should be clear after clear()",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_flag_explicit(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_flag flag = D_ATOMIC_FLAG_INIT;

    // test with explicit memory order
    result = d_assert_standalone(
        !d_atomic_flag_test_and_set_explicit(&flag, D_MEMORY_ORDER_SEQ_CST),
        "flag_explicit_test_and_set",
        "Explicit test_and_set should work",
        _counter) && result;

    d_atomic_flag_clear_explicit(&flag, D_MEMORY_ORDER_SEQ_CST);

    result = d_assert_standalone(
        !d_atomic_flag_test_and_set(&flag),
        "flag_explicit_clear",
        "Explicit clear should work",
        _counter) && result;

    return result;
}

struct flag_thread_data
{
    d_atomic_flag flag;
    d_atomic_int counter;
};

static d_thread_result_t flag_thread_func(void* _arg)
{
    struct flag_thread_data* data = (struct flag_thread_data*)_arg;
    int i;

    for (i = 0; i < 1000; i++)
    {
        while (d_atomic_flag_test_and_set(&data->flag))
        {
            // spin
        }

        // critical section
        d_atomic_fetch_add_int(&data->counter, 1);

        d_atomic_flag_clear(&data->flag);
    }

#if D_MUTEX_HAS_C11_THREADS
    return 0;
#else
    return NULL;
#endif
}

bool d_tests_sa_atomic_flag_multithreaded(struct d_test_counter* _counter)
{
    bool result = true;
    struct flag_thread_data data;
    d_thread_t threads[4];
    int i;

    data.flag = (d_atomic_flag)D_ATOMIC_FLAG_INIT;
    d_atomic_init_int(&data.counter, 0);

    for (i = 0; i < 4; i++)
    {
        d_thread_create(&threads[i], flag_thread_func, &data);
    }

    for (i = 0; i < 4; i++)
    {
        d_thread_join(threads[i], NULL);
    }

    int final_count = d_atomic_load_int(&data.counter);
    result = d_assert_standalone(
        final_count == 4000,
        "flag_multithreaded",
        "Flag should provide mutual exclusion (count should be 4000)",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_flag_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Atomic Flag Operations\n");
    printf("  ---------------------------------\n");

    result = d_tests_sa_atomic_flag_test_and_set(_counter) && result;
    result = d_tests_sa_atomic_flag_clear(_counter) && result;
    result = d_tests_sa_atomic_flag_explicit(_counter) && result;
    result = d_tests_sa_atomic_flag_multithreaded(_counter) && result;

    return result;
}
