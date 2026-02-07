#include ".\datomic_tests_sa.h"


bool d_tests_sa_atomic_exchange_int(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_int val;
    int old;

    d_atomic_init_int(&val, 100);
    old = d_atomic_exchange_int(&val, 200);

    result = d_assert_standalone(old == 100,
        "exchange_int_old", "Exchange should return old value 100", _counter) && result;
    result = d_assert_standalone(d_atomic_load_int(&val) == 200,
        "exchange_int_new", "Exchange should set new value 200", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_uint(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_uint val;
    unsigned int old;

    d_atomic_init_uint(&val, 50);
    old = d_atomic_exchange_uint(&val, 150);

    result = d_assert_standalone(old == 50 && d_atomic_load_uint(&val) == 150,
        "exchange_uint", "Exchange uint should work correctly", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_long(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_long val;
    long old;

    d_atomic_init_long(&val, -50L);
    old = d_atomic_exchange_long(&val, 75L);

    result = d_assert_standalone(old == -50L && d_atomic_load_long(&val) == 75L,
        "exchange_long", "Exchange long should work correctly", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_ulong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ulong val;
    unsigned long old;

    d_atomic_init_ulong(&val, 100UL);
    old = d_atomic_exchange_ulong(&val, 300UL);

    result = d_assert_standalone(old == 100UL && d_atomic_load_ulong(&val) == 300UL,
        "exchange_ulong", "Exchange ulong should work correctly", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_llong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_llong val;
    long long old;

    d_atomic_init_llong(&val, 1000LL);
    old = d_atomic_exchange_llong(&val, 2000LL);

    result = d_assert_standalone(old == 1000LL && d_atomic_load_llong(&val) == 2000LL,
        "exchange_llong", "Exchange llong should work correctly", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_ullong(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ullong val;
    unsigned long long old;

    d_atomic_init_ullong(&val, 5000ULL);
    old = d_atomic_exchange_ullong(&val, 6000ULL);

    result = d_assert_standalone(old == 5000ULL && d_atomic_load_ullong(&val) == 6000ULL,
        "exchange_ullong", "Exchange ullong should work correctly", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_ptr(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_ptr val;
    int dummy1 = 1, dummy2 = 2;
    void* old;

    d_atomic_init_ptr(&val, &dummy1);
    old = d_atomic_exchange_ptr(&val, &dummy2);

    result = d_assert_standalone(old == &dummy1 && d_atomic_load_ptr(&val) == &dummy2,
        "exchange_ptr", "Exchange ptr should work correctly", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_size(struct d_test_counter* _counter)
{
    bool result = true;
    d_atomic_size_t val;
    size_t old;

    d_atomic_init_size(&val, 111);
    old = d_atomic_exchange_size(&val, 222);

    result = d_assert_standalone(old == 111 && d_atomic_load_size(&val) == 222,
        "exchange_size", "Exchange size should work correctly", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_exchange_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Atomic Exchange Operations\n");
    printf("  -------------------------------------\n");

    result = d_tests_sa_atomic_exchange_int(_counter) && result;
    result = d_tests_sa_atomic_exchange_uint(_counter) && result;
    result = d_tests_sa_atomic_exchange_long(_counter) && result;
    result = d_tests_sa_atomic_exchange_ulong(_counter) && result;
    result = d_tests_sa_atomic_exchange_llong(_counter) && result;
    result = d_tests_sa_atomic_exchange_ullong(_counter) && result;
    result = d_tests_sa_atomic_exchange_ptr(_counter) && result;
    result = d_tests_sa_atomic_exchange_size(_counter) && result;

    return result;
}
