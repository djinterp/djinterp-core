#include ".\datomic_tests_sa.h"


bool d_tests_sa_atomic_fetch_add_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    int old;

    d_atomic_init_int(&val, 10);
    old = d_atomic_fetch_add_int(&val, 5);

    result = d_assert_standalone(old == 10,
        "fetch_add_int_old", "Fetch-add should return old value 10", _counter) && result;
    result = d_assert_standalone(d_atomic_load_int(&val) == 15,
        "fetch_add_int_new", "Fetch-add should result in 15", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_fetch_add_uint(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_uint val;
    d_atomic_init_uint(&val, 20);
    unsigned int old = d_atomic_fetch_add_uint(&val, 10);
    result = d_assert_standalone(old == 20 && d_atomic_load_uint(&val) == 30,
        "fetch_add_uint", "Fetch-add uint should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_add_long(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_long val;
    d_atomic_init_long(&val, 100L);
    long old = d_atomic_fetch_add_long(&val, 50L);
    result = d_assert_standalone(old == 100L && d_atomic_load_long(&val) == 150L,
        "fetch_add_long", "Fetch-add long should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_add_llong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_llong val;
    d_atomic_init_llong(&val, 1000LL);
    long long old = d_atomic_fetch_add_llong(&val, 500LL);
    result = d_assert_standalone(old == 1000LL && d_atomic_load_llong(&val) == 1500LL,
        "fetch_add_llong", "Fetch-add llong should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_add_size(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_size_t val;
    d_atomic_init_size(&val, 200);
    size_t old = d_atomic_fetch_add_size(&val, 100);
    result = d_assert_standalone(old == 200 && d_atomic_load_size(&val) == 300,
        "fetch_add_size", "Fetch-add size should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_sub_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    d_atomic_init_int(&val, 50);
    int old = d_atomic_fetch_sub_int(&val, 20);
    result = d_assert_standalone(old == 50 && d_atomic_load_int(&val) == 30,
        "fetch_sub_int", "Fetch-sub int should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_sub_long(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_long val;
    d_atomic_init_long(&val, 300L);
    long old = d_atomic_fetch_sub_long(&val, 100L);
    result = d_assert_standalone(old == 300L && d_atomic_load_long(&val) == 200L,
        "fetch_sub_long", "Fetch-sub long should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_or_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    d_atomic_init_int(&val, 0x0F);
    int old = d_atomic_fetch_or_int(&val, 0xF0);
    result = d_assert_standalone(old == 0x0F && d_atomic_load_int(&val) == 0xFF,
        "fetch_or_int", "Fetch-or int should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_xor_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    d_atomic_init_int(&val, 0xFF);
    int old = d_atomic_fetch_xor_int(&val, 0x0F);
    result = d_assert_standalone(old == 0xFF && d_atomic_load_int(&val) == 0xF0,
        "fetch_xor_int", "Fetch-xor int should work", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_and_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    d_atomic_init_int(&val, 0xFF);
    int old = d_atomic_fetch_and_int(&val, 0x0F);
    result = d_assert_standalone(old == 0xFF && d_atomic_load_int(&val) == 0x0F,
        "fetch_and_int", "Fetch-and int should work", _counter) && result;
    return result;
}

struct fetch_thread_data { d_atomic_int counter; };

static d_thread_result_t fetch_thread_func(void* _arg)
{
    struct fetch_thread_data* data = (struct fetch_thread_data*)_arg;
    int i;
    for (i = 0; i < 1000; i++) d_atomic_fetch_add_int(&data->counter, 1);
#if D_MUTEX_HAS_C11_THREADS
    return 0;
#else
    return NULL;
#endif
}

bool d_tests_sa_atomic_fetch_multithreaded(struct d_test_counter* _counter)
{
    bool result = true;
    struct fetch_thread_data data;
    d_thread_t threads[4];
    int i;

    d_atomic_init_int(&data.counter, 0);
    for (i = 0; i < 4; i++) d_thread_create(&threads[i], fetch_thread_func, &data);
    for (i = 0; i < 4; i++) d_thread_join(threads[i], NULL);

    result = d_assert_standalone(d_atomic_load_int(&data.counter) == 4000,
        "fetch_multithreaded", "Fetch-add should be thread-safe (count=4000)", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_fetch_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Atomic Fetch-and-Modify\n");
    printf("  ----------------------------------\n");

    result = d_tests_sa_atomic_fetch_add_int(_counter) && result;
    result = d_tests_sa_atomic_fetch_add_uint(_counter) && result;
    result = d_tests_sa_atomic_fetch_add_long(_counter) && result;
    result = d_tests_sa_atomic_fetch_add_llong(_counter) && result;
    result = d_tests_sa_atomic_fetch_add_size(_counter) && result;
    result = d_tests_sa_atomic_fetch_sub_int(_counter) && result;
    result = d_tests_sa_atomic_fetch_sub_long(_counter) && result;
    result = d_tests_sa_atomic_fetch_or_int(_counter) && result;
    result = d_tests_sa_atomic_fetch_xor_int(_counter) && result;
    result = d_tests_sa_atomic_fetch_and_int(_counter) && result;
    result = d_tests_sa_atomic_fetch_multithreaded(_counter) && result;

    return result;
}
