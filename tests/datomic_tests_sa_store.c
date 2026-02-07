#include ".\datomic_tests_sa.h"


bool d_tests_sa_atomic_store_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    d_atomic_init_int(&val, 0);
    d_atomic_store_int(&val, 999);
    result = d_assert_standalone(d_atomic_load_int(&val) == 999,
        "store_int", "Store int should set value to 999", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_uint(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_uint val;
    d_atomic_init_uint(&val, 0);
    d_atomic_store_uint(&val, 888);
    result = d_assert_standalone(d_atomic_load_uint(&val) == 888,
        "store_uint", "Store uint should set value to 888", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_long(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_long val;
    d_atomic_init_long(&val, 0);
    d_atomic_store_long(&val, 777L);
    result = d_assert_standalone(d_atomic_load_long(&val) == 777L,
        "store_long", "Store long should set value to 777", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_ulong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ulong val;
    d_atomic_init_ulong(&val, 0);
    d_atomic_store_ulong(&val, 666UL);
    result = d_assert_standalone(d_atomic_load_ulong(&val) == 666UL,
        "store_ulong", "Store ulong should set value to 666", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_llong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_llong val;
    d_atomic_init_llong(&val, 0);
    d_atomic_store_llong(&val, 555LL);
    result = d_assert_standalone(d_atomic_load_llong(&val) == 555LL,
        "store_llong", "Store llong should set value to 555", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_ullong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ullong val;
    d_atomic_init_ullong(&val, 0);
    d_atomic_store_ullong(&val, 444ULL);
    result = d_assert_standalone(d_atomic_load_ullong(&val) == 444ULL,
        "store_ullong", "Store ullong should set value to 444", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_ptr(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ptr val;
    int dummy = 0;
    d_atomic_init_ptr(&val, NULL);
    d_atomic_store_ptr(&val, &dummy);
    result = d_assert_standalone(d_atomic_load_ptr(&val) == &dummy,
        "store_ptr", "Store ptr should set pointer", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_size(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_size_t val;
    d_atomic_init_size(&val, 0);
    d_atomic_store_size(&val, 333);
    result = d_assert_standalone(d_atomic_load_size(&val) == 333,
        "store_size", "Store size should set value to 333", _counter) && result;
    return result;
}

bool d_tests_sa_atomic_store_explicit(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    d_atomic_init_int(&val, 0);

    d_atomic_store_int_explicit(&val, 111, D_MEMORY_ORDER_RELAXED);
    result = d_assert_standalone(d_atomic_load_int(&val) == 111,
        "store_explicit_relaxed", "Store with relaxed order should work", _counter) && result;

    d_atomic_store_int_explicit(&val, 222, D_MEMORY_ORDER_RELEASE);
    result = d_assert_standalone(d_atomic_load_int(&val) == 222,
        "store_explicit_release", "Store with release order should work", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_store_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Atomic Store Operations\n");
    printf("  ----------------------------------\n");

    result = d_tests_sa_atomic_store_int(_counter) && result;
    result = d_tests_sa_atomic_store_uint(_counter) && result;
    result = d_tests_sa_atomic_store_long(_counter) && result;
    result = d_tests_sa_atomic_store_ulong(_counter) && result;
    result = d_tests_sa_atomic_store_llong(_counter) && result;
    result = d_tests_sa_atomic_store_ullong(_counter) && result;
    result = d_tests_sa_atomic_store_ptr(_counter) && result;
    result = d_tests_sa_atomic_store_size(_counter) && result;
    result = d_tests_sa_atomic_store_explicit(_counter) && result;

    return result;
}
