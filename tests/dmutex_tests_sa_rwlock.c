#include ".\dmutex_tests_sa.h"


bool d_tests_sa_rwlock_init(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    result = d_assert_standalone(d_rwlock_init(NULL) != D_MUTEX_SUCCESS,
        "rwlock_init_null", "NULL rwlock should fail", _counter) && result;
    result = d_assert_standalone(d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS,
        "rwlock_init_success", "Rwlock init should succeed", _counter) && result;

    d_rwlock_destroy(&rwlock);
    return result;
}

bool d_tests_sa_rwlock_destroy(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    result = d_assert_standalone(d_rwlock_destroy(NULL) != D_MUTEX_SUCCESS,
        "rwlock_destroy_null", "NULL rwlock destroy should fail", _counter) && result;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_rwlock_destroy(&rwlock) == D_MUTEX_SUCCESS,
            "rwlock_destroy_success", "Rwlock destroy should succeed", _counter) && result;
    }
    return result;
}

bool d_tests_sa_rwlock_rdlock(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    result = d_assert_standalone(d_rwlock_rdlock(NULL) != D_MUTEX_SUCCESS,
        "rwlock_rdlock_null", "NULL rwlock rdlock should fail", _counter) && result;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_rwlock_rdlock(&rwlock) == D_MUTEX_SUCCESS,
            "rwlock_rdlock_success", "Rdlock should succeed", _counter) && result;
        d_rwlock_unlock(&rwlock);
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_rwlock_wrlock(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    result = d_assert_standalone(d_rwlock_wrlock(NULL) != D_MUTEX_SUCCESS,
        "rwlock_wrlock_null", "NULL rwlock wrlock should fail", _counter) && result;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_rwlock_wrlock(&rwlock) == D_MUTEX_SUCCESS,
            "rwlock_wrlock_success", "Wrlock should succeed", _counter) && result;
        d_rwlock_unlock(&rwlock);
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_rwlock_tryrdlock(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_rwlock_tryrdlock(&rwlock) == D_MUTEX_SUCCESS,
            "rwlock_tryrdlock_success", "Try rdlock should succeed", _counter) && result;
        d_rwlock_unlock(&rwlock);
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_rwlock_trywrlock(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(d_rwlock_trywrlock(&rwlock) == D_MUTEX_SUCCESS,
            "rwlock_trywrlock_success", "Try wrlock should succeed", _counter) && result;
        d_rwlock_unlock(&rwlock);
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_rwlock_unlock(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    result = d_assert_standalone(d_rwlock_unlock(NULL) != D_MUTEX_SUCCESS,
        "rwlock_unlock_null", "NULL rwlock unlock should fail", _counter) && result;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        d_rwlock_rdlock(&rwlock);
        result = d_assert_standalone(d_rwlock_unlock(&rwlock) == D_MUTEX_SUCCESS,
            "rwlock_unlock_success", "Unlock should succeed", _counter) && result;
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_rwlock_timedrdlock(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;
    struct timespec timeout;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        d_clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 1;
        result = d_assert_standalone(d_rwlock_timedrdlock(&rwlock, &timeout) == D_MUTEX_SUCCESS,
            "rwlock_timedrdlock_success", "Timed rdlock should succeed", _counter) && result;
        d_rwlock_unlock(&rwlock);
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_rwlock_timedwrlock(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;
    struct timespec timeout;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        d_clock_gettime(CLOCK_REALTIME, &timeout);
        timeout.tv_sec += 1;
        result = d_assert_standalone(d_rwlock_timedwrlock(&rwlock, &timeout) == D_MUTEX_SUCCESS,
            "rwlock_timedwrlock_success", "Timed wrlock should succeed", _counter) && result;
        d_rwlock_unlock(&rwlock);
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_rwlock_multiple_readers(struct d_test_counter* _counter)
{
    bool result = true;
    struct d_rwlock_t rwlock;

    if (d_rwlock_init(&rwlock) == D_MUTEX_SUCCESS)
    {
        d_rwlock_rdlock(&rwlock);
        result = d_assert_standalone(d_rwlock_tryrdlock(&rwlock) == D_MUTEX_SUCCESS,
            "rwlock_multiple_readers", "Multiple readers should be allowed", _counter) && result;
        d_rwlock_unlock(&rwlock);
        d_rwlock_unlock(&rwlock);
        d_rwlock_destroy(&rwlock);
    }
    return result;
}

bool d_tests_sa_mutex_rwlock_all(struct d_test_counter* _counter)
{
    bool result = true;
    printf("\n  [SECTION] Read-Write Locks\n");
    printf("  ---------------------------\n");

    result = d_tests_sa_rwlock_init(_counter) && result;
    result = d_tests_sa_rwlock_destroy(_counter) && result;
    result = d_tests_sa_rwlock_rdlock(_counter) && result;
    result = d_tests_sa_rwlock_wrlock(_counter) && result;
    result = d_tests_sa_rwlock_tryrdlock(_counter) && result;
    result = d_tests_sa_rwlock_trywrlock(_counter) && result;
    result = d_tests_sa_rwlock_unlock(_counter) && result;
    result = d_tests_sa_rwlock_timedrdlock(_counter) && result;
    result = d_tests_sa_rwlock_timedwrlock(_counter) && result;
    result = d_tests_sa_rwlock_multiple_readers(_counter) && result;

    return result;
}
