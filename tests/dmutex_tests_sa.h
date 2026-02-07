/******************************************************************************
* djinterp [test]                                               mutex_tests_sa.h
*
*   Unit test declarations for `dmutex.h` module.
*   Provides comprehensive testing of all d_mutex functions including
* basic mutex operations, recursive mutexes, thread operations, condition
* variables, read-write locks, thread-specific storage, synchronization
* primitives, and utility functions.
*
*
* path:      \tests\threading\mutex\mutex_tests_sa.h
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.02.06
******************************************************************************/

#ifndef DJINTERP_TESTS_MUTEX_SA_
#define DJINTERP_TESTS_MUTEX_SA_ 1

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "..\..\..\inc\test\test_standalone.h"
#include "..\..\..\inc\dmutex.h"
#include "..\..\..\inc\string_fn.h"
#include "..\..\..\inc\dtime.h"


/******************************************************************************
 * I. BASIC MUTEX OPERATION TESTS
 *****************************************************************************/
bool d_tests_sa_mutex_init(struct d_test_counter* _counter);
bool d_tests_sa_mutex_destroy(struct d_test_counter* _counter);
bool d_tests_sa_mutex_lock(struct d_test_counter* _counter);
bool d_tests_sa_mutex_unlock(struct d_test_counter* _counter);
bool d_tests_sa_mutex_trylock(struct d_test_counter* _counter);
bool d_tests_sa_mutex_timedlock(struct d_test_counter* _counter);

// I.   aggregation function
bool d_tests_sa_mutex_basic_all(struct d_test_counter* _counter);


/******************************************************************************
 * II. RECURSIVE MUTEX OPERATION TESTS
 *****************************************************************************/
bool d_tests_sa_recursive_mutex_init(struct d_test_counter* _counter);
bool d_tests_sa_recursive_mutex_destroy(struct d_test_counter* _counter);
bool d_tests_sa_recursive_mutex_lock(struct d_test_counter* _counter);
bool d_tests_sa_recursive_mutex_unlock(struct d_test_counter* _counter);
bool d_tests_sa_recursive_mutex_trylock(struct d_test_counter* _counter);
bool d_tests_sa_recursive_mutex_recursive_locking(struct d_test_counter* _counter);

// II.  aggregation function
bool d_tests_sa_mutex_recursive_all(struct d_test_counter* _counter);


/******************************************************************************
 * III. THREAD OPERATION TESTS
 *****************************************************************************/
bool d_tests_sa_thread_create(struct d_test_counter* _counter);
bool d_tests_sa_thread_join(struct d_test_counter* _counter);
bool d_tests_sa_thread_detach(struct d_test_counter* _counter);
bool d_tests_sa_thread_exit(struct d_test_counter* _counter);
bool d_tests_sa_thread_yield(struct d_test_counter* _counter);
bool d_tests_sa_thread_sleep(struct d_test_counter* _counter);
bool d_tests_sa_thread_current(struct d_test_counter* _counter);
bool d_tests_sa_thread_equal(struct d_test_counter* _counter);

// III. aggregation function
bool d_tests_sa_mutex_thread_all(struct d_test_counter* _counter);


/******************************************************************************
 * IV. CONDITION VARIABLE TESTS
 *****************************************************************************/
bool d_tests_sa_cond_init(struct d_test_counter* _counter);
bool d_tests_sa_cond_destroy(struct d_test_counter* _counter);
bool d_tests_sa_cond_signal(struct d_test_counter* _counter);
bool d_tests_sa_cond_broadcast(struct d_test_counter* _counter);
bool d_tests_sa_cond_wait(struct d_test_counter* _counter);
bool d_tests_sa_cond_timedwait(struct d_test_counter* _counter);

// IV.  aggregation function
bool d_tests_sa_mutex_cond_all(struct d_test_counter* _counter);


/******************************************************************************
 * V. READ-WRITE LOCK TESTS
 *****************************************************************************/
bool d_tests_sa_rwlock_init(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_destroy(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_rdlock(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_wrlock(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_tryrdlock(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_trywrlock(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_unlock(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_timedrdlock(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_timedwrlock(struct d_test_counter* _counter);
bool d_tests_sa_rwlock_multiple_readers(struct d_test_counter* _counter);

// V.   aggregation function
bool d_tests_sa_mutex_rwlock_all(struct d_test_counter* _counter);


/******************************************************************************
 * VI. THREAD-SPECIFIC STORAGE TESTS
 *****************************************************************************/
bool d_tests_sa_tss_create(struct d_test_counter* _counter);
bool d_tests_sa_tss_delete(struct d_test_counter* _counter);
bool d_tests_sa_tss_get(struct d_test_counter* _counter);
bool d_tests_sa_tss_set(struct d_test_counter* _counter);
bool d_tests_sa_tss_multithreaded(struct d_test_counter* _counter);

// VI.  aggregation function
bool d_tests_sa_mutex_tss_all(struct d_test_counter* _counter);


/******************************************************************************
 * VII. SYNCHRONIZATION PRIMITIVE TESTS
 *****************************************************************************/
bool d_tests_sa_call_once(struct d_test_counter* _counter);
bool d_tests_sa_call_once_multithreaded(struct d_test_counter* _counter);

// VII. aggregation function
bool d_tests_sa_mutex_sync_all(struct d_test_counter* _counter);


/******************************************************************************
 * VIII. UTILITY FUNCTION TESTS
 *****************************************************************************/
bool d_tests_sa_thread_hardware_concurrency(struct d_test_counter* _counter);

// VIII. aggregation function
bool d_tests_sa_mutex_utility_all(struct d_test_counter* _counter);


/******************************************************************************
 * MODULE-LEVEL AGGREGATION
 *****************************************************************************/
bool d_tests_sa_mutex_run_all(struct d_test_counter* _counter);


#endif  // DJINTERP_TESTS_MUTEX_SA_
