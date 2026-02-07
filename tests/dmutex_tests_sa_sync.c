#include ".\dmutex_tests_sa.h"


static int once_counter = 0;

static void once_init_func(void)
{
    once_counter++;
}

static d_thread_result_t
once_thread_func
(
    void* _arg
)
{
    d_once_flag_t* flag = (d_once_flag_t*)_arg;
    d_call_once(flag, once_init_func);

#if D_MUTEX_HAS_C11_THREADS
    return 0;
#else
    return NULL;
#endif
}


bool d_tests_sa_call_once(struct d_test_counter* _counter)
{
    bool result = true;
    d_once_flag_t flag = D_ONCE_FLAG_INIT;

    once_counter = 0;

    d_call_once(&flag, once_init_func);
    d_call_once(&flag, once_init_func);
    d_call_once(&flag, once_init_func);

    result = d_assert_standalone(once_counter == 1,
        "call_once_single", "Function should be called only once", _counter) && result;

    return result;
}

bool d_tests_sa_call_once_multithreaded(struct d_test_counter* _counter)
{
    bool result = true;
    d_once_flag_t flag = D_ONCE_FLAG_INIT;
    d_thread_t threads[5];
    int i;

    once_counter = 0;

    for (i = 0; i < 5; i++)
    {
        d_thread_create(&threads[i], once_thread_func, &flag);
    }

    for (i = 0; i < 5; i++)
    {
        d_thread_join(threads[i], NULL);
    }

    result = d_assert_standalone(once_counter == 1,
        "call_once_multithreaded", "Function should be called once across threads", _counter) && result;

    return result;
}

bool d_tests_sa_mutex_sync_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Synchronization Primitives\n");
    printf("  -------------------------------------\n");

    result = d_tests_sa_call_once(_counter) && result;
    result = d_tests_sa_call_once_multithreaded(_counter) && result;

    return result;
}
