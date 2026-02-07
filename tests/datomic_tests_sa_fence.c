#include ".\datomic_tests_sa.h"


bool d_tests_sa_atomic_thread_fence(struct d_test_counter* _counter)
{
    bool result = true;

    // fences don't crash
    d_atomic_thread_fence(D_MEMORY_ORDER_RELAXED);
    d_atomic_thread_fence(D_MEMORY_ORDER_ACQUIRE);
    d_atomic_thread_fence(D_MEMORY_ORDER_RELEASE);
    d_atomic_thread_fence(D_MEMORY_ORDER_ACQ_REL);
    d_atomic_thread_fence(D_MEMORY_ORDER_SEQ_CST);

    result = d_assert_standalone(true,
        "thread_fence_no_crash", "Thread fences should not crash", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_signal_fence(struct d_test_counter* _counter)
{
    bool result = true;

    // signal fences don't crash
    d_atomic_signal_fence(D_MEMORY_ORDER_RELAXED);
    d_atomic_signal_fence(D_MEMORY_ORDER_ACQUIRE);
    d_atomic_signal_fence(D_MEMORY_ORDER_RELEASE);
    d_atomic_signal_fence(D_MEMORY_ORDER_ACQ_REL);
    d_atomic_signal_fence(D_MEMORY_ORDER_SEQ_CST);

    result = d_assert_standalone(true,
        "signal_fence_no_crash", "Signal fences should not crash", _counter) && result;

    return result;
}

bool d_tests_sa_atomic_is_lock_free(struct d_test_counter* _counter)
{
    bool result = true;

    // just verify functions return boolean values
    bool lf1 = d_atomic_is_lock_free_1();
    bool lf2 = d_atomic_is_lock_free_2();
    bool lf4 = d_atomic_is_lock_free_4();
    bool lf8 = d_atomic_is_lock_free_8();

    result = d_assert_standalone(
        lf1 == true || lf1 == false,
        "is_lock_free_1_valid",
        "Is-lock-free-1 should return boolean",
        _counter) && result;

    result = d_assert_standalone(
        lf2 == true || lf2 == false,
        "is_lock_free_2_valid",
        "Is-lock-free-2 should return boolean",
        _counter) && result;

    result = d_assert_standalone(
        lf4 == true || lf4 == false,
        "is_lock_free_4_valid",
        "Is-lock-free-4 should return boolean",
        _counter) && result;

    result = d_assert_standalone(
        lf8 == true || lf8 == false,
        "is_lock_free_8_valid",
        "Is-lock-free-8 should return boolean",
        _counter) && result;

    return result;
}

bool d_tests_sa_atomic_fence_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Memory Fences and Utilities\n");
    printf("  --------------------------------------\n");

    result = d_tests_sa_atomic_thread_fence(_counter) && result;
    result = d_tests_sa_atomic_signal_fence(_counter) && result;
    result = d_tests_sa_atomic_is_lock_free(_counter) && result;

    return result;
}
