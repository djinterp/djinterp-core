#include ".\dmutex_tests_sa.h"


static d_thread_result_t
tss_thread_func
(
    void* _arg
)
{
    d_tss_t* key = (d_tss_t*)_arg;
    int value = 42;

    d_tss_set(*key, &value);

    int* retrieved = (int*)d_tss_get(*key);

#if D_MUTEX_HAS_C11_THREADS
    return (retrieved && *retrieved == 42) ? 0 : -1;
#else
    return (retrieved && *retrieved == 42) ? NULL : (void*)-1;
#endif
}


bool d_tests_sa_tss_create(struct d_test_counter* _counter)
{
    bool result = true;
    d_tss_t key;

    result = d_assert_standalone(d_tss_create(NULL, NULL) != D_MUTEX_SUCCESS,
        "tss_create_null", "NULL key should fail", _counter) && result;
    result = d_assert_standalone(d_tss_create(&key, NULL) == D_MUTEX_SUCCESS,
        "tss_create_success", "TSS create should succeed", _counter) && result;

    d_tss_delete(key);
    return result;
}

bool d_tests_sa_tss_delete(struct d_test_counter* _counter)
{
    bool result = true;
    d_tss_t key;

    if (d_tss_create(&key, NULL) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_tss_delete(key) == D_MUTEX_SUCCESS,
            "tss_delete_success", "TSS delete should succeed", _counter) && result;
    }
    return result;
}

bool d_tests_sa_tss_get(struct d_test_counter* _counter)
{
    bool result = true;
    d_tss_t key;

    if (d_tss_create(&key, NULL) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_tss_get(key) == NULL,
            "tss_get_initial", "Initial TSS value should be NULL", _counter) && result;
        d_tss_delete(key);
    }
    return result;
}

bool d_tests_sa_tss_set(struct d_test_counter* _counter)
{
    bool result = true;
    d_tss_t key;
    int value = 123;

    if (d_tss_create(&key, NULL) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_tss_set(key, &value) == D_MUTEX_SUCCESS,
            "tss_set_success", "TSS set should succeed", _counter) && result;

        int* retrieved = (int*)d_tss_get(key);
        result = d_assert_standalone(retrieved == &value && *retrieved == 123,
            "tss_set_retrieve", "Retrieved value should match set value", _counter) && result;

        d_tss_delete(key);
    }
    return result;
}

bool d_tests_sa_tss_multithreaded(struct d_test_counter* _counter)
{
    bool result = true;
    d_tss_t key;
    d_thread_t thread;
    d_thread_result_t ret_val;

    if (d_tss_create(&key, NULL) == D_MUTEX_SUCCESS)
    {
        if (d_thread_create(&thread, tss_thread_func, &key) == D_MUTEX_SUCCESS)
        {
            d_thread_join(thread, &ret_val);

#if D_MUTEX_HAS_C11_THREADS
            result = d_assert_standalone(ret_val == 0,
                "tss_multithreaded", "TSS should work per-thread", _counter) && result;
#else
            result = d_assert_standalone(ret_val == NULL,
                "tss_multithreaded", "TSS should work per-thread", _counter) && result;
#endif
        }
        d_tss_delete(key);
    }
    return result;
}

bool d_tests_sa_mutex_tss_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Thread-Specific Storage\n");
    printf("  ----------------------------------\n");

    result = d_tests_sa_tss_create(_counter) && result;
    result = d_tests_sa_tss_delete(_counter) && result;
    result = d_tests_sa_tss_get(_counter) && result;
    result = d_tests_sa_tss_set(_counter) && result;
    result = d_tests_sa_tss_multithreaded(_counter) && result;

    return result;
}
