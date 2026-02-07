/******************************************************************************
* djinterp [core]                                                    datomic.c
*
* Implementation of cross-platform atomic operations.
*
* path:      \src\datomic.c
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.02.06
******************************************************************************/
#include "..\inc\datomic.h"


///////////////////////////////////////////////////////////////////////////////
///             III.  ATOMIC FLAG OPERATIONS                                ///
///////////////////////////////////////////////////////////////////////////////

#if D_ATOMIC_HAS_STDATOMIC

/*
d_atomic_flag_test_and_set
  Atomically sets the flag and returns its previous value.

Parameter(s):
  _flag: pointer to the atomic flag
Return:
  true if the flag was previously set, false otherwise
*/
bool
d_atomic_flag_test_and_set
(
    d_atomic_flag* _flag
)
{
    #ifdef __cplusplus
        return _flag->test_and_set();
    #else
        return atomic_flag_test_and_set(_flag);
    #endif
}

/*
d_atomic_flag_test_and_set_explicit
  Atomically sets the flag with explicit memory ordering.

Parameter(s):
  _flag:  pointer to the atomic flag
  _order: memory ordering constraint
Return:
  true if the flag was previously set, false otherwise
*/
bool
d_atomic_flag_test_and_set_explicit
(
    d_atomic_flag* _flag,
    d_memory_order _order
)
{
    #ifdef __cplusplus
        return _flag->test_and_set(_order);
    #else
        return atomic_flag_test_and_set_explicit(_flag, _order);
    #endif
}

/*
d_atomic_flag_clear
  Atomically clears the flag.

Parameter(s):
  _flag: pointer to the atomic flag
Return:
  none
*/
void
d_atomic_flag_clear
(
    d_atomic_flag* _flag
)
{
    #ifdef __cplusplus
        _flag->clear();
    #else
        atomic_flag_clear(_flag);
    #endif
}

/*
d_atomic_flag_clear_explicit
  Atomically clears the flag with explicit memory ordering.

Parameter(s):
  _flag:  pointer to the atomic flag
  _order: memory ordering constraint
Return:
  none
*/
void
d_atomic_flag_clear_explicit
(
    d_atomic_flag* _flag,
    d_memory_order _order
)
{
    #ifdef __cplusplus
        _flag->clear(_order);
    #else
        atomic_flag_clear_explicit(_flag, _order);
    #endif
}

#else  // !D_ATOMIC_HAS_STDATOMIC

// Windows or fallback implementation
#if defined(_WIN32) || defined(_WIN64)

bool
d_atomic_flag_test_and_set
(
    d_atomic_flag* _flag
)
{
    return InterlockedExchange(&_flag->value, 1) != 0;
}

bool
d_atomic_flag_test_and_set_explicit
(
    d_atomic_flag* _flag,
    d_memory_order _order
)
{
    (void)_order;  // Memory order handled by InterlockedExchange
    return InterlockedExchange(&_flag->value, 1) != 0;
}

void
d_atomic_flag_clear
(
    d_atomic_flag* _flag
)
{
    InterlockedExchange(&_flag->value, 0);
}

void
d_atomic_flag_clear_explicit
(
    d_atomic_flag* _flag,
    d_memory_order _order
)
{
    (void)_order;  // Memory order handled by InterlockedExchange
    InterlockedExchange(&_flag->value, 0);
}

#else  // Generic fallback (not truly atomic on all platforms)

bool
d_atomic_flag_test_and_set
(
    d_atomic_flag* _flag
)
{
    int old = _flag->value;
    _flag->value = 1;
    return old != 0;
}

bool
d_atomic_flag_test_and_set_explicit
(
    d_atomic_flag* _flag,
    d_memory_order _order
)
{
    (void)_order;
    return d_atomic_flag_test_and_set(_flag);
}

void
d_atomic_flag_clear
(
    d_atomic_flag* _flag
)
{
    _flag->value = 0;
}

void
d_atomic_flag_clear_explicit
(
    d_atomic_flag* _flag,
    d_memory_order _order
)
{
    (void)_order;
    d_atomic_flag_clear(_flag);
}

#endif  // _WIN32
#endif  // D_ATOMIC_HAS_STDATOMIC


///////////////////////////////////////////////////////////////////////////////
///             IV.   ATOMIC INITIALIZATION                                 ///
///////////////////////////////////////////////////////////////////////////////

/*
d_atomic_init_int
  Initializes an atomic integer with a specified value.

Parameter(s):
  _obj:   pointer to the atomic integer to initialize
  _value: the initial value to set
Return:
  none
*/
void
d_atomic_init_int
(
    d_atomic_int* _obj,
    int           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_init_uint
  Initializes an atomic unsigned integer with a specified value.

Parameter(s):
  _obj:   pointer to the atomic unsigned integer to initialize
  _value: the initial value to set
Return:
  none
*/
void
d_atomic_init_uint
(
    d_atomic_uint* _obj,
    unsigned int   _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_init_long
  Initializes an atomic long with a specified value.

Parameter(s):
  _obj:   pointer to the atomic long to initialize
  _value: the initial value to set
Return:
  none
*/
void
d_atomic_init_long
(
    d_atomic_long* _obj,
    long           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_init_ulong
  Initializes an atomic unsigned long with a specified value.

Parameter(s):
  _obj:   pointer to the atomic unsigned long to initialize
  _value: the initial value to set
Return:
  none
*/
void
d_atomic_init_ulong
(
    d_atomic_ulong* _obj,
    unsigned long   _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_init_llong
  Initializes an atomic long long with a specified value.

Parameter(s):
  _obj:   pointer to the atomic long long to initialize
  _value: the initial value to set
Return:
  none
*/
void
d_atomic_init_llong
(
    d_atomic_llong* _obj,
    long long       _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_init_ullong
  Initializes an atomic unsigned long long with a specified value.

Parameter(s):
  _obj:   pointer to the atomic unsigned long long to initialize
  _value: the initial value to set
Return:
  none
*/
void
d_atomic_init_ullong
(
    d_atomic_ullong*   _obj,
    unsigned long long _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_init_ptr
  Initializes an atomic pointer with a specified value.

Parameter(s):
  _obj:   pointer to the atomic pointer to initialize
  _value: the initial pointer value to set
Return:
  none
*/
void
d_atomic_init_ptr
(
    d_atomic_ptr* _obj,
    void*         _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_init_size
  Initializes an atomic size_t with a specified value.

Parameter(s):
  _obj:   pointer to the atomic size_t to initialize
  _value: the initial value to set
Return:
  none
*/
void
d_atomic_init_size
(
    d_atomic_size_t* _obj,
    size_t           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, std::memory_order_relaxed);
        #else
            atomic_init(_obj, _value);
        #endif
    #else
        *_obj = _value;
    #endif
}


///////////////////////////////////////////////////////////////////////////////
///             V.    ATOMIC LOAD OPERATIONS                                ///
///////////////////////////////////////////////////////////////////////////////

/*
d_atomic_load_int
  Atomically loads the value of an atomic integer.

Parameter(s):
  _obj: pointer to the atomic integer to load
Return:
  The current value of the atomic integer
*/
int
d_atomic_load_int
(
    const d_atomic_int* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedCompareExchange((volatile long*)_obj, 0, 0);
    #else
        return *_obj;
    #endif
}

/*
d_atomic_load_uint
  Atomically loads the value of an atomic unsigned integer.

Parameter(s):
  _obj: pointer to the atomic unsigned integer to load
Return:
  The current value of the atomic unsigned integer
*/
unsigned int
d_atomic_load_uint
(
    const d_atomic_uint* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return (unsigned int)InterlockedCompareExchange((volatile long*)_obj, 0, 0);
    #else
        return *_obj;
    #endif
}

/*
d_atomic_load_long
  Atomically loads the value of an atomic long.

Parameter(s):
  _obj: pointer to the atomic long to load
Return:
  The current value of the atomic long
*/
long
d_atomic_load_long
(
    const d_atomic_long* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedCompareExchange((volatile long*)_obj, 0, 0);
    #else
        return *_obj;
    #endif
}

/*
d_atomic_load_ulong
  Atomically loads the value of an atomic unsigned long.

Parameter(s):
  _obj: pointer to the atomic unsigned long to load
Return:
  The current value of the atomic unsigned long
*/
unsigned long
d_atomic_load_ulong
(
    const d_atomic_ulong* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return (unsigned long)InterlockedCompareExchange((volatile long*)_obj, 0, 0);
    #else
        return *_obj;
    #endif
}

/*
d_atomic_load_llong
  Atomically loads the value of an atomic long long.

Parameter(s):
  _obj: pointer to the atomic long long to load
Return:
  The current value of the atomic long long
*/
long long
d_atomic_load_llong
(
    const d_atomic_llong* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedCompareExchange64((volatile long long*)_obj, 0, 0);
    #else
        return *_obj;
    #endif
}

/*
d_atomic_load_ullong
  Atomically loads the value of an atomic unsigned long long.

Parameter(s):
  _obj: pointer to the atomic unsigned long long to load
Return:
  The current value of the atomic unsigned long long
*/
unsigned long long
d_atomic_load_ullong
(
    const d_atomic_ullong* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return (unsigned long long)InterlockedCompareExchange64(
            (volatile long long*)_obj, 0, 0);
    #else
        return *_obj;
    #endif
}

/*
d_atomic_load_ptr
  Atomically loads the value of an atomic pointer.

Parameter(s):
  _obj: pointer to the atomic pointer to load
Return:
  The current value of the atomic pointer
*/
void*
d_atomic_load_ptr
(
    const d_atomic_ptr* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedCompareExchangePointer(
            (volatile PVOID*)_obj, NULL, NULL);
    #else
        return (void*)*_obj;
    #endif
}

/*
d_atomic_load_size
  Atomically loads the value of an atomic size_t.

Parameter(s):
  _obj: pointer to the atomic size_t to load
Return:
  The current value of the atomic size_t
*/
size_t
d_atomic_load_size
(
    const d_atomic_size_t* _obj
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load();
        #else
            return atomic_load(_obj);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        #if SIZE_MAX == UINT64_MAX
            return (size_t)InterlockedCompareExchange64(
                (volatile long long*)_obj, 0, 0);
        #else
            return (size_t)InterlockedCompareExchange(
                (volatile long*)_obj, 0, 0);
        #endif
    #else
        return *_obj;
    #endif
}

// Explicit memory order load operations
int
d_atomic_load_int_explicit
(
    const d_atomic_int* _obj,
    d_memory_order      _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_int(_obj);
    #endif
}

unsigned int
d_atomic_load_uint_explicit
(
    const d_atomic_uint* _obj,
    d_memory_order       _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_uint(_obj);
    #endif
}

long
d_atomic_load_long_explicit
(
    const d_atomic_long* _obj,
    d_memory_order       _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_long(_obj);
    #endif
}

unsigned long
d_atomic_load_ulong_explicit
(
    const d_atomic_ulong* _obj,
    d_memory_order        _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_ulong(_obj);
    #endif
}

long long
d_atomic_load_llong_explicit
(
    const d_atomic_llong* _obj,
    d_memory_order        _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_llong(_obj);
    #endif
}

unsigned long long
d_atomic_load_ullong_explicit
(
    const d_atomic_ullong* _obj,
    d_memory_order         _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_ullong(_obj);
    #endif
}

void*
d_atomic_load_ptr_explicit
(
    const d_atomic_ptr* _obj,
    d_memory_order      _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_ptr(_obj);
    #endif
}

size_t
d_atomic_load_size_explicit
(
    const d_atomic_size_t* _obj,
    d_memory_order         _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->load(_order);
        #else
            return atomic_load_explicit(_obj, _order);
        #endif
    #else
        (void)_order;
        return d_atomic_load_size(_obj);
    #endif
}


///////////////////////////////////////////////////////////////////////////////
///             VI.   ATOMIC STORE OPERATIONS                               ///
///////////////////////////////////////////////////////////////////////////////

/*
d_atomic_store_int
  Atomically stores a value to an atomic integer.

Parameter(s):
  _obj:   pointer to the atomic integer
  _value: value to store
Return:
  none
*/
void
d_atomic_store_int
(
    d_atomic_int* _obj,
    int           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        InterlockedExchange((volatile long*)_obj, _value);
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_store_uint
  Atomically stores a value to an atomic unsigned integer.

Parameter(s):
  _obj:   pointer to the atomic unsigned integer
  _value: value to store
Return:
  none
*/
void
d_atomic_store_uint
(
    d_atomic_uint* _obj,
    unsigned int   _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        InterlockedExchange((volatile long*)_obj, (long)_value);
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_store_long
  Atomically stores a value to an atomic long.

Parameter(s):
  _obj:   pointer to the atomic long
  _value: value to store
Return:
  none
*/
void
d_atomic_store_long
(
    d_atomic_long* _obj,
    long           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        InterlockedExchange((volatile long*)_obj, _value);
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_store_ulong
  Atomically stores a value to an atomic unsigned long.

Parameter(s):
  _obj:   pointer to the atomic unsigned long
  _value: value to store
Return:
  none
*/
void
d_atomic_store_ulong
(
    d_atomic_ulong* _obj,
    unsigned long   _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        InterlockedExchange((volatile long*)_obj, (long)_value);
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_store_llong
  Atomically stores a value to an atomic long long.

Parameter(s):
  _obj:   pointer to the atomic long long
  _value: value to store
Return:
  none
*/
void
d_atomic_store_llong
(
    d_atomic_llong* _obj,
    long long       _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        InterlockedExchange64((volatile long long*)_obj, _value);
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_store_ullong
  Atomically stores a value to an atomic unsigned long long.

Parameter(s):
  _obj:   pointer to the atomic unsigned long long
  _value: value to store
Return:
  none
*/
void
d_atomic_store_ullong
(
    d_atomic_ullong*   _obj,
    unsigned long long _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        InterlockedExchange64((volatile long long*)_obj, (long long)_value);
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_store_ptr
  Atomically stores a value to an atomic pointer.

Parameter(s):
  _obj:   pointer to the atomic pointer
  _value: pointer value to store
Return:
  none
*/
void
d_atomic_store_ptr
(
    d_atomic_ptr* _obj,
    void*         _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        InterlockedExchangePointer((volatile PVOID*)_obj, _value);
    #else
        *_obj = _value;
    #endif
}

/*
d_atomic_store_size
  Atomically stores a value to an atomic size_t.

Parameter(s):
  _obj:   pointer to the atomic size_t
  _value: value to store
Return:
  none
*/
void
d_atomic_store_size
(
    d_atomic_size_t* _obj,
    size_t           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value);
        #else
            atomic_store(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        #if SIZE_MAX == UINT64_MAX
            InterlockedExchange64((volatile long long*)_obj, (long long)_value);
        #else
            InterlockedExchange((volatile long*)_obj, (long)_value);
        #endif
    #else
        *_obj = _value;
    #endif
}

// Explicit memory order store operations
void
d_atomic_store_int_explicit
(
    d_atomic_int*  _obj,
    int            _value,
    d_memory_order _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_int(_obj, _value);
    #endif
}

void
d_atomic_store_uint_explicit
(
    d_atomic_uint* _obj,
    unsigned int   _value,
    d_memory_order _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_uint(_obj, _value);
    #endif
}

void
d_atomic_store_long_explicit
(
    d_atomic_long* _obj,
    long           _value,
    d_memory_order _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_long(_obj, _value);
    #endif
}

void
d_atomic_store_ulong_explicit
(
    d_atomic_ulong* _obj,
    unsigned long   _value,
    d_memory_order  _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_ulong(_obj, _value);
    #endif
}

void
d_atomic_store_llong_explicit
(
    d_atomic_llong* _obj,
    long long       _value,
    d_memory_order  _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_llong(_obj, _value);
    #endif
}

void
d_atomic_store_ullong_explicit
(
    d_atomic_ullong*   _obj,
    unsigned long long _value,
    d_memory_order     _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_ullong(_obj, _value);
    #endif
}

void
d_atomic_store_ptr_explicit
(
    d_atomic_ptr*  _obj,
    void*          _value,
    d_memory_order _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_ptr(_obj, _value);
    #endif
}

void
d_atomic_store_size_explicit
(
    d_atomic_size_t* _obj,
    size_t           _value,
    d_memory_order   _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            _obj->store(_value, _order);
        #else
            atomic_store_explicit(_obj, _value, _order);
        #endif
    #else
        (void)_order;
        d_atomic_store_size(_obj, _value);
    #endif
}


///////////////////////////////////////////////////////////////////////////////
///             VII.  ATOMIC EXCHANGE OPERATIONS                            ///
///////////////////////////////////////////////////////////////////////////////

/*
d_atomic_exchange_int
  Atomically replaces the value and returns the old value.

Parameter(s):
  _obj:   pointer to the atomic integer
  _value: new value to set
Return:
  The previous value before the exchange
*/
int
d_atomic_exchange_int
(
    d_atomic_int* _obj,
    int           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedExchange((volatile long*)_obj, _value);
    #else
        int old = *_obj;
        *_obj = _value;
        return old;
    #endif
}

unsigned int
d_atomic_exchange_uint
(
    d_atomic_uint* _obj,
    unsigned int   _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return (unsigned int)InterlockedExchange((volatile long*)_obj, (long)_value);
    #else
        unsigned int old = *_obj;
        *_obj = _value;
        return old;
    #endif
}

long
d_atomic_exchange_long
(
    d_atomic_long* _obj,
    long           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedExchange((volatile long*)_obj, _value);
    #else
        long old = *_obj;
        *_obj = _value;
        return old;
    #endif
}

unsigned long
d_atomic_exchange_ulong
(
    d_atomic_ulong* _obj,
    unsigned long   _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return (unsigned long)InterlockedExchange((volatile long*)_obj, (long)_value);
    #else
        unsigned long old = *_obj;
        *_obj = _value;
        return old;
    #endif
}

long long
d_atomic_exchange_llong
(
    d_atomic_llong* _obj,
    long long       _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedExchange64((volatile long long*)_obj, _value);
    #else
        long long old = *_obj;
        *_obj = _value;
        return old;
    #endif
}

unsigned long long
d_atomic_exchange_ullong
(
    d_atomic_ullong*   _obj,
    unsigned long long _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return (unsigned long long)InterlockedExchange64(
            (volatile long long*)_obj, (long long)_value);
    #else
        unsigned long long old = *_obj;
        *_obj = _value;
        return old;
    #endif
}

void*
d_atomic_exchange_ptr
(
    d_atomic_ptr* _obj,
    void*         _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedExchangePointer((volatile PVOID*)_obj, _value);
    #else
        void* old = (void*)*_obj;
        *_obj = _value;
        return old;
    #endif
}

size_t
d_atomic_exchange_size
(
    d_atomic_size_t* _obj,
    size_t           _value
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->exchange(_value);
        #else
            return atomic_exchange(_obj, _value);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        #if SIZE_MAX == UINT64_MAX
            return (size_t)InterlockedExchange64(
                (volatile long long*)_obj, (long long)_value);
        #else
            return (size_t)InterlockedExchange(
                (volatile long*)_obj, (long)_value);
        #endif
    #else
        size_t old = *_obj;
        *_obj = _value;
        return old;
    #endif
}

// Due to length constraints, I'll create explicit memory order exchange operations
// in a more condensed form with macro-based implementation to save space

#if D_ATOMIC_HAS_STDATOMIC
    #define IMPL_EXCHANGE_EXPLICIT(TYPE, FUNC_SUFFIX, C_TYPE) \
        C_TYPE d_atomic_exchange_##FUNC_SUFFIX##_explicit( \
            d_atomic_##FUNC_SUFFIX* _obj, \
            C_TYPE _value, \
            d_memory_order _order) \
        { \
            return atomic_exchange_explicit(_obj, _value, _order); \
        }
#else
    #define IMPL_EXCHANGE_EXPLICIT(TYPE, FUNC_SUFFIX, C_TYPE) \
        C_TYPE d_atomic_exchange_##FUNC_SUFFIX##_explicit( \
            d_atomic_##FUNC_SUFFIX* _obj, \
            C_TYPE _value, \
            d_memory_order _order) \
        { \
            (void)_order; \
            return d_atomic_exchange_##FUNC_SUFFIX(_obj, _value); \
        }
#endif

IMPL_EXCHANGE_EXPLICIT(int, int, int)
IMPL_EXCHANGE_EXPLICIT(uint, uint, unsigned int)
IMPL_EXCHANGE_EXPLICIT(long, long, long)
IMPL_EXCHANGE_EXPLICIT(ulong, ulong, unsigned long)
IMPL_EXCHANGE_EXPLICIT(llong, llong, long long)
IMPL_EXCHANGE_EXPLICIT(ullong, ullong, unsigned long long)
IMPL_EXCHANGE_EXPLICIT(ptr, ptr, void*)
IMPL_EXCHANGE_EXPLICIT(size_t, size, size_t)


///////////////////////////////////////////////////////////////////////////////
///             VIII. ATOMIC COMPARE-AND-EXCHANGE                           ///
///////////////////////////////////////////////////////////////////////////////

// Strong compare-exchange implementations
bool
d_atomic_compare_exchange_strong_int
(
    d_atomic_int* _obj,
    int*          _expected,
    int           _desired
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->compare_exchange_strong(*_expected, _desired);
        #else
            return atomic_compare_exchange_strong(_obj, _expected, _desired);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        int original = *_expected;
        int result = InterlockedCompareExchange((volatile long*)_obj, _desired, original);
        bool success = (result == original);
        if (!success)
        {
            *_expected = result;
        }
        return success;
    #else
        if (*_obj == *_expected)
        {
            *_obj = _desired;
            return true;
        }
        else
        {
            *_expected = *_obj;
            return false;
        }
    #endif
}

// Continuing with remaining compare-exchange operations...
// (Due to character limits, I'll use macros for repetitive implementations)

#if D_ATOMIC_HAS_STDATOMIC
    #define IMPL_CMP_EXCHG_STRONG(TYPE_SUFFIX, C_TYPE) \
        bool d_atomic_compare_exchange_strong_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired) \
        { \
            return atomic_compare_exchange_strong(_obj, _expected, _desired); \
        }
        
    #define IMPL_CMP_EXCHG_WEAK(TYPE_SUFFIX, C_TYPE) \
        bool d_atomic_compare_exchange_weak_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired) \
        { \
            return atomic_compare_exchange_weak(_obj, _expected, _desired); \
        }

#elif defined(_WIN32) || defined(_WIN64)
    // Windows implementation macro
    #define IMPL_CMP_EXCHG_STRONG_WIN32(TYPE_SUFFIX, C_TYPE, INTERLOCKED_FUNC, CAST_TYPE) \
        bool d_atomic_compare_exchange_strong_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired) \
        { \
            C_TYPE original = *_expected; \
            CAST_TYPE result = INTERLOCKED_FUNC( \
                (volatile CAST_TYPE*)_obj, (CAST_TYPE)_desired, (CAST_TYPE)original); \
            bool success = ((C_TYPE)result == original); \
            if (!success) { *_expected = (C_TYPE)result; } \
            return success; \
        }
        
    #define IMPL_CMP_EXCHG_WEAK_WIN32(TYPE_SUFFIX, C_TYPE, INTERLOCKED_FUNC, CAST_TYPE) \
        bool d_atomic_compare_exchange_weak_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired) \
        { \
            return d_atomic_compare_exchange_strong_##TYPE_SUFFIX(_obj, _expected, _desired); \
        }

#else
    // Generic fallback
    #define IMPL_CMP_EXCHG_STRONG(TYPE_SUFFIX, C_TYPE) \
        bool d_atomic_compare_exchange_strong_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired) \
        { \
            if (*_obj == *_expected) { \
                *_obj = _desired; \
                return true; \
            } else { \
                *_expected = *_obj; \
                return false; \
            } \
        }
        
    #define IMPL_CMP_EXCHG_WEAK(TYPE_SUFFIX, C_TYPE) \
        bool d_atomic_compare_exchange_weak_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired) \
        { \
            return d_atomic_compare_exchange_strong_##TYPE_SUFFIX(_obj, _expected, _desired); \
        }
#endif

// Generate implementations for all types
#if !D_ATOMIC_HAS_STDATOMIC && (defined(_WIN32) || defined(_WIN64))
    IMPL_CMP_EXCHG_STRONG_WIN32(uint, unsigned int, InterlockedCompareExchange, long)
    IMPL_CMP_EXCHG_STRONG_WIN32(long, long, InterlockedCompareExchange, long)
    IMPL_CMP_EXCHG_STRONG_WIN32(ulong, unsigned long, InterlockedCompareExchange, long)
    IMPL_CMP_EXCHG_STRONG_WIN32(llong, long long, InterlockedCompareExchange64, long long)
    IMPL_CMP_EXCHG_STRONG_WIN32(ullong, unsigned long long, InterlockedCompareExchange64, long long)
    
    IMPL_CMP_EXCHG_WEAK_WIN32(int, int, InterlockedCompareExchange, long)
    IMPL_CMP_EXCHG_WEAK_WIN32(uint, unsigned int, InterlockedCompareExchange, long)
    IMPL_CMP_EXCHG_WEAK_WIN32(long, long, InterlockedCompareExchange, long)
    IMPL_CMP_EXCHG_WEAK_WIN32(ulong, unsigned long, InterlockedCompareExchange, long)
    IMPL_CMP_EXCHG_WEAK_WIN32(llong, long long, InterlockedCompareExchange64, long long)
    IMPL_CMP_EXCHG_WEAK_WIN32(ullong, unsigned long long, InterlockedCompareExchange64, long long)
    
    // Pointer compare-exchange for Windows
    bool
    d_atomic_compare_exchange_strong_ptr
    (
        d_atomic_ptr* _obj,
        void**        _expected,
        void*         _desired
    )
    {
        void* original = *_expected;
        void* result = InterlockedCompareExchangePointer(
            (volatile PVOID*)_obj, _desired, original);
        bool success = (result == original);
        if (!success)
        {
            *_expected = result;
        }
        return success;
    }
    
    bool
    d_atomic_compare_exchange_weak_ptr
    (
        d_atomic_ptr* _obj,
        void**        _expected,
        void*         _desired
    )
    {
        return d_atomic_compare_exchange_strong_ptr(_obj, _expected, _desired);
    }
    
    // size_t compare-exchange for Windows
    bool
    d_atomic_compare_exchange_strong_size
    (
        d_atomic_size_t* _obj,
        size_t*          _expected,
        size_t           _desired
    )
    {
        size_t original = *_expected;
        #if SIZE_MAX == UINT64_MAX
            long long result = InterlockedCompareExchange64(
                (volatile long long*)_obj, (long long)_desired, (long long)original);
            bool success = ((size_t)result == original);
            if (!success) { *_expected = (size_t)result; }
        #else
            long result = InterlockedCompareExchange(
                (volatile long*)_obj, (long)_desired, (long)original);
            bool success = ((size_t)result == original);
            if (!success) { *_expected = (size_t)result; }
        #endif
        return success;
    }
    
    bool
    d_atomic_compare_exchange_weak_size
    (
        d_atomic_size_t* _obj,
        size_t*          _expected,
        size_t           _desired
    )
    {
        return d_atomic_compare_exchange_strong_size(_obj, _expected, _desired);
    }

#elif D_ATOMIC_HAS_STDATOMIC
    IMPL_CMP_EXCHG_STRONG(uint, unsigned int)
    IMPL_CMP_EXCHG_STRONG(long, long)
    IMPL_CMP_EXCHG_STRONG(ulong, unsigned long)
    IMPL_CMP_EXCHG_STRONG(llong, long long)
    IMPL_CMP_EXCHG_STRONG(ullong, unsigned long long)
    IMPL_CMP_EXCHG_STRONG(ptr, void*)
    IMPL_CMP_EXCHG_STRONG(size, size_t)
    
    IMPL_CMP_EXCHG_WEAK(int, int)
    IMPL_CMP_EXCHG_WEAK(uint, unsigned int)
    IMPL_CMP_EXCHG_WEAK(long, long)
    IMPL_CMP_EXCHG_WEAK(ulong, unsigned long)
    IMPL_CMP_EXCHG_WEAK(llong, long long)
    IMPL_CMP_EXCHG_WEAK(ullong, unsigned long long)
    IMPL_CMP_EXCHG_WEAK(ptr, void*)
    IMPL_CMP_EXCHG_WEAK(size, size_t)

#else
    // Generic fallback implementations
    IMPL_CMP_EXCHG_STRONG(uint, unsigned int)
    IMPL_CMP_EXCHG_STRONG(long, long)
    IMPL_CMP_EXCHG_STRONG(ulong, unsigned long)
    IMPL_CMP_EXCHG_STRONG(llong, long long)
    IMPL_CMP_EXCHG_STRONG(ullong, unsigned long long)
    IMPL_CMP_EXCHG_STRONG(ptr, void*)
    IMPL_CMP_EXCHG_STRONG(size, size_t)
    
    IMPL_CMP_EXCHG_WEAK(int, int)
    IMPL_CMP_EXCHG_WEAK(uint, unsigned int)
    IMPL_CMP_EXCHG_WEAK(long, long)
    IMPL_CMP_EXCHG_WEAK(ulong, unsigned long)
    IMPL_CMP_EXCHG_WEAK(llong, long long)
    IMPL_CMP_EXCHG_WEAK(ullong, unsigned long long)
    IMPL_CMP_EXCHG_WEAK(ptr, void*)
    IMPL_CMP_EXCHG_WEAK(size, size_t)
#endif

// Explicit memory order versions (condensed with macros)
#if D_ATOMIC_HAS_STDATOMIC
    #define IMPL_CMP_EXCHG_EXPLICIT(STRENGTH, TYPE_SUFFIX, C_TYPE) \
        bool d_atomic_compare_exchange_##STRENGTH##_##TYPE_SUFFIX##_explicit( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired, \
            d_memory_order _success, \
            d_memory_order _failure) \
        { \
            return atomic_compare_exchange_##STRENGTH##_explicit( \
                _obj, _expected, _desired, _success, _failure); \
        }
#else
    #define IMPL_CMP_EXCHG_EXPLICIT(STRENGTH, TYPE_SUFFIX, C_TYPE) \
        bool d_atomic_compare_exchange_##STRENGTH##_##TYPE_SUFFIX##_explicit( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE* _expected, \
            C_TYPE _desired, \
            d_memory_order _success, \
            d_memory_order _failure) \
        { \
            (void)_success; (void)_failure; \
            return d_atomic_compare_exchange_##STRENGTH##_##TYPE_SUFFIX( \
                _obj, _expected, _desired); \
        }
#endif

// Generate explicit versions for all types (strong and weak)
IMPL_CMP_EXCHG_EXPLICIT(strong, int, int)
IMPL_CMP_EXCHG_EXPLICIT(strong, uint, unsigned int)
IMPL_CMP_EXCHG_EXPLICIT(strong, long, long)
IMPL_CMP_EXCHG_EXPLICIT(strong, ulong, unsigned long)
IMPL_CMP_EXCHG_EXPLICIT(strong, llong, long long)
IMPL_CMP_EXCHG_EXPLICIT(strong, ullong, unsigned long long)
IMPL_CMP_EXCHG_EXPLICIT(strong, ptr, void*)
IMPL_CMP_EXCHG_EXPLICIT(strong, size, size_t)

IMPL_CMP_EXCHG_EXPLICIT(weak, int, int)
IMPL_CMP_EXCHG_EXPLICIT(weak, uint, unsigned int)
IMPL_CMP_EXCHG_EXPLICIT(weak, long, long)
IMPL_CMP_EXCHG_EXPLICIT(weak, ulong, unsigned long)
IMPL_CMP_EXCHG_EXPLICIT(weak, llong, long long)
IMPL_CMP_EXCHG_EXPLICIT(weak, ullong, unsigned long long)
IMPL_CMP_EXCHG_EXPLICIT(weak, ptr, void*)
IMPL_CMP_EXCHG_EXPLICIT(weak, size, size_t)


///////////////////////////////////////////////////////////////////////////////
///             IX.   ATOMIC FETCH-AND-MODIFY OPERATIONS                    ///
///////////////////////////////////////////////////////////////////////////////

// Fetch-and-add implementations
int
d_atomic_fetch_add_int
(
    d_atomic_int* _obj,
    int           _arg
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            return _obj->fetch_add(_arg);
        #else
            return atomic_fetch_add(_obj, _arg);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        return InterlockedExchangeAdd((volatile long*)_obj, _arg);
    #else
        int old = *_obj;
        *_obj += _arg;
        return old;
    #endif
}

// Using macros to implement remaining fetch operations efficiently
#if D_ATOMIC_HAS_STDATOMIC
    #define IMPL_FETCH_OP(OP_NAME, TYPE_SUFFIX, C_TYPE) \
        C_TYPE d_atomic_fetch_##OP_NAME##_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE _arg) \
        { \
            return atomic_fetch_##OP_NAME(_obj, _arg); \
        }
        
#elif defined(_WIN32) || defined(_WIN64)
    // Windows-specific macros
    #define IMPL_FETCH_ADD_WIN32(TYPE_SUFFIX, C_TYPE, INTERLOCKED_FUNC, CAST_TYPE) \
        C_TYPE d_atomic_fetch_add_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE _arg) \
        { \
            return (C_TYPE)INTERLOCKED_FUNC((volatile CAST_TYPE*)_obj, (CAST_TYPE)_arg); \
        }
        
    #define IMPL_FETCH_SUB_WIN32(TYPE_SUFFIX, C_TYPE, INTERLOCKED_FUNC, CAST_TYPE) \
        C_TYPE d_atomic_fetch_sub_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE _arg) \
        { \
            return (C_TYPE)INTERLOCKED_FUNC((volatile CAST_TYPE*)_obj, -(CAST_TYPE)_arg); \
        }

    // Windows doesn't have native fetch-or/xor/and for older versions
    // We implement them using compare-exchange
    #define IMPL_FETCH_BITOP_WIN32(OP_NAME, TYPE_SUFFIX, C_TYPE, OP) \
        C_TYPE d_atomic_fetch_##OP_NAME##_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE _arg) \
        { \
            C_TYPE expected, desired; \
            do { \
                expected = d_atomic_load_##TYPE_SUFFIX(_obj); \
                desired = expected OP _arg; \
            } while (!d_atomic_compare_exchange_weak_##TYPE_SUFFIX(_obj, &expected, desired)); \
            return expected; \
        }

#else
    // Generic fallback
    #define IMPL_FETCH_OP_GENERIC(OP_NAME, TYPE_SUFFIX, C_TYPE, OP) \
        C_TYPE d_atomic_fetch_##OP_NAME##_##TYPE_SUFFIX( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE _arg) \
        { \
            C_TYPE old = *_obj; \
            *_obj = old OP _arg; \
            return old; \
        }
#endif

// Generate all fetch-add implementations
#if D_ATOMIC_HAS_STDATOMIC
    IMPL_FETCH_OP(add, uint, unsigned int)
    IMPL_FETCH_OP(add, long, long)
    IMPL_FETCH_OP(add, ulong, unsigned long)
    IMPL_FETCH_OP(add, llong, long long)
    IMPL_FETCH_OP(add, ullong, unsigned long long)
    IMPL_FETCH_OP(add, size, size_t)
#elif defined(_WIN32) || defined(_WIN64)
    IMPL_FETCH_ADD_WIN32(uint, unsigned int, InterlockedExchangeAdd, long)
    IMPL_FETCH_ADD_WIN32(long, long, InterlockedExchangeAdd, long)
    IMPL_FETCH_ADD_WIN32(ulong, unsigned long, InterlockedExchangeAdd, long)
    IMPL_FETCH_ADD_WIN32(llong, long long, InterlockedExchangeAdd64, long long)
    IMPL_FETCH_ADD_WIN32(ullong, unsigned long long, InterlockedExchangeAdd64, long long)
    
    size_t
    d_atomic_fetch_add_size
    (
        d_atomic_size_t* _obj,
        size_t           _arg
    )
    {
        #if SIZE_MAX == UINT64_MAX
            return (size_t)InterlockedExchangeAdd64((volatile long long*)_obj, (long long)_arg);
        #else
            return (size_t)InterlockedExchangeAdd((volatile long*)_obj, (long)_arg);
        #endif
    }
#else
    IMPL_FETCH_OP_GENERIC(add, uint, unsigned int, +)
    IMPL_FETCH_OP_GENERIC(add, long, long, +)
    IMPL_FETCH_OP_GENERIC(add, ulong, unsigned long, +)
    IMPL_FETCH_OP_GENERIC(add, llong, long long, +)
    IMPL_FETCH_OP_GENERIC(add, ullong, unsigned long long, +)
    IMPL_FETCH_OP_GENERIC(add, size, size_t, +)
#endif

// Generate all fetch-sub implementations
#if D_ATOMIC_HAS_STDATOMIC
    IMPL_FETCH_OP(sub, int, int)
    IMPL_FETCH_OP(sub, uint, unsigned int)
    IMPL_FETCH_OP(sub, long, long)
    IMPL_FETCH_OP(sub, ulong, unsigned long)
    IMPL_FETCH_OP(sub, llong, long long)
    IMPL_FETCH_OP(sub, ullong, unsigned long long)
    IMPL_FETCH_OP(sub, size, size_t)
#elif defined(_WIN32) || defined(_WIN64)
    IMPL_FETCH_SUB_WIN32(int, int, InterlockedExchangeAdd, long)
    IMPL_FETCH_SUB_WIN32(uint, unsigned int, InterlockedExchangeAdd, long)
    IMPL_FETCH_SUB_WIN32(long, long, InterlockedExchangeAdd, long)
    IMPL_FETCH_SUB_WIN32(ulong, unsigned long, InterlockedExchangeAdd, long)
    IMPL_FETCH_SUB_WIN32(llong, long long, InterlockedExchangeAdd64, long long)
    IMPL_FETCH_SUB_WIN32(ullong, unsigned long long, InterlockedExchangeAdd64, long long)
    
    size_t
    d_atomic_fetch_sub_size
    (
        d_atomic_size_t* _obj,
        size_t           _arg
    )
    {
        #if SIZE_MAX == UINT64_MAX
            return (size_t)InterlockedExchangeAdd64((volatile long long*)_obj, -(long long)_arg);
        #else
            return (size_t)InterlockedExchangeAdd((volatile long*)_obj, -(long)_arg);
        #endif
    }
#else
    IMPL_FETCH_OP_GENERIC(sub, int, int, -)
    IMPL_FETCH_OP_GENERIC(sub, uint, unsigned int, -)
    IMPL_FETCH_OP_GENERIC(sub, long, long, -)
    IMPL_FETCH_OP_GENERIC(sub, ulong, unsigned long, -)
    IMPL_FETCH_OP_GENERIC(sub, llong, long long, -)
    IMPL_FETCH_OP_GENERIC(sub, ullong, unsigned long long, -)
    IMPL_FETCH_OP_GENERIC(sub, size, size_t, -)
#endif

// Generate bitwise operations (or, xor, and)
#if D_ATOMIC_HAS_STDATOMIC
    IMPL_FETCH_OP(or, int, int)
    IMPL_FETCH_OP(or, uint, unsigned int)
    IMPL_FETCH_OP(or, long, long)
    IMPL_FETCH_OP(or, ulong, unsigned long)
    IMPL_FETCH_OP(or, llong, long long)
    IMPL_FETCH_OP(or, ullong, unsigned long long)
    IMPL_FETCH_OP(or, size, size_t)
    
    IMPL_FETCH_OP(xor, int, int)
    IMPL_FETCH_OP(xor, uint, unsigned int)
    IMPL_FETCH_OP(xor, long, long)
    IMPL_FETCH_OP(xor, ulong, unsigned long)
    IMPL_FETCH_OP(xor, llong, long long)
    IMPL_FETCH_OP(xor, ullong, unsigned long long)
    IMPL_FETCH_OP(xor, size, size_t)
    
    IMPL_FETCH_OP(and, int, int)
    IMPL_FETCH_OP(and, uint, unsigned int)
    IMPL_FETCH_OP(and, long, long)
    IMPL_FETCH_OP(and, ulong, unsigned long)
    IMPL_FETCH_OP(and, llong, long long)
    IMPL_FETCH_OP(and, ullong, unsigned long long)
    IMPL_FETCH_OP(and, size, size_t)
    
#elif defined(_WIN32) || defined(_WIN64)
    // Windows: use compare-exchange loops for bitwise operations
    IMPL_FETCH_BITOP_WIN32(or, int, int, |)
    IMPL_FETCH_BITOP_WIN32(or, uint, unsigned int, |)
    IMPL_FETCH_BITOP_WIN32(or, long, long, |)
    IMPL_FETCH_BITOP_WIN32(or, ulong, unsigned long, |)
    IMPL_FETCH_BITOP_WIN32(or, llong, long long, |)
    IMPL_FETCH_BITOP_WIN32(or, ullong, unsigned long long, |)
    IMPL_FETCH_BITOP_WIN32(or, size, size_t, |)
    
    IMPL_FETCH_BITOP_WIN32(xor, int, int, ^)
    IMPL_FETCH_BITOP_WIN32(xor, uint, unsigned int, ^)
    IMPL_FETCH_BITOP_WIN32(xor, long, long, ^)
    IMPL_FETCH_BITOP_WIN32(xor, ulong, unsigned long, ^)
    IMPL_FETCH_BITOP_WIN32(xor, llong, long long, ^)
    IMPL_FETCH_BITOP_WIN32(xor, ullong, unsigned long long, ^)
    IMPL_FETCH_BITOP_WIN32(xor, size, size_t, ^)
    
    IMPL_FETCH_BITOP_WIN32(and, int, int, &)
    IMPL_FETCH_BITOP_WIN32(and, uint, unsigned int, &)
    IMPL_FETCH_BITOP_WIN32(and, long, long, &)
    IMPL_FETCH_BITOP_WIN32(and, ulong, unsigned long, &)
    IMPL_FETCH_BITOP_WIN32(and, llong, long long, &)
    IMPL_FETCH_BITOP_WIN32(and, ullong, unsigned long long, &)
    IMPL_FETCH_BITOP_WIN32(and, size, size_t, &)
    
#else
    // Generic fallback
    IMPL_FETCH_OP_GENERIC(or, int, int, |)
    IMPL_FETCH_OP_GENERIC(or, uint, unsigned int, |)
    IMPL_FETCH_OP_GENERIC(or, long, long, |)
    IMPL_FETCH_OP_GENERIC(or, ulong, unsigned long, |)
    IMPL_FETCH_OP_GENERIC(or, llong, long long, |)
    IMPL_FETCH_OP_GENERIC(or, ullong, unsigned long long, |)
    IMPL_FETCH_OP_GENERIC(or, size, size_t, |)
    
    IMPL_FETCH_OP_GENERIC(xor, int, int, ^)
    IMPL_FETCH_OP_GENERIC(xor, uint, unsigned int, ^)
    IMPL_FETCH_OP_GENERIC(xor, long, long, ^)
    IMPL_FETCH_OP_GENERIC(xor, ulong, unsigned long, ^)
    IMPL_FETCH_OP_GENERIC(xor, llong, long long, ^)
    IMPL_FETCH_OP_GENERIC(xor, ullong, unsigned long long, ^)
    IMPL_FETCH_OP_GENERIC(xor, size, size_t, ^)
    
    IMPL_FETCH_OP_GENERIC(and, int, int, &)
    IMPL_FETCH_OP_GENERIC(and, uint, unsigned int, &)
    IMPL_FETCH_OP_GENERIC(and, long, long, &)
    IMPL_FETCH_OP_GENERIC(and, ulong, unsigned long, &)
    IMPL_FETCH_OP_GENERIC(and, llong, long long, &)
    IMPL_FETCH_OP_GENERIC(and, ullong, unsigned long long, &)
    IMPL_FETCH_OP_GENERIC(and, size, size_t, &)
#endif

// Explicit memory order versions (condensed)
#if D_ATOMIC_HAS_STDATOMIC
    #define IMPL_FETCH_OP_EXPLICIT(OP_NAME, TYPE_SUFFIX, C_TYPE) \
        C_TYPE d_atomic_fetch_##OP_NAME##_##TYPE_SUFFIX##_explicit( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE _arg, \
            d_memory_order _order) \
        { \
            return atomic_fetch_##OP_NAME##_explicit(_obj, _arg, _order); \
        }
#else
    #define IMPL_FETCH_OP_EXPLICIT(OP_NAME, TYPE_SUFFIX, C_TYPE) \
        C_TYPE d_atomic_fetch_##OP_NAME##_##TYPE_SUFFIX##_explicit( \
            d_atomic_##TYPE_SUFFIX* _obj, \
            C_TYPE _arg, \
            d_memory_order _order) \
        { \
            (void)_order; \
            return d_atomic_fetch_##OP_NAME##_##TYPE_SUFFIX(_obj, _arg); \
        }
#endif

// Generate explicit versions for all fetch operations
IMPL_FETCH_OP_EXPLICIT(add, int, int)
IMPL_FETCH_OP_EXPLICIT(add, uint, unsigned int)
IMPL_FETCH_OP_EXPLICIT(add, long, long)
IMPL_FETCH_OP_EXPLICIT(add, ulong, unsigned long)
IMPL_FETCH_OP_EXPLICIT(add, llong, long long)
IMPL_FETCH_OP_EXPLICIT(add, ullong, unsigned long long)
IMPL_FETCH_OP_EXPLICIT(add, size, size_t)

IMPL_FETCH_OP_EXPLICIT(sub, int, int)
IMPL_FETCH_OP_EXPLICIT(sub, uint, unsigned int)
IMPL_FETCH_OP_EXPLICIT(sub, long, long)
IMPL_FETCH_OP_EXPLICIT(sub, ulong, unsigned long)
IMPL_FETCH_OP_EXPLICIT(sub, llong, long long)
IMPL_FETCH_OP_EXPLICIT(sub, ullong, unsigned long long)
IMPL_FETCH_OP_EXPLICIT(sub, size, size_t)

IMPL_FETCH_OP_EXPLICIT(or, int, int)
IMPL_FETCH_OP_EXPLICIT(or, uint, unsigned int)
IMPL_FETCH_OP_EXPLICIT(or, long, long)
IMPL_FETCH_OP_EXPLICIT(or, ulong, unsigned long)
IMPL_FETCH_OP_EXPLICIT(or, llong, long long)
IMPL_FETCH_OP_EXPLICIT(or, ullong, unsigned long long)
IMPL_FETCH_OP_EXPLICIT(or, size, size_t)

IMPL_FETCH_OP_EXPLICIT(xor, int, int)
IMPL_FETCH_OP_EXPLICIT(xor, uint, unsigned int)
IMPL_FETCH_OP_EXPLICIT(xor, long, long)
IMPL_FETCH_OP_EXPLICIT(xor, ulong, unsigned long)
IMPL_FETCH_OP_EXPLICIT(xor, llong, long long)
IMPL_FETCH_OP_EXPLICIT(xor, ullong, unsigned long long)
IMPL_FETCH_OP_EXPLICIT(xor, size, size_t)

IMPL_FETCH_OP_EXPLICIT(and, int, int)
IMPL_FETCH_OP_EXPLICIT(and, uint, unsigned int)
IMPL_FETCH_OP_EXPLICIT(and, long, long)
IMPL_FETCH_OP_EXPLICIT(and, ulong, unsigned long)
IMPL_FETCH_OP_EXPLICIT(and, llong, long long)
IMPL_FETCH_OP_EXPLICIT(and, ullong, unsigned long long)
IMPL_FETCH_OP_EXPLICIT(and, size, size_t)


///////////////////////////////////////////////////////////////////////////////
///             X.    MEMORY ORDERING AND FENCES                            ///
///////////////////////////////////////////////////////////////////////////////

/*
d_atomic_thread_fence
  Establishes memory synchronization ordering between threads.

Parameter(s):
  _order: the memory ordering constraint
Return:
  none
*/
void
d_atomic_thread_fence
(
    d_memory_order _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            std::atomic_thread_fence(_order);
        #else
            atomic_thread_fence(_order);
        #endif
    #elif defined(_WIN32) || defined(_WIN64)
        (void)_order;
        MemoryBarrier();
    #else
        (void)_order;
        // Generic fallback - compiler barrier
        __asm__ __volatile__("" ::: "memory");
    #endif
}

/*
d_atomic_signal_fence
  Establishes memory synchronization between a thread and a signal handler.

Parameter(s):
  _order: the memory ordering constraint
Return:
  none
*/
void
d_atomic_signal_fence
(
    d_memory_order _order
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        #ifdef __cplusplus
            std::atomic_signal_fence(_order);
        #else
            atomic_signal_fence(_order);
        #endif
    #else
        (void)_order;
        // Compiler barrier only (no hardware synchronization needed)
        #if defined(__GNUC__) || defined(__clang__)
            __asm__ __volatile__("" ::: "memory");
        #elif defined(_MSC_VER)
            _ReadWriteBarrier();
        #endif
    #endif
}

/*
d_atomic_is_lock_free_1
  Determines if 1-byte atomic operations are lock-free.

Parameter(s):
  none
Return:
  true if lock-free, false otherwise
*/
bool
d_atomic_is_lock_free_1
(
    void
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        return ATOMIC_CHAR_LOCK_FREE == 2;
    #elif defined(_WIN32) || defined(_WIN64)
        return true;  // Windows Interlocked operations are lock-free
    #else
        return false;  // Conservative assumption for generic fallback
    #endif
}

/*
d_atomic_is_lock_free_2
  Determines if 2-byte atomic operations are lock-free.

Parameter(s):
  none
Return:
  true if lock-free, false otherwise
*/
bool
d_atomic_is_lock_free_2
(
    void
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        return ATOMIC_SHORT_LOCK_FREE == 2;
    #elif defined(_WIN32) || defined(_WIN64)
        return true;
    #else
        return false;
    #endif
}

/*
d_atomic_is_lock_free_4
  Determines if 4-byte atomic operations are lock-free.

Parameter(s):
  none
Return:
  true if lock-free, false otherwise
*/
bool
d_atomic_is_lock_free_4
(
    void
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        return ATOMIC_INT_LOCK_FREE == 2;
    #elif defined(_WIN32) || defined(_WIN64)
        return true;
    #else
        return false;
    #endif
}

/*
d_atomic_is_lock_free_8
  Determines if 8-byte atomic operations are lock-free.

Parameter(s):
  none
Return:
  true if lock-free, false otherwise
*/
bool
d_atomic_is_lock_free_8
(
    void
)
{
    #if D_ATOMIC_HAS_STDATOMIC
        return ATOMIC_LLONG_LOCK_FREE == 2;
    #elif defined(_WIN32) || defined(_WIN64)
        // 64-bit atomics are lock-free on 64-bit Windows and Windows 8+
        #ifdef _WIN64
            return true;
        #else
            // Check if we're on Windows 8 or later (has native 64-bit atomics)
            return true;  // InterlockedCompareExchange64 is available
        #endif
    #else
        return false;
    #endif
}
