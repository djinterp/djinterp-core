#include ".\dmutex_tests_sa.h"


// shared data for condition variable tests
struct cond_test_data
{
    d_mutex_t mutex;
    d_cond_t  cond;
    int       value;
    bool      ready;
};


static d_thread_result_t
cond_wait_thread
(
    void* _arg
)
{
    struct cond_test_data* data = (struct cond_test_data*)_arg;

    d_mutex_lock(&data->mutex);

    while (!data->ready)
    {
        d_cond_wait(&data->cond, &data->mutex);
    }

    data->value = 99;

    d_mutex_unlock(&data->mutex);

#if D_MUTEX_HAS_C11_THREADS
    return 0;
#else
    return NULL;
#endif
}


bool
d_tests_sa_cond_init
(
    struct d_test_counter* _counter
)
{
    bool     result;
    d_cond_t cond;

    result = true;

    result = d_assert_standalone(
        d_cond_init(NULL) != D_MUTEX_SUCCESS,
        "cond_init_null",
        "NULL cond should fail",
        _counter) && result;

    result = d_assert_standalone(
        d_cond_init(&cond) == D_MUTEX_SUCCESS,
        "cond_init_success",
        "Cond init should succeed",
        _counter) && result;

    d_cond_destroy(&cond);

    return result;
}


bool
d_tests_sa_cond_destroy
(
    struct d_test_counter* _counter
)
{
    bool     result;
    d_cond_t cond;

    result = true;

    result = d_assert_standalone(
        d_cond_destroy(NULL) != D_MUTEX_SUCCESS,
        "cond_destroy_null",
        "NULL cond destroy should fail",
        _counter) && result;

    if (d_cond_init(&cond) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(
            d_cond_destroy(&cond) == D_MUTEX_SUCCESS,
            "cond_destroy_success",
            "Cond destroy should succeed",
            _counter) && result;
    }

    return result;
}


bool
d_tests_sa_cond_signal
(
    struct d_test_counter* _counter
)
{
    bool                   result;
    struct cond_test_data  data;
    d_thread_t             thread;
    struct timespec        sleep_time;

    result = true;

    result = d_assert_standalone(
        d_cond_signal(NULL) != D_MUTEX_SUCCESS,
        "cond_signal_null",
        "NULL cond signal should fail",
        _counter) && result;

    // test signal wakes waiting thread
    d_mutex_init(&data.mutex);
    d_cond_init(&data.cond);
    data.value = 0;
    data.ready = false;

    d_thread_create(&thread, cond_wait_thread, &data);

    sleep_time.tv_sec  = 0;
    sleep_time.tv_nsec = 100000000;  // 100ms
    d_thread_sleep(&sleep_time, NULL);

    d_mutex_lock(&data.mutex);
    data.ready = true;
    d_cond_signal(&data.cond);
    d_mutex_unlock(&data.mutex);

    d_thread_join(thread, NULL);

    result = d_assert_standalone(
        data.value == 99,
        "cond_signal_wakes",
        "Signal should wake waiting thread",
        _counter) && result;

    d_cond_destroy(&data.cond);
    d_mutex_destroy(&data.mutex);

    return result;
}


bool
d_tests_sa_cond_broadcast
(
    struct d_test_counter* _counter
)
{
    bool    result;
    d_cond_t cond;

    result = true;

    result = d_assert_standalone(
        d_cond_broadcast(NULL) != D_MUTEX_SUCCESS,
        "cond_broadcast_null",
        "NULL cond broadcast should fail",
        _counter) && result;

    if (d_cond_init(&cond) == D_MUTEX_SUCCESS)
    {
        result = d_assert_standalone(
            d_cond_broadcast(&cond) == D_MUTEX_SUCCESS,
            "cond_broadcast_success",
            "Broadcast should succeed",
            _counter) && result;

        d_cond_destroy(&cond);
    }

    return result;
}


bool
d_tests_sa_cond_wait
(
    struct d_test_counter* _counter
)
{
    bool     result;
    d_cond_t cond;
    d_mutex_t mutex;

    result = true;

    d_cond_init(&cond);
    d_mutex_init(&mutex);

    result = d_assert_standalone(
        d_cond_wait(NULL, &mutex) != D_MUTEX_SUCCESS,
        "cond_wait_null_cond",
        "NULL cond wait should fail",
        _counter) && result;

    result = d_assert_standalone(
        d_cond_wait(&cond, NULL) != D_MUTEX_SUCCESS,
        "cond_wait_null_mutex",
        "Cond wait with NULL mutex should fail",
        _counter) && result;

    d_cond_destroy(&cond);
    d_mutex_destroy(&mutex);

    return result;
}


bool
d_tests_sa_cond_timedwait
(
    struct d_test_counter* _counter
)
{
    bool            result;
    d_cond_t        cond;
    d_mutex_t       mutex;
    struct timespec timeout;

    result = true;

    d_cond_init(&cond);
    d_mutex_init(&mutex);

    d_clock_gettime(CLOCK_REALTIME, &timeout);
    timeout.tv_nsec += 100000000;  // 100ms
    if (timeout.tv_nsec >= 1000000000)
    {
        timeout.tv_sec++;
        timeout.tv_nsec -= 1000000000;
    }

    d_mutex_lock(&mutex);
    result = d_assert_standalone(
        d_cond_timedwait(&cond, &mutex, &timeout) == D_MUTEX_TIMEDOUT,
        "cond_timedwait_timeout",
        "Timed wait should timeout",
        _counter) && result;
    d_mutex_unlock(&mutex);

    d_cond_destroy(&cond);
    d_mutex_destroy(&mutex);

    return result;
}


bool
d_tests_sa_mutex_cond_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Condition Variables\n");
    printf("  ------------------------------\n");

    result = d_tests_sa_cond_init(_counter) && result;
    result = d_tests_sa_cond_destroy(_counter) && result;
    result = d_tests_sa_cond_signal(_counter) && result;
    result = d_tests_sa_cond_broadcast(_counter) && result;
    result = d_tests_sa_cond_wait(_counter) && result;
    result = d_tests_sa_cond_timedwait(_counter) && result;

    return result;
}
