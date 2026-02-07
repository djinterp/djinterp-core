#include ".\datomic_tests_sa.h"


bool d_tests_sa_atomic_load_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;

    d_atomic_init_int(&val, 100);
    result = d_assert_standalone(
        d_atomic_load_int(&val) == 100,
        "load_int",
        "Load int should return 100",
        _counter) && result;

    d_atomic_store_int(&val, -50);
    result = d_assert_standalone(
        d_atomic_load_int(&val) == -50,
        "load_int_after_store",
        "Load int should return -50 after store",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_uint(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_uint val;

    d_atomic_init_uint(&val, 200);
    result = d_assert_standalone(
        d_atomic_load_uint(&val) == 200,
        "load_uint",
        "Load uint should return 200",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_long(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_long val;

    d_atomic_init_long(&val, 300L);
    result = d_assert_standalone(
        d_atomic_load_long(&val) == 300L,
        "load_long",
        "Load long should return 300",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_ulong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ulong val;

    d_atomic_init_ulong(&val, 400UL);
    result = d_assert_standalone(
        d_atomic_load_ulong(&val) == 400UL,
        "load_ulong",
        "Load ulong should return 400",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_llong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_llong val;

    d_atomic_init_llong(&val, 500LL);
    result = d_assert_standalone(
        d_atomic_load_llong(&val) == 500LL,
        "load_llong",
        "Load llong should return 500",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_ullong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ullong val;

    d_atomic_init_ullong(&val, 600ULL);
    result = d_assert_standalone(
        d_atomic_load_ullong(&val) == 600ULL,
        "load_ullong",
        "Load ullong should return 600",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_ptr(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ptr val;
    int dummy = 42;

    d_atomic_init_ptr(&val, &dummy);
    result = d_assert_standalone(
        d_atomic_load_ptr(&val) == &dummy,
        "load_ptr",
        "Load ptr should return pointer to dummy",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_size(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_size_t val;

    d_atomic_init_size(&val, 700);
    result = d_assert_standalone(
        d_atomic_load_size(&val) == 700,
        "load_size",
        "Load size should return 700",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_explicit(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;

    d_atomic_init_int(&val, 123);

    result = d_assert_standalone(
        d_atomic_load_int_explicit(&val, D_MEMORY_ORDER_RELAXED) == 123,
        "load_explicit_relaxed",
        "Load with relaxed order should work",
        _counter) && result;

    result = d_assert_standalone(
        d_atomic_load_int_explicit(&val, D_MEMORY_ORDER_ACQUIRE) == 123,
        "load_explicit_acquire",
        "Load with acquire order should work",
        _counter) && result;

    result = d_assert_standalone(
        d_atomic_load_int_explicit(&val, D_MEMORY_ORDER_SEQ_CST) == 123,
        "load_explicit_seq_cst",
        "Load with seq_cst order should work",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_load_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Atomic Load Operations\n");
    printf("  ---------------------------------\n");

    result = d_tests_sa_atomic_load_int(_counter) && result;
    result = d_tests_sa_atomic_load_uint(_counter) && result;
    result = d_tests_sa_atomic_load_long(_counter) && result;
    result = d_tests_sa_atomic_load_ulong(_counter) && result;
    result = d_tests_sa_atomic_load_llong(_counter) && result;
    result = d_tests_sa_atomic_load_ullong(_counter) && result;
    result = d_tests_sa_atomic_load_ptr(_counter) && result;
    result = d_tests_sa_atomic_load_size(_counter) && result;
    result = d_tests_sa_atomic_load_explicit(_counter) && result;

    return result;
}
