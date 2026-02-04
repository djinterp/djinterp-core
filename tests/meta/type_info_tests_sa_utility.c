/******************************************************************************
* djinterp [test]                                       type_info_tests_sa_utility.c
*
*   Unit tests for type_info.h utility macros.
*   Tests D_TYPE_STRIP_CV, D_TYPE_STRIP_PTR, D_TYPE_STRIP_ALL_PTR, D_TYPE_BASE,
*   D_TYPE_EQ, D_TYPE_EQ_BASE, D_TYPE_COMPAT, D_TYPE_TO_BASE, and D_TYPE_TO_PTR
*   macros.
*
*
* path:      \tests\meta\type_info_tests_sa_utility.c
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.02.04
******************************************************************************/

#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_strip_cv
    Tests the D_TYPE_STRIP_CV macro for removing const/volatile qualifiers.
    Tests the following:
    - Removes const from const type
    - Removes volatile from volatile type
    - Removes both const and volatile from CV type
    - Preserves other flags through CV removal
    - No-op on non-CV type
*/
bool
d_tests_sa_type_info_strip_cv
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 const_int;
    d_type_info16 stripped_const;
    d_type_info16 volatile_int;
    d_type_info16 stripped_volatile;
    d_type_info16 cv_int;
    d_type_info16 stripped_cv;
    d_type_info16 plain_int;
    d_type_info16 stripped_plain;
    d_type_info16 const_array;
    d_type_info16 stripped_const_array;

    result = true;

    // test 1: D_TYPE_STRIP_CV removes const
    const_int      = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
    stripped_const = D_TYPE_STRIP_CV(const_int);
    result         = d_assert_standalone(
        !D_TYPE_IS_CONST(stripped_const) && !D_TYPE_IS_VOLATILE(stripped_const),
        "strip_cv_removes_const",
        "D_TYPE_STRIP_CV removes CONST flag",
        _counter) && result;

    // test 2: D_TYPE_STRIP_CV removes volatile
    volatile_int      = D_TYPE_SET_VOLATILE(D_TYPE_INFO_INT);
    stripped_volatile = D_TYPE_STRIP_CV(volatile_int);
    result            = d_assert_standalone(
        !D_TYPE_IS_CONST(stripped_volatile) &&
        !D_TYPE_IS_VOLATILE(stripped_volatile),
        "strip_cv_removes_volatile",
        "D_TYPE_STRIP_CV removes VOLATILE flag",
        _counter) && result;

    // test 3: D_TYPE_STRIP_CV removes both const and volatile
    cv_int     = D_TYPE_CV_OF(D_TYPE_INFO_INT);
    stripped_cv = D_TYPE_STRIP_CV(cv_int);
    result      = d_assert_standalone(
        !D_TYPE_IS_CONST(stripped_cv) && !D_TYPE_IS_VOLATILE(stripped_cv),
        "strip_cv_removes_both",
        "D_TYPE_STRIP_CV removes both CONST and VOLATILE",
        _counter) && result;

    // test 4: D_TYPE_STRIP_CV is no-op on non-CV type
    plain_int      = D_TYPE_INFO_INT;
    stripped_plain = D_TYPE_STRIP_CV(plain_int);
    result         = d_assert_standalone(
        stripped_plain == plain_int,
        "strip_cv_noop_on_plain",
        "D_TYPE_STRIP_CV is no-op on non-CV type",
        _counter) && result;

    // test 5: D_TYPE_STRIP_CV preserves other flags (ARRAY)
    const_array         = D_TYPE_SET_CONST(D_TYPE_SET_ARRAY(D_TYPE_INFO_INT));
    stripped_const_array = D_TYPE_STRIP_CV(const_array);
    result               = d_assert_standalone(
        D_TYPE_IS_ARRAY(stripped_const_array) && !D_TYPE_IS_CONST(stripped_const_array),
        "strip_cv_preserves_array",
        "D_TYPE_STRIP_CV preserves ARRAY flag",
        _counter) && result;

    // test 6: D_TYPE_STRIP_CV preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(stripped_const) == D_TYPE_PRIM_INT,
        "strip_cv_preserves_sub",
        "D_TYPE_STRIP_CV preserves subtype",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_strip_ptr
    Tests the D_TYPE_STRIP_PTR macro for removing one level of pointer.
    Tests the following:
    - Removes one level from single pointer
    - Decrements depth from multi-level pointer
    - Returns base type when stripping single-level pointer
    - No-op on non-pointer type
*/
bool
d_tests_sa_type_info_strip_ptr
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 int_ptr;
    d_type_info32 stripped_ptr;
    d_type_info32 int_ptr_ptr;
    d_type_info32 stripped_ptr_ptr;
    d_type_info16 plain_int;
    d_type_info16 stripped_plain;
    d_type_info32 depth_3_ptr;
    d_type_info32 stripped_depth_3;

    result = true;

    // test 1: D_TYPE_STRIP_PTR removes single-level pointer
    int_ptr     = D_TYPE_INFO_INT_PTR;
    stripped_ptr = D_TYPE_STRIP_PTR(int_ptr);
    result       = d_assert_standalone(
        !D_TYPE_IS_POINTER(stripped_ptr),
        "strip_ptr_removes_single",
        "D_TYPE_STRIP_PTR removes single-level pointer",
        _counter) && result;

    // test 2: D_TYPE_STRIP_PTR decrements depth from depth-2 pointer
    int_ptr_ptr     = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 2);
    stripped_ptr_ptr = D_TYPE_STRIP_PTR(int_ptr_ptr);
    result           = d_assert_standalone(
        D_TYPE_IS_POINTER(stripped_ptr_ptr) &&
        D_TYPE_GET_PTR_DEPTH(stripped_ptr_ptr) == 1,
        "strip_ptr_decrements_depth",
        "D_TYPE_STRIP_PTR decrements pointer depth by 1",
        _counter) && result;

    // test 3: D_TYPE_STRIP_PTR on non-pointer returns same (minus POINTER flag)
    plain_int      = D_TYPE_INFO_INT;
    stripped_plain = D_TYPE_STRIP_PTR(plain_int);
    result         = d_assert_standalone(
        !D_TYPE_IS_POINTER(stripped_plain),
        "strip_ptr_noop_on_nonptr",
        "D_TYPE_STRIP_PTR on non-pointer removes POINTER flag",
        _counter) && result;

    // test 4: D_TYPE_STRIP_PTR from depth-3 gives depth-2
    depth_3_ptr     = D_TYPE_MAKE_PTR(D_TYPE_INFO_CHAR, 3);
    stripped_depth_3 = D_TYPE_STRIP_PTR(depth_3_ptr);
    result           = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(stripped_depth_3) == 2,
        "strip_ptr_depth_3_to_2",
        "D_TYPE_STRIP_PTR(depth-3) gives depth-2",
        _counter) && result;

    // test 5: D_TYPE_STRIP_PTR preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(stripped_ptr) == D_TYPE_PRIM_INT,
        "strip_ptr_preserves_sub",
        "D_TYPE_STRIP_PTR preserves subtype",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_strip_all_ptr
    Tests D_TYPE_STRIP_ALL_PTR macro for removing all pointer levels.
    Tests the following:
    - Removes all levels from multi-level pointer
    - Returns base type without POINTER flag
    - No-op on non-pointer type
*/
bool
d_tests_sa_type_info_strip_all_ptr
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 int_ptr;
    d_type_info16 stripped_all;
    d_type_info32 int_ptr_ptr_ptr;
    d_type_info16 stripped_all_3;
    d_type_info16 plain_int;
    d_type_info16 stripped_plain;

    result = true;

    // test 1: D_TYPE_STRIP_ALL_PTR removes all from single pointer
    int_ptr     = D_TYPE_INFO_INT_PTR;
    stripped_all = D_TYPE_STRIP_ALL_PTR(int_ptr);
    result       = d_assert_standalone(
        !D_TYPE_IS_POINTER(stripped_all),
        "strip_all_ptr_single",
        "D_TYPE_STRIP_ALL_PTR removes single-level pointer",
        _counter) && result;

    // test 2: D_TYPE_STRIP_ALL_PTR removes all from depth-3 pointer
    int_ptr_ptr_ptr = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 3);
    stripped_all_3  = D_TYPE_STRIP_ALL_PTR(int_ptr_ptr_ptr);
    result          = d_assert_standalone(
        !D_TYPE_IS_POINTER(stripped_all_3),
        "strip_all_ptr_depth3",
        "D_TYPE_STRIP_ALL_PTR removes depth-3 pointer",
        _counter) && result;

    // test 3: D_TYPE_STRIP_ALL_PTR on non-pointer returns base without POINTER
    plain_int      = D_TYPE_INFO_INT;
    stripped_plain = D_TYPE_STRIP_ALL_PTR(plain_int);
    result         = d_assert_standalone(
        !D_TYPE_IS_POINTER(stripped_plain),
        "strip_all_ptr_nonptr",
        "D_TYPE_STRIP_ALL_PTR on non-pointer is no-op",
        _counter) && result;

    // test 4: D_TYPE_STRIP_ALL_PTR preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(stripped_all) == D_TYPE_PRIM_INT,
        "strip_all_ptr_preserves_sub",
        "D_TYPE_STRIP_ALL_PTR preserves subtype",
        _counter) && result;

    // test 5: D_TYPE_STRIP_ALL_PTR preserves PRIMITIVE flag
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(stripped_all),
        "strip_all_ptr_preserves_prim",
        "D_TYPE_STRIP_ALL_PTR preserves PRIMITIVE flag",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_base
    Tests D_TYPE_BASE macro for extracting base type info.
    Tests the following:
    - Extracts PRIMITIVE, subtype, and SIGNED bits
    - Ignores qualifiers, pointers, arrays, etc.
    - Same base for type with and without qualifiers
*/
bool
d_tests_sa_type_info_base
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 int_base;
    d_type_info16 const_int_base;
    d_type_info16 volatile_int_base;
    d_type_info32 ptr_int_base;
    d_type_info16 array_int_base;
    d_type_info16 schar_base;
    d_type_info16 const_schar_base;

    result = true;

    // test 1: D_TYPE_BASE extracts base of plain int
    int_base = D_TYPE_BASE(D_TYPE_INFO_INT);
    result   = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(int_base) && D_TYPE_GET_SUB(int_base) == D_TYPE_PRIM_INT,
        "base_plain_int",
        "D_TYPE_BASE extracts primitive and subtype",
        _counter) && result;

    // test 2: D_TYPE_BASE of const int equals base of plain int
    const_int_base = D_TYPE_BASE(D_TYPE_SET_CONST(D_TYPE_INFO_INT));
    result         = d_assert_standalone(
        const_int_base == int_base,
        "base_const_equals_plain",
        "D_TYPE_BASE(const int) == D_TYPE_BASE(int)",
        _counter) && result;

    // test 3: D_TYPE_BASE of volatile int equals base of plain int
    volatile_int_base = D_TYPE_BASE(D_TYPE_SET_VOLATILE(D_TYPE_INFO_INT));
    result            = d_assert_standalone(
        volatile_int_base == int_base,
        "base_volatile_equals_plain",
        "D_TYPE_BASE(volatile int) == D_TYPE_BASE(int)",
        _counter) && result;

    // test 4: D_TYPE_BASE ignores pointer
    ptr_int_base = D_TYPE_BASE(D_TYPE_INFO_INT_PTR);
    result       = d_assert_standalone(
        (d_type_info16)ptr_int_base == int_base,
        "base_ptr_equals_plain",
        "D_TYPE_BASE(int*) == D_TYPE_BASE(int)",
        _counter) && result;

    // test 5: D_TYPE_BASE ignores array
    array_int_base = D_TYPE_BASE(D_TYPE_SET_ARRAY(D_TYPE_INFO_INT));
    result         = d_assert_standalone(
        array_int_base == int_base,
        "base_array_equals_plain",
        "D_TYPE_BASE(int[]) == D_TYPE_BASE(int)",
        _counter) && result;

    // test 6: D_TYPE_BASE preserves SIGNED bit for signed char
    schar_base       = D_TYPE_BASE(D_TYPE_INFO_SCHAR);
    const_schar_base = D_TYPE_BASE(D_TYPE_SET_CONST(D_TYPE_INFO_SCHAR));
    result           = d_assert_standalone(
        D_TYPE_IS_SIGNED(schar_base) && schar_base == const_schar_base,
        "base_preserves_signed",
        "D_TYPE_BASE preserves SIGNED bit",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_eq
    Tests D_TYPE_EQ macro for exact equality comparison.
    Tests the following:
    - Equal types return true
    - Different types return false
    - Qualifiers affect equality
    - Pointer depth affects equality
*/
bool
d_tests_sa_type_info_eq
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 int_a;
    d_type_info16 int_b;
    d_type_info16 const_int;
    d_type_info16 double_val;
    d_type_info32 int_ptr;
    d_type_info32 int_ptr_ptr;

    result = true;

    // test 1: D_TYPE_EQ returns true for same type
    int_a = D_TYPE_INFO_INT;
    int_b = D_TYPE_INFO_INT;
    result = d_assert_standalone(
        D_TYPE_EQ(int_a, int_b),
        "eq_same_type",
        "D_TYPE_EQ(int, int) should return true",
        _counter) && result;

    // test 2: D_TYPE_EQ returns false for different types
    double_val = D_TYPE_INFO_DOUBLE;
    result     = d_assert_standalone(
        !D_TYPE_EQ(int_a, double_val),
        "eq_different_types",
        "D_TYPE_EQ(int, double) should return false",
        _counter) && result;

    // test 3: D_TYPE_EQ considers qualifiers (const int != int)
    const_int = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
    result    = d_assert_standalone(
        !D_TYPE_EQ(int_a, const_int),
        "eq_considers_const",
        "D_TYPE_EQ(int, const int) should return false",
        _counter) && result;

    // test 4: D_TYPE_EQ considers pointer depth
    int_ptr     = D_TYPE_INFO_INT_PTR;
    int_ptr_ptr = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 2);
    result      = d_assert_standalone(
        !D_TYPE_EQ(int_ptr, int_ptr_ptr),
        "eq_considers_ptr_depth",
        "D_TYPE_EQ(int*, int**) should return false",
        _counter) && result;

    // test 5: D_TYPE_EQ is reflexive
    result = d_assert_standalone(
        D_TYPE_EQ(const_int, const_int),
        "eq_reflexive",
        "D_TYPE_EQ(x, x) should return true",
        _counter) && result;

    // test 6: D_TYPE_EQ is symmetric
    result = d_assert_standalone(
        D_TYPE_EQ(int_a, int_b) == D_TYPE_EQ(int_b, int_a),
        "eq_symmetric",
        "D_TYPE_EQ(a, b) == D_TYPE_EQ(b, a)",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_eq_base
    Tests D_TYPE_EQ_BASE macro for base 16-bit equality.
    Tests the following:
    - Equal base types return true
    - Ignores upper 16 bits (pointer depth byte)
    - Considers qualifiers in base 16 bits
*/
bool
d_tests_sa_type_info_eq_base
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 int_ptr;
    d_type_info32 int_ptr_ptr;
    d_type_info16 const_int;
    d_type_info32 const_int_ptr;

    result = true;

    // test 1: D_TYPE_EQ_BASE compares base 16 bits (ignoring depth byte)
    // Compare two pointers with different depths - base 16 bits should match
    int_ptr     = D_TYPE_INFO_INT_PTR;
    int_ptr_ptr = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 2);
    result      = d_assert_standalone(
        D_TYPE_EQ_BASE(int_ptr, int_ptr_ptr),
        "eq_base_ignores_depth",
        "D_TYPE_EQ_BASE ignores pointer depth byte",
        _counter) && result;

    // test 2: D_TYPE_EQ_BASE for same base with different depths
    int_ptr_ptr = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 2);
    result      = d_assert_standalone(
        D_TYPE_EQ_BASE(int_ptr, int_ptr_ptr),
        "eq_base_diff_depths",
        "D_TYPE_EQ_BASE(int*, int**) should be true (same base)",
        _counter) && result;

    // test 3: D_TYPE_EQ_BASE matches when base 16 bits are same (including const)
    // Compare two const pointers with different depths
    const_int     = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
    const_int_ptr = D_TYPE_MAKE_PTR(const_int, 1);
    result        = d_assert_standalone(
        D_TYPE_EQ_BASE(const_int_ptr, D_TYPE_MAKE_PTR(const_int, 3)),
        "eq_base_const",
        "D_TYPE_EQ_BASE matches const pointers with different depths",
        _counter) && result;

    // test 4: D_TYPE_EQ_BASE false for different base types
    result = d_assert_standalone(
        !D_TYPE_EQ_BASE(D_TYPE_INFO_INT, D_TYPE_INFO_DOUBLE),
        "eq_base_different",
        "D_TYPE_EQ_BASE(int, double) should be false",
        _counter) && result;

    // test 5: D_TYPE_EQ_BASE false when qualifiers differ in base
    result = d_assert_standalone(
        !D_TYPE_EQ_BASE(D_TYPE_INFO_INT, const_int),
        "eq_base_const_differs",
        "D_TYPE_EQ_BASE(int, const int) should be false",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_compat
    Tests D_TYPE_COMPAT macro for type compatibility comparison.
    Tests the following:
    - Compatible types (same primitive and signed) return true
    - Ignores qualifiers (const, volatile)
    - Ignores pointer flag
    - Different primitives are not compatible
*/
bool
d_tests_sa_type_info_compat
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 int_val;
    d_type_info16 const_int;
    d_type_info16 volatile_int;
    d_type_info32 int_ptr;
    d_type_info16 uint_val;
    d_type_info16 double_val;

    result = true;

    // test 1: D_TYPE_COMPAT returns true for int and const int
    int_val   = D_TYPE_INFO_INT;
    const_int = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
    result    = d_assert_standalone(
        D_TYPE_COMPAT(int_val, const_int),
        "compat_ignores_const",
        "D_TYPE_COMPAT(int, const int) should be true",
        _counter) && result;

    // test 2: D_TYPE_COMPAT returns true for int and volatile int
    volatile_int = D_TYPE_SET_VOLATILE(D_TYPE_INFO_INT);
    result       = d_assert_standalone(
        D_TYPE_COMPAT(int_val, volatile_int),
        "compat_ignores_volatile",
        "D_TYPE_COMPAT(int, volatile int) should be true",
        _counter) && result;

    // test 3: D_TYPE_COMPAT returns true for int and int*
    int_ptr = D_TYPE_INFO_INT_PTR;
    result  = d_assert_standalone(
        D_TYPE_COMPAT(int_val, int_ptr),
        "compat_ignores_ptr",
        "D_TYPE_COMPAT(int, int*) should be true (same base)",
        _counter) && result;

    // test 4: D_TYPE_COMPAT returns false for int and uint (different signed)
    uint_val = D_TYPE_INFO_UINT;
    result   = d_assert_standalone(
        !D_TYPE_COMPAT(int_val, uint_val),
        "compat_considers_signed",
        "D_TYPE_COMPAT(int, uint) should be false (signed differs)",
        _counter) && result;

    // test 5: D_TYPE_COMPAT returns false for int and double
    double_val = D_TYPE_INFO_DOUBLE;
    result     = d_assert_standalone(
        !D_TYPE_COMPAT(int_val, double_val),
        "compat_different_types",
        "D_TYPE_COMPAT(int, double) should be false",
        _counter) && result;

    // test 6: D_TYPE_COMPAT is reflexive
    result = d_assert_standalone(
        D_TYPE_COMPAT(int_val, int_val),
        "compat_reflexive",
        "D_TYPE_COMPAT(x, x) should be true",
        _counter) && result;

    // test 7: D_TYPE_COMPAT is symmetric
    result = d_assert_standalone(
        D_TYPE_COMPAT(int_val, const_int) == D_TYPE_COMPAT(const_int, int_val),
        "compat_symmetric",
        "D_TYPE_COMPAT(a, b) == D_TYPE_COMPAT(b, a)",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_to_base
    Tests D_TYPE_TO_BASE macro for converting to base 16-bit type.
    Tests the following:
    - Extracts lower 16 bits from 32-bit type
    - Preserves all flags in lower 16 bits
    - Discards pointer depth byte
*/
bool
d_tests_sa_type_info_to_base
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 int_ptr;
    d_type_info16 base;
    d_type_info32 int_ptr_ptr;
    d_type_info16 base_ptr_ptr;
    d_type_info32 const_int_ptr;
    d_type_info16 base_const;

    result = true;

    // test 1: D_TYPE_TO_BASE extracts base from int*
    int_ptr = D_TYPE_INFO_INT_PTR;
    base    = D_TYPE_TO_BASE(int_ptr);
    result  = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(base) && D_TYPE_IS_POINTER(base),
        "to_base_extracts_16bit",
        "D_TYPE_TO_BASE extracts lower 16 bits",
        _counter) && result;

    // test 2: D_TYPE_TO_BASE discards pointer depth
    int_ptr_ptr  = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 2);
    base_ptr_ptr = D_TYPE_TO_BASE(int_ptr_ptr);
    result       = d_assert_standalone(
        base_ptr_ptr == base,
        "to_base_discards_depth",
        "D_TYPE_TO_BASE discards pointer depth byte",
        _counter) && result;

    // test 3: D_TYPE_TO_BASE preserves const
    const_int_ptr = D_TYPE_SET_CONST(int_ptr);
    base_const    = D_TYPE_TO_BASE(const_int_ptr);
    result        = d_assert_standalone(
        D_TYPE_IS_CONST(base_const),
        "to_base_preserves_const",
        "D_TYPE_TO_BASE preserves CONST flag",
        _counter) && result;

    // test 4: D_TYPE_TO_BASE preserves POINTER flag
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(base),
        "to_base_preserves_ptr_flag",
        "D_TYPE_TO_BASE preserves POINTER flag in base",
        _counter) && result;

    // test 5: D_TYPE_TO_BASE preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(base) == D_TYPE_PRIM_INT,
        "to_base_preserves_sub",
        "D_TYPE_TO_BASE preserves subtype",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_to_ptr
    Tests D_TYPE_TO_PTR macro for converting to 32-bit type.
    Tests the following:
    - Converts 16-bit to 32-bit preserving all bits
    - No change for types already 32-bit
*/
bool
d_tests_sa_type_info_to_ptr
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 int_val;
    d_type_info32 int_32;
    d_type_info32 int_ptr;
    d_type_info32 int_ptr_via_to;
    d_type_info16 const_int;
    d_type_info32 const_int_32;

    result = true;

    // test 1: D_TYPE_TO_PTR converts 16-bit to 32-bit
    int_val = D_TYPE_INFO_INT;
    int_32  = D_TYPE_TO_PTR(int_val);
    result  = d_assert_standalone(
        int_32 == (d_type_info32)int_val,
        "to_ptr_converts_16_to_32",
        "D_TYPE_TO_PTR converts 16-bit to 32-bit",
        _counter) && result;

    // test 2: D_TYPE_TO_PTR preserves existing 32-bit value
    int_ptr        = D_TYPE_INFO_INT_PTR;
    int_ptr_via_to = D_TYPE_TO_PTR(int_ptr);
    result         = d_assert_standalone(
        int_ptr_via_to == int_ptr,
        "to_ptr_preserves_32",
        "D_TYPE_TO_PTR preserves 32-bit value",
        _counter) && result;

    // test 3: D_TYPE_TO_PTR preserves const
    const_int    = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
    const_int_32 = D_TYPE_TO_PTR(const_int);
    result       = d_assert_standalone(
        D_TYPE_IS_CONST(const_int_32),
        "to_ptr_preserves_const",
        "D_TYPE_TO_PTR preserves CONST flag",
        _counter) && result;

    // test 4: D_TYPE_TO_PTR preserves primitive flag
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(int_32),
        "to_ptr_preserves_prim",
        "D_TYPE_TO_PTR preserves PRIMITIVE flag",
        _counter) && result;

    // test 5: D_TYPE_TO_PTR preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(int_32) == D_TYPE_PRIM_INT,
        "to_ptr_preserves_sub",
        "D_TYPE_TO_PTR preserves subtype",
        _counter) && result;

    return result;
}

/*
d_tests_sa_type_info_utility_all
    Aggregation function that runs all utility macro tests.
*/
bool
d_tests_sa_type_info_utility_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Utility Macros\n");
    printf("  -------------------------\n");

    result = d_tests_sa_type_info_strip_cv(_counter) && result;
    result = d_tests_sa_type_info_strip_ptr(_counter) && result;
    result = d_tests_sa_type_info_strip_all_ptr(_counter) && result;
    result = d_tests_sa_type_info_base(_counter) && result;
    result = d_tests_sa_type_info_eq(_counter) && result;
    result = d_tests_sa_type_info_eq_base(_counter) && result;
    result = d_tests_sa_type_info_compat(_counter) && result;
    result = d_tests_sa_type_info_to_base(_counter) && result;
    result = d_tests_sa_type_info_to_ptr(_counter) && result;

    return result;
}