/******************************************************************************
* djinterp [core]                                                    datomic.h
*
* Cross-platform atomic operations interface.
*   This header provides a unified interface for atomic operations across
* platforms, wrapping C11 stdatomic.h on supported platforms and providing
* Windows-specific implementations using Interlocked* functions on older
* compilers. The interface is designed to be as seamless as using stdatomic.h
* directly, with no functionality missing.
*   Native C11 atomics are used whenever available, with fallback
* implementations for Windows and pre-C11 compilers.
*
* path:      \inc\datomic.h
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.02.06
******************************************************************************/

/*
TABLE OF CONTENTS
=================
I.    PLATFORM DETECTION AND INCLUDES
      --------------------------------
      1.  Standard headers
      2.  Platform-specific headers
      3.  Feature detection macros

II.   TYPE DEFINITIONS
      -----------------
      1.  Atomic type definitions
      2.  Memory order enumeration
      3.  Atomic flag type

III.  ATOMIC FLAG OPERATIONS
      ------------------------
      1.  d_atomic_flag_test_and_set
      2.  d_atomic_flag_clear
      3.  d_atomic_flag_init

IV.   ATOMIC INITIALIZATION
      ----------------------
      1.  d_atomic_init_int
      2.  d_atomic_init_uint
      3.  d_atomic_init_long
      4.  d_atomic_init_ulong
      5.  d_atomic_init_llong
      6.  d_atomic_init_ullong
      7.  d_atomic_init_ptr
      8.  d_atomic_init_size

V.    ATOMIC LOAD OPERATIONS
      -----------------------
      1.  d_atomic_load_int
      2.  d_atomic_load_uint
      3.  d_atomic_load_long
      4.  d_atomic_load_ulong
      5.  d_atomic_load_llong
      6.  d_atomic_load_ullong
      7.  d_atomic_load_ptr
      8.  d_atomic_load_size
      9.  d_atomic_load_explicit (with memory order)

VI.   ATOMIC STORE OPERATIONS
      ------------------------
      1.  d_atomic_store_int
      2.  d_atomic_store_uint
      3.  d_atomic_store_long
      4.  d_atomic_store_ulong
      5.  d_atomic_store_llong
      6.  d_atomic_store_ullong
      7.  d_atomic_store_ptr
      8.  d_atomic_store_size
      9.  d_atomic_store_explicit (with memory order)

VII.  ATOMIC EXCHANGE OPERATIONS
      ---------------------------
      1.  d_atomic_exchange_int
      2.  d_atomic_exchange_uint
      3.  d_atomic_exchange_long
      4.  d_atomic_exchange_ulong
      5.  d_atomic_exchange_llong
      6.  d_atomic_exchange_ullong
      7.  d_atomic_exchange_ptr
      8.  d_atomic_exchange_size

VIII. ATOMIC COMPARE-AND-EXCHANGE
      ----------------------------
      1.  d_atomic_compare_exchange_strong_int
      2.  d_atomic_compare_exchange_weak_int
      3.  d_atomic_compare_exchange_strong_uint
      4.  d_atomic_compare_exchange_weak_uint
      5.  d_atomic_compare_exchange_strong_long
      6.  d_atomic_compare_exchange_weak_long
      7.  d_atomic_compare_exchange_strong_ulong
      8.  d_atomic_compare_exchange_weak_ulong
      9.  d_atomic_compare_exchange_strong_llong
      10. d_atomic_compare_exchange_weak_llong
      11. d_atomic_compare_exchange_strong_ullong
      12. d_atomic_compare_exchange_weak_ullong
      13. d_atomic_compare_exchange_strong_ptr
      14. d_atomic_compare_exchange_weak_ptr
      15. d_atomic_compare_exchange_strong_size
      16. d_atomic_compare_exchange_weak_size

IX.   ATOMIC FETCH-AND-MODIFY OPERATIONS
      -------------------------------------
      1.  Fetch-and-add operations
      2.  Fetch-and-subtract operations
      3.  Fetch-and-bitwise-or operations
      4.  Fetch-and-bitwise-xor operations
      5.  Fetch-and-bitwise-and operations

X.    MEMORY ORDERING AND FENCES
      ---------------------------
      1.  d_atomic_thread_fence
      2.  d_atomic_signal_fence
      3.  d_atomic_is_lock_free

XI.   CONVENIENCE MACROS
      -------------------
      1.  Atomic type initialization macros
      2.  Generic atomic operation macros
*/

#ifndef DJINTERP_ATOMIC_
#define DJINTERP_ATOMIC_ 1


///////////////////////////////////////////////////////////////////////////////
///             I.    PLATFORM DETECTION AND INCLUDES                       ///
///////////////////////////////////////////////////////////////////////////////

#include <stddef.h>      // for size_t
#include <stdint.h>      // for fixed-width types
#include <stdbool.h>     // for bool
#include ".\djinterp.h"


// D_ATOMIC_HAS_STDATOMIC
//   feature: detect if we can use C11 stdatomic.h
#ifndef D_ATOMIC_HAS_STDATOMIC
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
        // Check if stdatomic.h is actually available
        #if !defined(__STDC_NO_ATOMICS__)
            #define D_ATOMIC_HAS_STDATOMIC 1
        #else
            #define D_ATOMIC_HAS_STDATOMIC 0
        #endif
    #elif defined(__cplusplus) && (__cplusplus >= 201103L)
        // C++11 and later have atomic support
        #define D_ATOMIC_HAS_STDATOMIC 1
    #else
        #define D_ATOMIC_HAS_STDATOMIC 0
    #endif
#endif


// Include appropriate headers based on platform and availability
#if D_ATOMIC_HAS_STDATOMIC
    #ifdef __cplusplus
        #include <atomic>
        using std::atomic_int;
        using std::atomic_uint;
        using std::atomic_long;
        using std::atomic_ulong;
        using std::atomic_llong;
        using std::atomic_ullong;
        using std::atomic_size_t;
        using std::atomic_flag;
        using std::memory_order;
        using std::memory_order_relaxed;
        using std::memory_order_consume;
        using std::memory_order_acquire;
        using std::memory_order_release;
        using std::memory_order_acq_rel;
        using std::memory_order_seq_cst;
    #else
        #include <stdatomic.h>
    #endif
#elif defined(_WIN32) || defined(_WIN64)
    // Windows-specific includes for Interlocked* functions
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #include <intrin.h>
#endif


///////////////////////////////////////////////////////////////////////////////
///             II.   TYPE DEFINITIONS                                      ///
///////////////////////////////////////////////////////////////////////////////

#if D_ATOMIC_HAS_STDATOMIC
    // Use standard atomic types
    typedef atomic_int     d_atomic_int;
    typedef atomic_uint    d_atomic_uint;
    typedef atomic_long    d_atomic_long;
    typedef atomic_ulong   d_atomic_ulong;
    typedef atomic_llong   d_atomic_llong;
    typedef atomic_ullong  d_atomic_ullong;
    typedef atomic_size_t  d_atomic_size_t;
    typedef atomic_flag    d_atomic_flag;
    
    // Atomic pointer type (generic)
    #define D_ATOMIC_PTR_TYPE(T) _Atomic(T*)
    typedef _Atomic(void*) d_atomic_ptr;

    // Memory order type
    typedef memory_order d_memory_order;
    
    // Memory order constants
    #define D_MEMORY_ORDER_RELAXED memory_order_relaxed
    #define D_MEMORY_ORDER_CONSUME memory_order_consume
    #define D_MEMORY_ORDER_ACQUIRE memory_order_acquire
    #define D_MEMORY_ORDER_RELEASE memory_order_release
    #define D_MEMORY_ORDER_ACQ_REL memory_order_acq_rel
    #define D_MEMORY_ORDER_SEQ_CST memory_order_seq_cst

#else
    // Fallback types for non-C11 platforms
    #if defined(_WIN32) || defined(_WIN64)
        // Windows volatile types
        typedef volatile long           d_atomic_int;
        typedef volatile unsigned long  d_atomic_uint;
        typedef volatile long           d_atomic_long;
        typedef volatile unsigned long  d_atomic_ulong;
        typedef volatile long long      d_atomic_llong;
        typedef volatile unsigned long long d_atomic_ullong;
        typedef volatile size_t         d_atomic_size_t;
        typedef volatile void*          d_atomic_ptr;
        
        // Atomic flag implementation
        typedef struct {
            volatile long value;
        } d_atomic_flag;
        
    #else
        // Generic fallback (may not be truly atomic on all platforms)
        typedef volatile int                d_atomic_int;
        typedef volatile unsigned int       d_atomic_uint;
        typedef volatile long               d_atomic_long;
        typedef volatile unsigned long      d_atomic_ulong;
        typedef volatile long long          d_atomic_llong;
        typedef volatile unsigned long long d_atomic_ullong;
        typedef volatile size_t             d_atomic_size_t;
        typedef volatile void*              d_atomic_ptr;
        
        // Atomic flag implementation
        typedef struct {
            volatile int value;
        } d_atomic_flag;
    #endif

    // Memory order enumeration (for compatibility)
    typedef enum {
        D_MEMORY_ORDER_RELAXED = 0,
        D_MEMORY_ORDER_CONSUME = 1,
        D_MEMORY_ORDER_ACQUIRE = 2,
        D_MEMORY_ORDER_RELEASE = 3,
        D_MEMORY_ORDER_ACQ_REL = 4,
        D_MEMORY_ORDER_SEQ_CST = 5
    } d_memory_order;

#endif  // D_ATOMIC_HAS_STDATOMIC


// D_ATOMIC_VAR_INIT
//   macro: initialize atomic variable at declaration (deprecated in C17 but
// useful for compatibility)
#if D_ATOMIC_HAS_STDATOMIC && !defined(__cplusplus)
    #ifdef ATOMIC_VAR_INIT
        #define D_ATOMIC_VAR_INIT(value) ATOMIC_VAR_INIT(value)
    #else
        // C17 deprecated ATOMIC_VAR_INIT, just use direct initialization
        #define D_ATOMIC_VAR_INIT(value) (value)
    #endif
#else
    #define D_ATOMIC_VAR_INIT(value) (value)
#endif

// D_ATOMIC_FLAG_INIT
//   macro: initialize atomic flag to clear state
#if D_ATOMIC_HAS_STDATOMIC
    #define D_ATOMIC_FLAG_INIT ATOMIC_FLAG_INIT
#else
    #define D_ATOMIC_FLAG_INIT {0}
#endif


///////////////////////////////////////////////////////////////////////////////
///             III.  ATOMIC FLAG OPERATIONS                                ///
///////////////////////////////////////////////////////////////////////////////

// d_atomic_flag_test_and_set
//   function: atomically sets the flag and returns the previous value
bool               d_atomic_flag_test_and_set(d_atomic_flag* _flag);
bool               d_atomic_flag_test_and_set_explicit(d_atomic_flag* _flag, d_memory_order _order);

// d_atomic_flag_clear
//   function: atomically clears the flag
void               d_atomic_flag_clear(d_atomic_flag* _flag);
void               d_atomic_flag_clear_explicit(d_atomic_flag* _flag, d_memory_order _order);


///////////////////////////////////////////////////////////////////////////////
///             IV.   ATOMIC INITIALIZATION                                 ///
///////////////////////////////////////////////////////////////////////////////

void               d_atomic_init_int(d_atomic_int* _obj, int _value);
void               d_atomic_init_uint(d_atomic_uint* _obj, unsigned int _value);
void               d_atomic_init_long(d_atomic_long* _obj, long _value);
void               d_atomic_init_ulong(d_atomic_ulong* _obj, unsigned long _value);
void               d_atomic_init_llong(d_atomic_llong* _obj, long long _value);
void               d_atomic_init_ullong(d_atomic_ullong* _obj, unsigned long long _value);
void               d_atomic_init_ptr(d_atomic_ptr* _obj, void* _value);
void               d_atomic_init_size(d_atomic_size_t* _obj, size_t _value);


///////////////////////////////////////////////////////////////////////////////
///             V.    ATOMIC LOAD OPERATIONS                                ///
///////////////////////////////////////////////////////////////////////////////

// Basic load operations
int                d_atomic_load_int(const d_atomic_int* _obj);
unsigned int       d_atomic_load_uint(const d_atomic_uint* _obj);
long               d_atomic_load_long(const d_atomic_long* _obj);
unsigned long      d_atomic_load_ulong(const d_atomic_ulong* _obj);
long long          d_atomic_load_llong(const d_atomic_llong* _obj);
unsigned long long d_atomic_load_ullong(const d_atomic_ullong* _obj);
void*              d_atomic_load_ptr(const d_atomic_ptr* _obj);
size_t             d_atomic_load_size(const d_atomic_size_t* _obj);

// Explicit memory order variants
int                d_atomic_load_int_explicit(const d_atomic_int* _obj, d_memory_order _order);
unsigned int       d_atomic_load_uint_explicit(const d_atomic_uint* _obj, d_memory_order _order);
long               d_atomic_load_long_explicit(const d_atomic_long* _obj, d_memory_order _order);
unsigned long      d_atomic_load_ulong_explicit(const d_atomic_ulong* _obj, d_memory_order _order);
long long          d_atomic_load_llong_explicit(const d_atomic_llong* _obj, d_memory_order _order);
unsigned long long d_atomic_load_ullong_explicit(const d_atomic_ullong* _obj, d_memory_order _order);
void*              d_atomic_load_ptr_explicit(const d_atomic_ptr* _obj, d_memory_order _order);
size_t             d_atomic_load_size_explicit(const d_atomic_size_t* _obj, d_memory_order _order);


///////////////////////////////////////////////////////////////////////////////
///             VI.   ATOMIC STORE OPERATIONS                               ///
///////////////////////////////////////////////////////////////////////////////

// Basic store operations
void               d_atomic_store_int(d_atomic_int* _obj, int _value);
void               d_atomic_store_uint(d_atomic_uint* _obj, unsigned int _value);
void               d_atomic_store_long(d_atomic_long* _obj, long _value);
void               d_atomic_store_ulong(d_atomic_ulong* _obj, unsigned long _value);
void               d_atomic_store_llong(d_atomic_llong* _obj, long long _value);
void               d_atomic_store_ullong(d_atomic_ullong* _obj, unsigned long long _value);
void               d_atomic_store_ptr(d_atomic_ptr* _obj, void* _value);
void               d_atomic_store_size(d_atomic_size_t* _obj, size_t _value);

// Explicit memory order variants
void               d_atomic_store_int_explicit(d_atomic_int* _obj, int _value, d_memory_order _order);
void               d_atomic_store_uint_explicit(d_atomic_uint* _obj, unsigned int _value, d_memory_order _order);
void               d_atomic_store_long_explicit(d_atomic_long* _obj, long _value, d_memory_order _order);
void               d_atomic_store_ulong_explicit(d_atomic_ulong* _obj, unsigned long _value, d_memory_order _order);
void               d_atomic_store_llong_explicit(d_atomic_llong* _obj, long long _value, d_memory_order _order);
void               d_atomic_store_ullong_explicit(d_atomic_ullong* _obj, unsigned long long _value, d_memory_order _order);
void               d_atomic_store_ptr_explicit(d_atomic_ptr* _obj, void* _value, d_memory_order _order);
void               d_atomic_store_size_explicit(d_atomic_size_t* _obj, size_t _value, d_memory_order _order);


///////////////////////////////////////////////////////////////////////////////
///             VII.  ATOMIC EXCHANGE OPERATIONS                            ///
///////////////////////////////////////////////////////////////////////////////

int                d_atomic_exchange_int(d_atomic_int* _obj, int _value);
unsigned int       d_atomic_exchange_uint(d_atomic_uint* _obj, unsigned int _value);
long               d_atomic_exchange_long(d_atomic_long* _obj, long _value);
unsigned long      d_atomic_exchange_ulong(d_atomic_ulong* _obj, unsigned long _value);
long long          d_atomic_exchange_llong(d_atomic_llong* _obj, long long _value);
unsigned long long d_atomic_exchange_ullong(d_atomic_ullong* _obj, unsigned long long _value);
void*              d_atomic_exchange_ptr(d_atomic_ptr* _obj, void* _value);
size_t             d_atomic_exchange_size(d_atomic_size_t* _obj, size_t _value);

// Explicit memory order variants
int                d_atomic_exchange_int_explicit(d_atomic_int* _obj, int _value, d_memory_order _order);
unsigned int       d_atomic_exchange_uint_explicit(d_atomic_uint* _obj, unsigned int _value, d_memory_order _order);
long               d_atomic_exchange_long_explicit(d_atomic_long* _obj, long _value, d_memory_order _order);
unsigned long      d_atomic_exchange_ulong_explicit(d_atomic_ulong* _obj, unsigned long _value, d_memory_order _order);
long long          d_atomic_exchange_llong_explicit(d_atomic_llong* _obj, long long _value, d_memory_order _order);
unsigned long long d_atomic_exchange_ullong_explicit(d_atomic_ullong* _obj, unsigned long long _value, d_memory_order _order);
void*              d_atomic_exchange_ptr_explicit(d_atomic_ptr* _obj, void* _value, d_memory_order _order);
size_t             d_atomic_exchange_size_explicit(d_atomic_size_t* _obj, size_t _value, d_memory_order _order);


///////////////////////////////////////////////////////////////////////////////
///             VIII. ATOMIC COMPARE-AND-EXCHANGE                           ///
///////////////////////////////////////////////////////////////////////////////

// Strong compare-and-exchange (no spurious failures)
bool               d_atomic_compare_exchange_strong_int(d_atomic_int* _obj, int* _expected, int _desired);
bool               d_atomic_compare_exchange_strong_uint(d_atomic_uint* _obj, unsigned int* _expected, unsigned int _desired);
bool               d_atomic_compare_exchange_strong_long(d_atomic_long* _obj, long* _expected, long _desired);
bool               d_atomic_compare_exchange_strong_ulong(d_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired);
bool               d_atomic_compare_exchange_strong_llong(d_atomic_llong* _obj, long long* _expected, long long _desired);
bool               d_atomic_compare_exchange_strong_ullong(d_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired);
bool               d_atomic_compare_exchange_strong_ptr(d_atomic_ptr* _obj, void** _expected, void* _desired);
bool               d_atomic_compare_exchange_strong_size(d_atomic_size_t* _obj, size_t* _expected, size_t _desired);

// Weak compare-and-exchange (may spuriously fail on some platforms)
bool               d_atomic_compare_exchange_weak_int(d_atomic_int* _obj, int* _expected, int _desired);
bool               d_atomic_compare_exchange_weak_uint(d_atomic_uint* _obj, unsigned int* _expected, unsigned int _desired);
bool               d_atomic_compare_exchange_weak_long(d_atomic_long* _obj, long* _expected, long _desired);
bool               d_atomic_compare_exchange_weak_ulong(d_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired);
bool               d_atomic_compare_exchange_weak_llong(d_atomic_llong* _obj, long long* _expected, long long _desired);
bool               d_atomic_compare_exchange_weak_ullong(d_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired);
bool               d_atomic_compare_exchange_weak_ptr(d_atomic_ptr* _obj, void** _expected, void* _desired);
bool               d_atomic_compare_exchange_weak_size(d_atomic_size_t* _obj, size_t* _expected, size_t _desired);

// Explicit memory order variants (strong)
bool               d_atomic_compare_exchange_strong_int_explicit(d_atomic_int* _obj, int* _expected, int _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_strong_uint_explicit(d_atomic_uint* _obj, unsigned int* _expected, unsigned int _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_strong_long_explicit(d_atomic_long* _obj, long* _expected, long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_strong_ulong_explicit(d_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_strong_llong_explicit(d_atomic_llong* _obj, long long* _expected, long long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_strong_ullong_explicit(d_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_strong_ptr_explicit(d_atomic_ptr* _obj, void** _expected, void* _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_strong_size_explicit(d_atomic_size_t* _obj, size_t* _expected, size_t _desired, d_memory_order _success, d_memory_order _failure);

// Explicit memory order variants (weak)
bool               d_atomic_compare_exchange_weak_int_explicit(d_atomic_int* _obj, int* _expected, int _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_weak_uint_explicit(d_atomic_uint* _obj, unsigned int* _expected, unsigned int _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_weak_long_explicit(d_atomic_long* _obj, long* _expected, long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_weak_ulong_explicit(d_atomic_ulong* _obj, unsigned long* _expected, unsigned long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_weak_llong_explicit(d_atomic_llong* _obj, long long* _expected, long long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_weak_ullong_explicit(d_atomic_ullong* _obj, unsigned long long* _expected, unsigned long long _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_weak_ptr_explicit(d_atomic_ptr* _obj, void** _expected, void* _desired, d_memory_order _success, d_memory_order _failure);
bool               d_atomic_compare_exchange_weak_size_explicit(d_atomic_size_t* _obj, size_t* _expected, size_t _desired, d_memory_order _success, d_memory_order _failure);


///////////////////////////////////////////////////////////////////////////////
///             IX.   ATOMIC FETCH-AND-MODIFY OPERATIONS                    ///
///////////////////////////////////////////////////////////////////////////////

// Fetch-and-add operations
int                d_atomic_fetch_add_int(d_atomic_int* _obj, int _arg);
unsigned int       d_atomic_fetch_add_uint(d_atomic_uint* _obj, unsigned int _arg);
long               d_atomic_fetch_add_long(d_atomic_long* _obj, long _arg);
unsigned long      d_atomic_fetch_add_ulong(d_atomic_ulong* _obj, unsigned long _arg);
long long          d_atomic_fetch_add_llong(d_atomic_llong* _obj, long long _arg);
unsigned long long d_atomic_fetch_add_ullong(d_atomic_ullong* _obj, unsigned long long _arg);
size_t             d_atomic_fetch_add_size(d_atomic_size_t* _obj, size_t _arg);

// Fetch-and-subtract operations
int                d_atomic_fetch_sub_int(d_atomic_int* _obj, int _arg);
unsigned int       d_atomic_fetch_sub_uint(d_atomic_uint* _obj, unsigned int _arg);
long               d_atomic_fetch_sub_long(d_atomic_long* _obj, long _arg);
unsigned long      d_atomic_fetch_sub_ulong(d_atomic_ulong* _obj, unsigned long _arg);
long long          d_atomic_fetch_sub_llong(d_atomic_llong* _obj, long long _arg);
unsigned long long d_atomic_fetch_sub_ullong(d_atomic_ullong* _obj, unsigned long long _arg);
size_t             d_atomic_fetch_sub_size(d_atomic_size_t* _obj, size_t _arg);

// Fetch-and-bitwise-or operations
int                d_atomic_fetch_or_int(d_atomic_int* _obj, int _arg);
unsigned int       d_atomic_fetch_or_uint(d_atomic_uint* _obj, unsigned int _arg);
long               d_atomic_fetch_or_long(d_atomic_long* _obj, long _arg);
unsigned long      d_atomic_fetch_or_ulong(d_atomic_ulong* _obj, unsigned long _arg);
long long          d_atomic_fetch_or_llong(d_atomic_llong* _obj, long long _arg);
unsigned long long d_atomic_fetch_or_ullong(d_atomic_ullong* _obj, unsigned long long _arg);
size_t             d_atomic_fetch_or_size(d_atomic_size_t* _obj, size_t _arg);

// Fetch-and-bitwise-xor operations
int                d_atomic_fetch_xor_int(d_atomic_int* _obj, int _arg);
unsigned int       d_atomic_fetch_xor_uint(d_atomic_uint* _obj, unsigned int _arg);
long               d_atomic_fetch_xor_long(d_atomic_long* _obj, long _arg);
unsigned long      d_atomic_fetch_xor_ulong(d_atomic_ulong* _obj, unsigned long _arg);
long long          d_atomic_fetch_xor_llong(d_atomic_llong* _obj, long long _arg);
unsigned long long d_atomic_fetch_xor_ullong(d_atomic_ullong* _obj, unsigned long long _arg);
size_t             d_atomic_fetch_xor_size(d_atomic_size_t* _obj, size_t _arg);

// Fetch-and-bitwise-and operations
int                d_atomic_fetch_and_int(d_atomic_int* _obj, int _arg);
unsigned int       d_atomic_fetch_and_uint(d_atomic_uint* _obj, unsigned int _arg);
long               d_atomic_fetch_and_long(d_atomic_long* _obj, long _arg);
unsigned long      d_atomic_fetch_and_ulong(d_atomic_ulong* _obj, unsigned long _arg);
long long          d_atomic_fetch_and_llong(d_atomic_llong* _obj, long long _arg);
unsigned long long d_atomic_fetch_and_ullong(d_atomic_ullong* _obj, unsigned long long _arg);
size_t             d_atomic_fetch_and_size(d_atomic_size_t* _obj, size_t _arg);

// Explicit memory order variants for fetch operations
int                d_atomic_fetch_add_int_explicit(d_atomic_int* _obj, int _arg, d_memory_order _order);
unsigned int       d_atomic_fetch_add_uint_explicit(d_atomic_uint* _obj, unsigned int _arg, d_memory_order _order);
long               d_atomic_fetch_add_long_explicit(d_atomic_long* _obj, long _arg, d_memory_order _order);
unsigned long      d_atomic_fetch_add_ulong_explicit(d_atomic_ulong* _obj, unsigned long _arg, d_memory_order _order);
long long          d_atomic_fetch_add_llong_explicit(d_atomic_llong* _obj, long long _arg, d_memory_order _order);
unsigned long long d_atomic_fetch_add_ullong_explicit(d_atomic_ullong* _obj, unsigned long long _arg, d_memory_order _order);
size_t             d_atomic_fetch_add_size_explicit(d_atomic_size_t* _obj, size_t _arg, d_memory_order _order);

int                d_atomic_fetch_sub_int_explicit(d_atomic_int* _obj, int _arg, d_memory_order _order);
unsigned int       d_atomic_fetch_sub_uint_explicit(d_atomic_uint* _obj, unsigned int _arg, d_memory_order _order);
long               d_atomic_fetch_sub_long_explicit(d_atomic_long* _obj, long _arg, d_memory_order _order);
unsigned long      d_atomic_fetch_sub_ulong_explicit(d_atomic_ulong* _obj, unsigned long _arg, d_memory_order _order);
long long          d_atomic_fetch_sub_llong_explicit(d_atomic_llong* _obj, long long _arg, d_memory_order _order);
unsigned long long d_atomic_fetch_sub_ullong_explicit(d_atomic_ullong* _obj, unsigned long long _arg, d_memory_order _order);
size_t             d_atomic_fetch_sub_size_explicit(d_atomic_size_t* _obj, size_t _arg, d_memory_order _order);

int                d_atomic_fetch_or_int_explicit(d_atomic_int* _obj, int _arg, d_memory_order _order);
unsigned int       d_atomic_fetch_or_uint_explicit(d_atomic_uint* _obj, unsigned int _arg, d_memory_order _order);
long               d_atomic_fetch_or_long_explicit(d_atomic_long* _obj, long _arg, d_memory_order _order);
unsigned long      d_atomic_fetch_or_ulong_explicit(d_atomic_ulong* _obj, unsigned long _arg, d_memory_order _order);
long long          d_atomic_fetch_or_llong_explicit(d_atomic_llong* _obj, long long _arg, d_memory_order _order);
unsigned long long d_atomic_fetch_or_ullong_explicit(d_atomic_ullong* _obj, unsigned long long _arg, d_memory_order _order);
size_t             d_atomic_fetch_or_size_explicit(d_atomic_size_t* _obj, size_t _arg, d_memory_order _order);

int                d_atomic_fetch_xor_int_explicit(d_atomic_int* _obj, int _arg, d_memory_order _order);
unsigned int       d_atomic_fetch_xor_uint_explicit(d_atomic_uint* _obj, unsigned int _arg, d_memory_order _order);
long               d_atomic_fetch_xor_long_explicit(d_atomic_long* _obj, long _arg, d_memory_order _order);
unsigned long      d_atomic_fetch_xor_ulong_explicit(d_atomic_ulong* _obj, unsigned long _arg, d_memory_order _order);
long long          d_atomic_fetch_xor_llong_explicit(d_atomic_llong* _obj, long long _arg, d_memory_order _order);
unsigned long long d_atomic_fetch_xor_ullong_explicit(d_atomic_ullong* _obj, unsigned long long _arg, d_memory_order _order);
size_t             d_atomic_fetch_xor_size_explicit(d_atomic_size_t* _obj, size_t _arg, d_memory_order _order);

int                d_atomic_fetch_and_int_explicit(d_atomic_int* _obj, int _arg, d_memory_order _order);
unsigned int       d_atomic_fetch_and_uint_explicit(d_atomic_uint* _obj, unsigned int _arg, d_memory_order _order);
long               d_atomic_fetch_and_long_explicit(d_atomic_long* _obj, long _arg, d_memory_order _order);
unsigned long      d_atomic_fetch_and_ulong_explicit(d_atomic_ulong* _obj, unsigned long _arg, d_memory_order _order);
long long          d_atomic_fetch_and_llong_explicit(d_atomic_llong* _obj, long long _arg, d_memory_order _order);
unsigned long long d_atomic_fetch_and_ullong_explicit(d_atomic_ullong* _obj, unsigned long long _arg, d_memory_order _order);
size_t             d_atomic_fetch_and_size_explicit(d_atomic_size_t* _obj, size_t _arg, d_memory_order _order);


///////////////////////////////////////////////////////////////////////////////
///             X.    MEMORY ORDERING AND FENCES                            ///
///////////////////////////////////////////////////////////////////////////////

void               d_atomic_thread_fence(d_memory_order _order);
void               d_atomic_signal_fence(d_memory_order _order);
bool               d_atomic_is_lock_free_1(void);   // 1-byte atomics
bool               d_atomic_is_lock_free_2(void);   // 2-byte atomics
bool               d_atomic_is_lock_free_4(void);   // 4-byte atomics
bool               d_atomic_is_lock_free_8(void);   // 8-byte atomics


#endif  // DJINTERP_ATOMIC_
