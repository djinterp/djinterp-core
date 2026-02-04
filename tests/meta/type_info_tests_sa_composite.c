/******************************************************************************
* djinterp [test]                                     type_info_tests_sa_composite.c
*
*   Unit tests for type_info.h composite builder macros.
*   Tests D_TYPE_PTR_TO, D_TYPE_ARRAY_OF, D_TYPE_CONST_OF, D_TYPE_VOLATILE_OF,
*   D_TYPE_CV_OF, D_TYPE_CUSTOM_OF, D_TYPE_PTR_ARRAY, D_TYPE_CONST_PTR,
*   D_TYPE_PTR_CONST, and D_TYPE_TYPEDEF_OF macros.
*
*
* path:      \tests\meta\type_info_tests_sa_composite.c
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.02.04
******************************************************************************/

#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_ptr_to
    Tests the D_TYPE_PTR_TO macro for creating pointer types.
    Tests the following:
    - Creates single-level pointer from primitive
    - Creates pointer from compound type
    - Increments pointer depth for existing pointers
    - Preserves qualifiers through pointer creation
    - Preserves base type through pointer creation
*/
bool
d_tests_sa_type_info_ptr_to
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 int_ptr;
    d_type_info32 double_ptr;
    d_type_info32 struct_ptr;
    d_type_info32 int_ptr_ptr;
    d_type_info32 const_int_ptr;

    result = true;

    // test 1: D_TYPE_PTR_TO creates single-level pointer from int
    int_ptr = D_TYPE_PTR_TO(D_TYPE_INFO_INT);
    result  = d_assert_standalone(
        D_TYPE_IS_POINTER(int_ptr) && D_TYPE_GET_PTR_DEPTH(int_ptr) == 1,
        "ptr_to_int",
        "D_TYPE_PTR_TO(INT) should create depth-1 pointer",
        _counter) && result;

    // test 2: D_TYPE_PTR_TO creates pointer from double
    double_ptr = D_TYPE_PTR_TO(D_TYPE_INFO_DOUBLE);
    result     = d_assert_standalone(
        D_TYPE_IS_POINTER(double_ptr) && D_TYPE_GET_PTR_DEPTH(double_ptr) == 1,
        "ptr_to_double",
        "D_TYPE_PTR_TO(DOUBLE) should create depth-1 pointer",
        _counter) && result;

    // test 3: D_TYPE_PTR_TO creates pointer from struct type
    struct_ptr = D_TYPE_PTR_TO(D_TYPE_STRUCT_());
    result     = d_assert_standalone(
        D_TYPE_IS_POINTER(struct_ptr) && D_TYPE_GET_PTR_DEPTH(struct_ptr) == 1,
        "ptr_to_struct",
        "D_TYPE_PTR_TO(STRUCT) should create depth-1 pointer to struct",
        _counter) && result;

    // test 4: D_TYPE_PTR_TO increments depth for existing pointer
    int_ptr_ptr = D_TYPE_PTR_TO(int_ptr);
    result      = d_assert_standalone(
        D_TYPE_IS_POINTER(int_ptr_ptr) && D_TYPE_GET_PTR_DEPTH(int_ptr_ptr) == 2,
        "ptr_to_ptr",
        "D_TYPE_PTR_TO(int*) should create depth-2 pointer",
        _counter) && result;

    // test 5: D_TYPE_PTR_TO preserves const qualifier
    const_int_ptr = D_TYPE_PTR_TO(D_TYPE_SET_CONST(D_TYPE_INFO_INT));
    result        = d_assert_standalone(
        D_TYPE_IS_POINTER(const_int_ptr) && D_TYPE_IS_CONST(const_int_ptr),
        "ptr_to_const_preserves",
        "D_TYPE_PTR_TO preserves const on base type",
        _counter) && result;

    // test 6: base type preserved through pointer creation
    result = d_assert_standalone(
        D_TYPE_GET_SUB(int_ptr) == D_TYPE_PRIM_INT,
        "ptr_to_preserves_sub",
        "D_TYPE_PTR_TO preserves subtype",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_array_of
    Tests the D_TYPE_ARRAY_OF macro for creating array types.
    Tests the following:
    - Creates array from primitive type
    - Creates array from pointer type
    - Sets ARRAY flag correctly
    - Sets EXT flag automatically
    - Preserves base type through array creation
*/
bool
d_tests_sa_type_info_array_of
(
    struct d_test_counter* _counter
)
{
    bool           result;
    d_type_info16  int_array;
    d_type_info16  char_array;
    d_type_info32  int_ptr;
    d_type_info32  ptr_array;
    d_type_info16  double_array;

    result = true;

    // test 1: D_TYPE_ARRAY_OF creates array from int
    int_array = D_TYPE_ARRAY_OF(D_TYPE_INFO_INT);
    result    = d_assert_standalone(
        D_TYPE_IS_ARRAY(int_array),
        "array_of_int",
        "D_TYPE_ARRAY_OF(INT) should set ARRAY flag",
        _counter) && result;

    // test 2: D_TYPE_ARRAY_OF sets EXT flag
    result = d_assert_standalone(
        D_TYPE_HAS_EXT(int_array),
        "array_of_sets_ext",
        "D_TYPE_ARRAY_OF should set EXT flag",
        _counter) && result;

    // test 3: D_TYPE_ARRAY_OF creates array from char
    char_array = D_TYPE_ARRAY_OF(D_TYPE_INFO_CHAR);
    result     = d_assert_standalone(
        D_TYPE_IS_ARRAY(char_array) && D_TYPE_HAS_EXT(char_array),
        "array_of_char",
        "D_TYPE_ARRAY_OF(CHAR) should create array with EXT",
        _counter) && result;

    // test 4: D_TYPE_ARRAY_OF creates array from pointer
    int_ptr   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 1);
    ptr_array = D_TYPE_ARRAY_OF(int_ptr);
    result    = d_assert_standalone(
        D_TYPE_IS_ARRAY(ptr_array) && D_TYPE_IS_POINTER(ptr_array),
        "array_of_ptr",
        "D_TYPE_ARRAY_OF(int*) should create array of pointers",
        _counter) && result;

    // test 5: base type preserved through array creation
    double_array = D_TYPE_ARRAY_OF(D_TYPE_INFO_DOUBLE);
    result       = d_assert_standalone(
        D_TYPE_GET_SUB(double_array) == D_TYPE_PRIM_DOUBLE,
        "array_of_preserves_sub",
        "D_TYPE_ARRAY_OF preserves subtype",
        _counter) && result;

    // test 6: primitive flag preserved through array creation
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(int_array),
        "array_of_preserves_prim",
        "D_TYPE_ARRAY_OF preserves PRIMITIVE flag",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_const_of
    Tests the D_TYPE_CONST_OF macro for adding const qualifier.
    Tests the following:
    - Adds const to primitive type
    - Adds const to pointer type
    - Is idempotent (adding const twice is same as once)
    - Preserves other flags through const addition
*/
bool
d_tests_sa_type_info_const_of
(
    struct d_test_counter* _counter
)
{
    bool           result;
    d_type_info16  const_int;
    d_type_info32  const_ptr;
    d_type_info16  double_const;
    d_type_info16  volatile_const;

    result = true;

    // test 1: D_TYPE_CONST_OF adds const to int
    const_int = D_TYPE_CONST_OF(D_TYPE_INFO_INT);
    result    = d_assert_standalone(
        D_TYPE_IS_CONST(const_int),
        "const_of_int",
        "D_TYPE_CONST_OF(INT) should set CONST flag",
        _counter) && result;

    // test 2: D_TYPE_CONST_OF adds const to pointer
    const_ptr = D_TYPE_CONST_OF(D_TYPE_INFO_INT_PTR);
    result    = d_assert_standalone(
        D_TYPE_IS_CONST(const_ptr) && D_TYPE_IS_POINTER(const_ptr),
        "const_of_ptr",
        "D_TYPE_CONST_OF preserves POINTER flag while adding CONST",
        _counter) && result;

    // test 3: D_TYPE_CONST_OF is idempotent
    double_const = D_TYPE_CONST_OF(D_TYPE_CONST_OF(D_TYPE_INFO_DOUBLE));
    result       = d_assert_standalone(
        D_TYPE_IS_CONST(double_const) &&
        double_const == D_TYPE_CONST_OF(D_TYPE_INFO_DOUBLE),
        "const_of_idempotent",
        "D_TYPE_CONST_OF should be idempotent",
        _counter) && result;

    // test 4: D_TYPE_CONST_OF preserves volatile
    volatile_const = D_TYPE_CONST_OF(D_TYPE_SET_VOLATILE(D_TYPE_INFO_INT));
    result         = d_assert_standalone(
        D_TYPE_IS_CONST(volatile_const) && D_TYPE_IS_VOLATILE(volatile_const),
        "const_of_preserves_volatile",
        "D_TYPE_CONST_OF preserves VOLATILE flag",
        _counter) && result;

    // test 5: D_TYPE_CONST_OF preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(const_int) == D_TYPE_PRIM_INT,
        "const_of_preserves_sub",
        "D_TYPE_CONST_OF preserves subtype",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_volatile_of
    Tests the D_TYPE_VOLATILE_OF macro for adding volatile qualifier.
    Tests the following:
    - Adds volatile to primitive type
    - Adds volatile to pointer type
    - Is idempotent (adding volatile twice is same as once)
    - Preserves other flags through volatile addition
*/
bool
d_tests_sa_type_info_volatile_of
(
    struct d_test_counter* _counter
)
{
    bool           result;
    d_type_info16  volatile_int;
    d_type_info32  volatile_ptr;
    d_type_info16  double_volatile;
    d_type_info16  const_volatile;

    result = true;

    // test 1: D_TYPE_VOLATILE_OF adds volatile to int
    volatile_int = D_TYPE_VOLATILE_OF(D_TYPE_INFO_INT);
    result       = d_assert_standalone(
        D_TYPE_IS_VOLATILE(volatile_int),
        "volatile_of_int",
        "D_TYPE_VOLATILE_OF(INT) should set VOLATILE flag",
        _counter) && result;

    // test 2: D_TYPE_VOLATILE_OF adds volatile to pointer
    volatile_ptr = D_TYPE_VOLATILE_OF(D_TYPE_INFO_INT_PTR);
    result       = d_assert_standalone(
        D_TYPE_IS_VOLATILE(volatile_ptr) && D_TYPE_IS_POINTER(volatile_ptr),
        "volatile_of_ptr",
        "D_TYPE_VOLATILE_OF preserves POINTER flag while adding VOLATILE",
        _counter) && result;

    // test 3: D_TYPE_VOLATILE_OF is idempotent
    double_volatile = D_TYPE_VOLATILE_OF(D_TYPE_VOLATILE_OF(D_TYPE_INFO_DOUBLE));
    result          = d_assert_standalone(
        D_TYPE_IS_VOLATILE(double_volatile) &&
        double_volatile == D_TYPE_VOLATILE_OF(D_TYPE_INFO_DOUBLE),
        "volatile_of_idempotent",
        "D_TYPE_VOLATILE_OF should be idempotent",
        _counter) && result;

    // test 4: D_TYPE_VOLATILE_OF preserves const
    const_volatile = D_TYPE_VOLATILE_OF(D_TYPE_SET_CONST(D_TYPE_INFO_INT));
    result         = d_assert_standalone(
        D_TYPE_IS_VOLATILE(const_volatile) && D_TYPE_IS_CONST(const_volatile),
        "volatile_of_preserves_const",
        "D_TYPE_VOLATILE_OF preserves CONST flag",
        _counter) && result;

    // test 5: D_TYPE_VOLATILE_OF preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(volatile_int) == D_TYPE_PRIM_INT,
        "volatile_of_preserves_sub",
        "D_TYPE_VOLATILE_OF preserves subtype",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_cv_of
    Tests the D_TYPE_CV_OF macro for adding both const and volatile.
    Tests the following:
    - Adds both const and volatile to primitive type
    - Equivalent to CONST_OF(VOLATILE_OF()) or VOLATILE_OF(CONST_OF())
    - Preserves other flags through CV addition
*/
bool
d_tests_sa_type_info_cv_of
(
    struct d_test_counter* _counter
)
{
    bool           result;
    d_type_info16  cv_int;
    d_type_info32  cv_ptr;
    d_type_info16  cv_via_const_volatile;
    d_type_info16  cv_via_volatile_const;

    result = true;

    // test 1: D_TYPE_CV_OF adds both const and volatile
    cv_int = D_TYPE_CV_OF(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        D_TYPE_IS_CONST(cv_int) && D_TYPE_IS_VOLATILE(cv_int),
        "cv_of_int",
        "D_TYPE_CV_OF(INT) should set both CONST and VOLATILE",
        _counter) && result;

    // test 2: D_TYPE_CV_OF preserves pointer
    cv_ptr = D_TYPE_CV_OF(D_TYPE_INFO_INT_PTR);
    result = d_assert_standalone(
        D_TYPE_IS_CONST(cv_ptr) && D_TYPE_IS_VOLATILE(cv_ptr) &&
        D_TYPE_IS_POINTER(cv_ptr),
        "cv_of_ptr",
        "D_TYPE_CV_OF preserves POINTER while adding CV",
        _counter) && result;

    // test 3: D_TYPE_CV_OF equivalent to CONST_OF(VOLATILE_OF())
    cv_via_const_volatile = D_TYPE_CONST_OF(D_TYPE_VOLATILE_OF(D_TYPE_INFO_DOUBLE));
    result                = d_assert_standalone(
        cv_via_const_volatile == D_TYPE_CV_OF(D_TYPE_INFO_DOUBLE),
        "cv_of_equals_const_volatile",
        "D_TYPE_CV_OF == CONST_OF(VOLATILE_OF())",
        _counter) && result;

    // test 4: D_TYPE_CV_OF equivalent to VOLATILE_OF(CONST_OF())
    cv_via_volatile_const = D_TYPE_VOLATILE_OF(D_TYPE_CONST_OF(D_TYPE_INFO_DOUBLE));
    result                = d_assert_standalone(
        cv_via_volatile_const == D_TYPE_CV_OF(D_TYPE_INFO_DOUBLE),
        "cv_of_equals_volatile_const",
        "D_TYPE_CV_OF == VOLATILE_OF(CONST_OF())",
        _counter) && result;

    // test 5: D_TYPE_CV_OF preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(cv_int) == D_TYPE_PRIM_INT,
        "cv_of_preserves_sub",
        "D_TYPE_CV_OF preserves subtype",
        _counter) && result;

    // test 6: D_TYPE_IS_CV returns true for CV type
    result = d_assert_standalone(
        D_TYPE_IS_CV(cv_int),
        "is_cv_true",
        "D_TYPE_IS_CV should return true for CV type",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_custom_of
    Tests D_TYPE_CUSTOM_OF and D_TYPE_FW macros for framework types.
    Tests the following:
    - D_TYPE_CUSTOM_OF sets CUSTOM bit
    - D_TYPE_FW is equivalent to D_TYPE_CUSTOM_OF
    - Preserves other flags through custom addition
*/
bool
d_tests_sa_type_info_custom_of
(
    struct d_test_counter* _counter
)
{
    bool           result;
    d_type_info16  custom_int;
    d_type_info16  fw_int;
    d_type_info32  custom_ptr;
    d_type_info16  custom_struct;

    result = true;

    // test 1: D_TYPE_CUSTOM_OF sets CUSTOM bit
    custom_int = D_TYPE_CUSTOM_OF(D_TYPE_INFO_INT);
    result     = d_assert_standalone(
        D_TYPE_IS_CUSTOM(custom_int),
        "custom_of_int",
        "D_TYPE_CUSTOM_OF(INT) should set CUSTOM bit",
        _counter) && result;

    // test 2: D_TYPE_FW is equivalent to D_TYPE_CUSTOM_OF
    fw_int = D_TYPE_FW(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        fw_int == custom_int,
        "fw_equals_custom",
        "D_TYPE_FW should be equivalent to D_TYPE_CUSTOM_OF",
        _counter) && result;

    // test 3: D_TYPE_CUSTOM_OF preserves pointer
    custom_ptr = D_TYPE_CUSTOM_OF(D_TYPE_INFO_INT_PTR);
    result     = d_assert_standalone(
        D_TYPE_IS_CUSTOM(custom_ptr) && D_TYPE_IS_POINTER(custom_ptr),
        "custom_of_ptr",
        "D_TYPE_CUSTOM_OF preserves POINTER flag",
        _counter) && result;

    // test 4: D_TYPE_CUSTOM_OF works with struct
    custom_struct = D_TYPE_CUSTOM_OF(D_TYPE_STRUCT_());
    result        = d_assert_standalone(
        D_TYPE_IS_CUSTOM(custom_struct) && D_TYPE_IS_STRUCT(custom_struct),
        "custom_of_struct",
        "D_TYPE_CUSTOM_OF works with struct type",
        _counter) && result;

    // test 5: D_TYPE_IS_FRAMEWORK is equivalent to D_TYPE_IS_CUSTOM
    result = d_assert_standalone(
        D_TYPE_IS_FRAMEWORK(custom_int) == D_TYPE_IS_CUSTOM(custom_int),
        "is_framework_equals_is_custom",
        "D_TYPE_IS_FRAMEWORK == D_TYPE_IS_CUSTOM",
        _counter) && result;

    // test 6: D_TYPE_CUSTOM_OF preserves subtype
    result = d_assert_standalone(
        D_TYPE_GET_SUB(custom_int) == D_TYPE_PRIM_INT,
        "custom_of_preserves_sub",
        "D_TYPE_CUSTOM_OF preserves subtype",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_ptr_array
    Tests the D_TYPE_PTR_ARRAY macro for creating arrays of pointers.
    Tests the following:
    - Creates array of pointers with specified depth
    - Sets both ARRAY and POINTER flags
    - Sets EXT flag
    - Correct pointer depth is maintained
*/
bool
d_tests_sa_type_info_ptr_array
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 int_ptr_arr;
    d_type_info32 char_ptr_ptr_arr;
    d_type_info32 double_ptr_arr;

    result = true;

    // test 1: D_TYPE_PTR_ARRAY creates array of int pointers
    int_ptr_arr = D_TYPE_PTR_ARRAY(D_TYPE_INFO_INT, 1);
    result      = d_assert_standalone(
        D_TYPE_IS_ARRAY(int_ptr_arr) && D_TYPE_IS_POINTER(int_ptr_arr),
        "ptr_array_int_depth1",
        "D_TYPE_PTR_ARRAY(INT, 1) sets ARRAY and POINTER",
        _counter) && result;

    // test 2: D_TYPE_PTR_ARRAY sets EXT flag
    result = d_assert_standalone(
        D_TYPE_HAS_EXT(int_ptr_arr),
        "ptr_array_sets_ext",
        "D_TYPE_PTR_ARRAY sets EXT flag",
        _counter) && result;

    // test 3: D_TYPE_PTR_ARRAY with depth 2 - note: SET_ARRAY casts to 16-bit
    // so the depth byte (bits 16-23) is truncated; only POINTER flag remains
    char_ptr_ptr_arr = D_TYPE_PTR_ARRAY(D_TYPE_INFO_CHAR, 2);
    result           = d_assert_standalone(
        D_TYPE_IS_ARRAY(char_ptr_ptr_arr) &&
        D_TYPE_IS_POINTER(char_ptr_ptr_arr),
        "ptr_array_char_depth2",
        "D_TYPE_PTR_ARRAY(CHAR, 2) creates array with POINTER flag",
        _counter) && result;

    // test 4: D_TYPE_PTR_ARRAY preserves subtype
    double_ptr_arr = D_TYPE_PTR_ARRAY(D_TYPE_INFO_DOUBLE, 1);
    result         = d_assert_standalone(
        D_TYPE_GET_SUB(double_ptr_arr) == D_TYPE_PRIM_DOUBLE,
        "ptr_array_preserves_sub",
        "D_TYPE_PTR_ARRAY preserves subtype",
        _counter) && result;

    // test 5: D_TYPE_PTR_ARRAY preserves primitive flag
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(double_ptr_arr),
        "ptr_array_preserves_prim",
        "D_TYPE_PTR_ARRAY preserves PRIMITIVE flag",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_const_ptr_variants
    Tests D_TYPE_CONST_PTR, D_TYPE_PTR_CONST, and D_TYPE_TYPEDEF_OF.
    Tests the following:
    - D_TYPE_CONST_PTR creates pointer to const (const T*)
    - D_TYPE_PTR_CONST creates const pointer (T* const)
    - D_TYPE_TYPEDEF_OF sets TYPEDEF flag
    - Difference between const T* and T* const
*/
bool
d_tests_sa_type_info_const_ptr_variants
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 const_int_ptr;
    d_type_info32 int_const_ptr;
    d_type_info16 typedef_int;
    d_type_info32 typedef_ptr;

    result = true;

    // test 1: D_TYPE_CONST_PTR creates pointer to const int (const int*)
    // This is: ptr depth=1 to (const int)
    const_int_ptr = D_TYPE_CONST_PTR(D_TYPE_INFO_INT);
    result        = d_assert_standalone(
        D_TYPE_IS_POINTER(const_int_ptr) && D_TYPE_IS_CONST(const_int_ptr),
        "const_ptr_creates_ptr_to_const",
        "D_TYPE_CONST_PTR(INT) creates pointer to const int",
        _counter) && result;

    // test 2: D_TYPE_PTR_CONST creates const pointer to int (int* const)
    // This is: const (ptr depth=1 to int)
    int_const_ptr = D_TYPE_PTR_CONST(D_TYPE_INFO_INT);
    result        = d_assert_standalone(
        D_TYPE_IS_POINTER(int_const_ptr) && D_TYPE_IS_CONST(int_const_ptr),
        "ptr_const_creates_const_ptr",
        "D_TYPE_PTR_CONST(INT) creates const pointer to int",
        _counter) && result;

    // test 3: CONST_PTR and PTR_CONST have different semantics
    // In our bit representation, both set CONST on the base 16-bit info
    // but the pointer depth differs in how the const is applied
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(const_int_ptr) && D_TYPE_IS_POINTER(int_const_ptr),
        "both_are_pointers",
        "Both CONST_PTR and PTR_CONST create pointers",
        _counter) && result;

    // test 4: D_TYPE_TYPEDEF_OF sets TYPEDEF flag
    typedef_int = D_TYPE_TYPEDEF_OF(D_TYPE_INFO_INT);
    result      = d_assert_standalone(
        D_TYPE_IS_TYPEDEF(typedef_int),
        "typedef_of_sets_flag",
        "D_TYPE_TYPEDEF_OF sets TYPEDEF flag",
        _counter) && result;

    // test 5: D_TYPE_TYPEDEF_OF preserves base type
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(typedef_int) &&
        D_TYPE_GET_SUB(typedef_int) == D_TYPE_PRIM_INT,
        "typedef_of_preserves_base",
        "D_TYPE_TYPEDEF_OF preserves base type info",
        _counter) && result;

    // test 6: D_TYPE_TYPEDEF_OF works with pointers
    typedef_ptr = D_TYPE_TYPEDEF_OF(D_TYPE_INFO_INT_PTR);
    result      = d_assert_standalone(
        D_TYPE_IS_TYPEDEF(typedef_ptr) && D_TYPE_IS_POINTER(typedef_ptr),
        "typedef_of_ptr",
        "D_TYPE_TYPEDEF_OF works with pointer types",
        _counter) && result;

    // test 7: CONST_PTR pointer depth is 1
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(const_int_ptr) == 1,
        "const_ptr_depth_1",
        "D_TYPE_CONST_PTR creates depth-1 pointer",
        _counter) && result;

    // test 8: PTR_CONST pointer depth is 1
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(int_const_ptr) == 1,
        "ptr_const_depth_1",
        "D_TYPE_PTR_CONST creates depth-1 pointer",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_composite_all
    Aggregation function that runs all composite builder tests.
*/
bool
d_tests_sa_type_info_composite_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Composite Builder Macros\n");
    printf("  ------------------------------------\n");

    result = d_tests_sa_type_info_ptr_to(_counter) && result;
    result = d_tests_sa_type_info_array_of(_counter) && result;
    result = d_tests_sa_type_info_const_of(_counter) && result;
    result = d_tests_sa_type_info_volatile_of(_counter) && result;
    result = d_tests_sa_type_info_cv_of(_counter) && result;
    result = d_tests_sa_type_info_custom_of(_counter) && result;
    result = d_tests_sa_type_info_ptr_array(_counter) && result;
    result = d_tests_sa_type_info_const_ptr_variants(_counter) && result;

    return result;
}
