#include ".\mutex_tests_sa.h"


bool d_tests_sa_thread_hardware_concurrency(struct d_test_counter* _counter)
{
    bool result = true;
    int concurrency;

    concurrency = d_thread_hardware_concurrency();

    result = d_assert_standalone(concurrency >= 1,
        "hardware_concurrency_positive", "Hardware concurrency should be at least 1", _counter) && result;

    result = d_assert_standalone(concurrency <= 1024,
        "hardware_concurrency_reasonable", "Hardware concurrency should be reasonable (<= 1024)", _counter) && result;

    return result;
}

bool d_tests_sa_mutex_utility_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Utility Functions\n");
    printf("  ----------------------------\n");

    result = d_tests_sa_thread_hardware_concurrency(_counter) && result;

    return result;
}
