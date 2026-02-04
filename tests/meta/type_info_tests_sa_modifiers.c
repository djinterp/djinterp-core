#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_set_array
  Tests the D_TYPE_SET_ARRAY macro for setting the ARRAY flag.
  Tests the following:
  - Sets ARRAY bit correctly
  - Preserves existing flags
*/
bool
d_tests_sa_type_info_set_array
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;
    d_type_info16 modified;

    result = true;

    // test 1: set array on primitive type
    info     = D_TYPE_INFO_INT;
    modified = D_TYPE_SET_ARRAY(info);
    result   = d_assert_standalone(
        D_TYPE_IS_ARRAY(modified),
        "set_array_int",
        "ARRAY bit should be set on int",
        _counter) && result;

    // test 2: primitive flag should be preserved
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(modified),
        "set_array_preserves_prim",
        "PRIMITIVE flag should be preserved",
        _counter) && result;

    // test 3: original type ID should be preserved
    result = d_assert_standalone(
        D_TYPE_GET_PRIM(modified) == D_TYPE_PRIM_INT,
        "set_array_preserves_id",
        "Primitive ID should be preserved",
        _counter) && result;

    // test 4: set array on const type
    info     = D_TYPE_SET_CONST(D_TYPE_INFO_CHAR);
    modified = D_TYPE_SET_ARRAY(info);
    result   = d_assert_standalone(
        D_TYPE_IS_ARRAY(modified) && D_TYPE_IS_CONST(modified),
        "set_array_preserves_const",
        "ARRAY and CONST should both be set",
        _counter) && result;

    // test 5: set array on already-array type (idempotent)
    info     = D_TYPE_SET_ARRAY(D_TYPE_INFO_INT);
    modified = D_TYPE_SET_ARRAY(info);
    result   = d_assert_standalone(
        D_TYPE_IS_ARRAY(modified),
        "set_array_idempotent",
        "Setting ARRAY twice should still have ARRAY",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_set_typedef
  Tests the D_TYPE_SET_TYPEDEF macro for setting the TYPEDEF flag.
  Tests the following:
  - Sets TYPEDEF bit correctly
  - Preserves existing flags
*/
bool
d_tests_sa_type_info_set_typedef
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;
    d_type_info16 modified;

    result = true;

    // test 1: set typedef on primitive type
    info     = D_TYPE_INFO_INT;
    modified = D_TYPE_SET_TYPEDEF(info);
    result   = d_assert_standalone(
        D_TYPE_IS_TYPEDEF(modified),
        "set_typedef_int",
        "TYPEDEF bit should be set on int",
        _counter) && result;

    // test 2: primitive flag should be preserved
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(modified),
        "set_typedef_preserves_prim",
        "PRIMITIVE flag should be preserved",
        _counter) && result;

    // test 3: set typedef on struct
    info     = D_TYPE_STRUCT_();
    modified = D_TYPE_SET_TYPEDEF(info);
    result   = d_assert_standalone(
        D_TYPE_IS_TYPEDEF(modified) && D_TYPE_IS_STRUCT(modified),
        "set_typedef_struct",
        "TYPEDEF and STRUCT should both work",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_set_ext
  Tests the D_TYPE_SET_EXT macro for setting the extended flag.
  Tests the following:
  - Sets EXT bit correctly
  - Preserves existing flags
*/
bool
d_tests_sa_type_info_set_ext
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;
    d_type_info16 modified;

    result = true;

    // test 1: set ext on primitive type
    info     = D_TYPE_INFO_INT;
    modified = D_TYPE_SET_EXT(info);
    result   = d_assert_standalone(
        D_TYPE_HAS_EXT(modified),
        "set_ext_int",
        "EXT bit should be set",
        _counter) && result;

    // test 2: primitive flag should be preserved
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(modified),
        "set_ext_preserves_prim",
        "PRIMITIVE flag should be preserved",
        _counter) && result;

    // test 3: set ext on array type
    info     = D_TYPE_SET_ARRAY(D_TYPE_INFO_CHAR);
    modified = D_TYPE_SET_EXT(info);
    result   = d_assert_standalone(
        D_TYPE_HAS_EXT(modified) && D_TYPE_IS_ARRAY(modified),
        "set_ext_array",
        "EXT and ARRAY should both be set",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_set_custom
  Tests the D_TYPE_SET_CUSTOM and D_TYPE_SET_FRAMEWORK macros.
  Tests the following:
  - Sets CUSTOM bit correctly
  - D_TYPE_SET_FRAMEWORK is an alias
  - Preserves existing flags
*/
bool
d_tests_sa_type_info_set_custom
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;
    d_type_info16 modified;

    result = true;

    // test 1: set custom on primitive type
    info     = D_TYPE_INFO_INT;
    modified = D_TYPE_SET_CUSTOM(info);
    result   = d_assert_standalone(
        D_TYPE_IS_CUSTOM(modified),
        "set_custom_int",
        "CUSTOM bit should be set",
        _counter) && result;

    // test 2: D_TYPE_SET_FRAMEWORK should be same as D_TYPE_SET_CUSTOM
    info     = D_TYPE_INFO_CHAR;
    modified = D_TYPE_SET_FRAMEWORK(info);
    result   = d_assert_standalone(
        D_TYPE_IS_FRAMEWORK(modified),
        "set_framework_alias",
        "FRAMEWORK should be same as CUSTOM",
        _counter) && result;

    // test 3: D_TYPE_IS_FRAMEWORK is alias for D_TYPE_IS_CUSTOM
    result = d_assert_standalone(
        D_TYPE_IS_CUSTOM(modified) == D_TYPE_IS_FRAMEWORK(modified),
        "is_framework_alias",
        "IS_FRAMEWORK should equal IS_CUSTOM",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_set_const
  Tests the D_TYPE_SET_CONST macro for setting the const qualifier.
  Tests the following:
  - Sets CONST bit correctly
  - Preserves existing flags
*/
bool
d_tests_sa_type_info_set_const
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;
    d_type_info16 modified;

    result = true;

    // test 1: set const on primitive type
    info     = D_TYPE_INFO_INT;
    modified = D_TYPE_SET_CONST(info);
    result   = d_assert_standalone(
        D_TYPE_IS_CONST(modified),
        "set_const_int",
        "CONST bit should be set on int",
        _counter) && result;

    // test 2: primitive flag should be preserved
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(modified),
        "set_const_preserves_prim",
        "PRIMITIVE flag should be preserved",
        _counter) && result;

    // test 3: original type should be preserved
    result = d_assert_standalone(
        D_TYPE_GET_PRIM(modified) == D_TYPE_PRIM_INT,
        "set_const_preserves_id",
        "Primitive ID should be preserved",
        _counter) && result;

    // test 4: signed bit should be preserved
    result = d_assert_standalone(
        D_TYPE_IS_SIGNED(modified),
        "set_const_preserves_signed",
        "Signed bit should be preserved",
        _counter) && result;

    // test 5: set const twice (idempotent)
    modified = D_TYPE_SET_CONST(modified);
    result   = d_assert_standalone(
        D_TYPE_IS_CONST(modified),
        "set_const_idempotent",
        "Setting CONST twice should still have CONST",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_set_volatile
  Tests the D_TYPE_SET_VOLATILE macro for setting the volatile qualifier.
  Tests the following:
  - Sets VOLATILE bit correctly
  - Preserves existing flags
*/
bool
d_tests_sa_type_info_set_volatile
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;
    d_type_info16 modified;

    result = true;

    // test 1: set volatile on primitive type
    info     = D_TYPE_INFO_INT;
    modified = D_TYPE_SET_VOLATILE(info);
    result   = d_assert_standalone(
        D_TYPE_IS_VOLATILE(modified),
        "set_volatile_int",
        "VOLATILE bit should be set on int",
        _counter) && result;

    // test 2: primitive flag should be preserved
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(modified),
        "set_volatile_preserves_prim",
        "PRIMITIVE flag should be preserved",
        _counter) && result;

    // test 3: can combine with CONST (CV-qualified)
    info     = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
    modified = D_TYPE_SET_VOLATILE(info);
    result   = d_assert_standalone(
        D_TYPE_IS_CONST(modified) && D_TYPE_IS_VOLATILE(modified),
        "set_volatile_with_const",
        "CONST and VOLATILE should both be set",
        _counter) && result;

    // test 4: D_TYPE_IS_CV should detect CV-qualified
    result = d_assert_standalone(
        D_TYPE_IS_CV(modified),
        "is_cv_detection",
        "IS_CV should detect CV-qualified type",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_get_set_ptr_depth
  Tests D_TYPE_GET_PTR_DEPTH and D_TYPE_SET_PTR_DEPTH macros.
  Tests the following:
  - GET_PTR_DEPTH returns 0 for non-pointers
  - GET_PTR_DEPTH returns correct depth for pointers
  - SET_PTR_DEPTH correctly modifies depth
  - SET_PTR_DEPTH with 0 removes pointer
*/
bool
d_tests_sa_type_info_get_set_ptr_depth
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 info;

    result = true;

    // test 1: non-pointer has depth 0
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(D_TYPE_INFO_INT) == 0,
        "get_depth_non_ptr",
        "Non-pointer should have depth 0",
        _counter) && result;

    // test 2: single pointer has depth 1
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 1);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 1,
        "get_depth_single_ptr",
        "Single pointer should have depth 1",
        _counter) && result;

    // test 3: double pointer has depth 2
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 2);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 2,
        "get_depth_double_ptr",
        "Double pointer should have depth 2",
        _counter) && result;

    // test 4: set depth on existing pointer
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 1);
    info   = D_TYPE_SET_PTR_DEPTH(info, 5);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 5,
        "set_depth_modify",
        "SET_PTR_DEPTH should modify depth",
        _counter) && result;

    // test 5: set depth to 0 removes pointer flag
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 1);
    info   = D_TYPE_SET_PTR_DEPTH(info, 0);
    result = d_assert_standalone(
        !D_TYPE_IS_POINTER(info),
        "set_depth_zero_removes_ptr",
        "Setting depth 0 should remove POINTER flag",
        _counter) && result;

    // test 6: max depth (255)
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_VOID, 255);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 255,
        "get_depth_max",
        "Max depth should be 255",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_add_sub_ptr
  Tests D_TYPE_ADD_PTR and D_TYPE_SUB_PTR macros.
  Tests the following:
  - ADD_PTR increments pointer depth
  - ADD_PTR on non-pointer creates depth 1
  - SUB_PTR decrements pointer depth
  - SUB_PTR at depth 1 removes pointer
  - ADD_PTR respects maximum depth
*/
bool
d_tests_sa_type_info_add_sub_ptr
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 info;

    result = true;

    // test 1: ADD_PTR on non-pointer creates single pointer
    info   = D_TYPE_ADD_PTR(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(info) && D_TYPE_GET_PTR_DEPTH(info) == 1,
        "add_ptr_non_ptr",
        "ADD_PTR on non-pointer should create depth 1",
        _counter) && result;

    // test 2: ADD_PTR on single pointer creates double pointer
    info   = D_TYPE_ADD_PTR(info);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 2,
        "add_ptr_increment",
        "ADD_PTR should increment depth",
        _counter) && result;

    // test 3: SUB_PTR on double pointer creates single pointer
    info   = D_TYPE_SUB_PTR(info);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 1,
        "sub_ptr_decrement",
        "SUB_PTR should decrement depth",
        _counter) && result;

    // test 4: SUB_PTR on single pointer removes pointer
    info   = D_TYPE_SUB_PTR(info);
    result = d_assert_standalone(
        !D_TYPE_IS_POINTER(info),
        "sub_ptr_removes",
        "SUB_PTR at depth 1 should remove pointer",
        _counter) && result;

    // test 5: base type is preserved through ADD/SUB
    info   = D_TYPE_ADD_PTR(D_TYPE_INFO_DOUBLE);
    info   = D_TYPE_ADD_PTR(info);
    info   = D_TYPE_SUB_PTR(info);
    info   = D_TYPE_SUB_PTR(info);
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(info) && D_TYPE_GET_PRIM(info) == D_TYPE_PRIM_DOUBLE,
        "add_sub_preserves_base",
        "ADD/SUB should preserve base type",
        _counter) && result;

    // test 6: ADD_PTR at max depth stays at max
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, D_TYPE_PTR_DEPTH_MAX);
    info   = D_TYPE_ADD_PTR(info);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == D_TYPE_PTR_DEPTH_MAX,
        "add_ptr_max_capped",
        "ADD_PTR at max should stay at max",
        _counter) && result;

    // test 7: qualifiers preserved through pointer operations
    {
        d_type_info16 const_int;

        const_int = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
        info      = D_TYPE_ADD_PTR(const_int);

        result = d_assert_standalone(
            D_TYPE_IS_CONST(info),
            "add_ptr_preserves_const",
            "ADD_PTR should preserve CONST qualifier",
            _counter) && result;
    }

    return result;
}


/*
d_tests_sa_type_info_extended_modifiers
  Tests extended (32-bit) modifier macros.
  Tests the following:
  - D_TYPE_SET_RESTRICT
  - D_TYPE_SET_ATOMIC
  - D_TYPE_SET_STATIC
  - D_TYPE_SET_EXTERN
  - D_TYPE_SET_INLINE
  - D_TYPE_SET_NORETURN
*/
bool
d_tests_sa_type_info_extended_modifiers
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info_ex info;

    result = true;

    // test 1: set restrict
    info   = D_TYPE_SET_RESTRICT(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        (info & D_TYPE_RESTRICT_BIT) != 0,
        "set_restrict",
        "RESTRICT bit should be set",
        _counter) && result;

    // test 2: set atomic
    info   = D_TYPE_SET_ATOMIC(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        (info & D_TYPE_ATOMIC_BIT) != 0,
        "set_atomic",
        "ATOMIC bit should be set",
        _counter) && result;

    // test 3: set static
    info   = D_TYPE_SET_STATIC(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        (info & D_TYPE_STATIC_BIT) != 0,
        "set_static",
        "STATIC bit should be set",
        _counter) && result;

    // test 4: set extern
    info   = D_TYPE_SET_EXTERN(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        (info & D_TYPE_EXTERN_BIT) != 0,
        "set_extern",
        "EXTERN bit should be set",
        _counter) && result;

    // test 5: set inline
    info   = D_TYPE_SET_INLINE(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        (info & D_TYPE_INLINE_BIT) != 0,
        "set_inline",
        "INLINE bit should be set",
        _counter) && result;

    // test 6: set noreturn
    info   = D_TYPE_SET_NORETURN(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        (info & D_TYPE_NORETURN_BIT) != 0,
        "set_noreturn",
        "NORETURN bit should be set",
        _counter) && result;

    // test 7: combine multiple extended modifiers
    info = D_TYPE_SET_STATIC(D_TYPE_INFO_INT);
    info = D_TYPE_SET_INLINE(info);
    result = d_assert_standalone(
        ((info & D_TYPE_STATIC_BIT) != 0) && ((info & D_TYPE_INLINE_BIT) != 0),
        "combine_ext_modifiers",
        "Multiple extended modifiers should combine",
        _counter) && result;

    // test 8: base type preserved with extended modifiers
    info   = D_TYPE_SET_RESTRICT(D_TYPE_INFO_CHAR);
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(info) && D_TYPE_GET_PRIM(info) == D_TYPE_PRIM_CHAR,
        "ext_preserves_base",
        "Extended modifiers should preserve base type",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_modifiers_all
  Aggregation function that runs all modifier macro tests.
*/
bool
d_tests_sa_type_info_modifiers_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Modifier Macros\n");
    printf("  -------------------------\n");

    result = d_tests_sa_type_info_set_array(_counter) && result;
    result = d_tests_sa_type_info_set_typedef(_counter) && result;
    result = d_tests_sa_type_info_set_ext(_counter) && result;
    result = d_tests_sa_type_info_set_custom(_counter) && result;
    result = d_tests_sa_type_info_set_const(_counter) && result;
    result = d_tests_sa_type_info_set_volatile(_counter) && result;
    result = d_tests_sa_type_info_get_set_ptr_depth(_counter) && result;
    result = d_tests_sa_type_info_add_sub_ptr(_counter) && result;
    result = d_tests_sa_type_info_extended_modifiers(_counter) && result;

    return result;
}
