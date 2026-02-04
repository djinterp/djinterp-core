#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_predefined_primitives
  Tests the predefined primitive type constants.
  Tests the following:
  - All D_TYPE_INFO_* primitives are valid
  - Each has correct primitive ID
  - Each has correct signed/unsigned status
*/
bool
d_tests_sa_type_info_predefined_primitives
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: D_TYPE_INFO_VOID
    result = d_assert_standalone(
        D_TYPE_IS_VOID(D_TYPE_INFO_VOID),
        "predef_void",
        "D_TYPE_INFO_VOID should be void",
        _counter) && result;

    // test 2: D_TYPE_INFO_BOOL
    result = d_assert_standalone(
        D_TYPE_IS_BOOL(D_TYPE_INFO_BOOL),
        "predef_bool",
        "D_TYPE_INFO_BOOL should be bool",
        _counter) && result;

    // test 3: D_TYPE_INFO_CHAR
    result = d_assert_standalone(
        D_TYPE_IS_CHAR_PLAIN(D_TYPE_INFO_CHAR),
        "predef_char",
        "D_TYPE_INFO_CHAR should be plain char",
        _counter) && result;

    // test 4: D_TYPE_INFO_SCHAR
    result = d_assert_standalone(
        D_TYPE_IS_SCHAR(D_TYPE_INFO_SCHAR) && D_TYPE_IS_SIGNED(D_TYPE_INFO_SCHAR),
        "predef_schar",
        "D_TYPE_INFO_SCHAR should be signed char",
        _counter) && result;

    // test 5: D_TYPE_INFO_UCHAR
    result = d_assert_standalone(
        D_TYPE_IS_UCHAR(D_TYPE_INFO_UCHAR) && !D_TYPE_IS_SIGNED(D_TYPE_INFO_UCHAR),
        "predef_uchar",
        "D_TYPE_INFO_UCHAR should be unsigned char",
        _counter) && result;

    // test 6: D_TYPE_INFO_SHORT
    result = d_assert_standalone(
        D_TYPE_IS_SHORT(D_TYPE_INFO_SHORT) && D_TYPE_IS_SIGNED(D_TYPE_INFO_SHORT),
        "predef_short",
        "D_TYPE_INFO_SHORT should be signed short",
        _counter) && result;

    // test 7: D_TYPE_INFO_USHORT
    result = d_assert_standalone(
        D_TYPE_IS_USHORT(D_TYPE_INFO_USHORT) && !D_TYPE_IS_SIGNED(D_TYPE_INFO_USHORT),
        "predef_ushort",
        "D_TYPE_INFO_USHORT should be unsigned short",
        _counter) && result;

    // test 8: D_TYPE_INFO_INT
    result = d_assert_standalone(
        D_TYPE_IS_INT(D_TYPE_INFO_INT) && D_TYPE_IS_SIGNED(D_TYPE_INFO_INT),
        "predef_int",
        "D_TYPE_INFO_INT should be signed int",
        _counter) && result;

    // test 9: D_TYPE_INFO_UINT
    result = d_assert_standalone(
        D_TYPE_IS_UINT(D_TYPE_INFO_UINT) && !D_TYPE_IS_SIGNED(D_TYPE_INFO_UINT),
        "predef_uint",
        "D_TYPE_INFO_UINT should be unsigned int",
        _counter) && result;

    // test 10: D_TYPE_INFO_LONG
    result = d_assert_standalone(
        D_TYPE_IS_LONG(D_TYPE_INFO_LONG) && D_TYPE_IS_SIGNED(D_TYPE_INFO_LONG),
        "predef_long",
        "D_TYPE_INFO_LONG should be signed long",
        _counter) && result;

    // test 11: D_TYPE_INFO_ULONG
    result = d_assert_standalone(
        D_TYPE_IS_ULONG(D_TYPE_INFO_ULONG) && !D_TYPE_IS_SIGNED(D_TYPE_INFO_ULONG),
        "predef_ulong",
        "D_TYPE_INFO_ULONG should be unsigned long",
        _counter) && result;

    // test 12: D_TYPE_INFO_LLONG
    result = d_assert_standalone(
        D_TYPE_IS_LLONG(D_TYPE_INFO_LLONG) && D_TYPE_IS_SIGNED(D_TYPE_INFO_LLONG),
        "predef_llong",
        "D_TYPE_INFO_LLONG should be signed long long",
        _counter) && result;

    // test 13: D_TYPE_INFO_ULLONG
    result = d_assert_standalone(
        D_TYPE_IS_ULLONG(D_TYPE_INFO_ULLONG) && !D_TYPE_IS_SIGNED(D_TYPE_INFO_ULLONG),
        "predef_ullong",
        "D_TYPE_INFO_ULLONG should be unsigned long long",
        _counter) && result;

    // test 14: D_TYPE_INFO_FLOAT
    result = d_assert_standalone(
        D_TYPE_IS_FLOAT(D_TYPE_INFO_FLOAT) && D_TYPE_IS_SIGNED(D_TYPE_INFO_FLOAT),
        "predef_float",
        "D_TYPE_INFO_FLOAT should be signed float",
        _counter) && result;

    // test 15: D_TYPE_INFO_DOUBLE
    result = d_assert_standalone(
        D_TYPE_IS_DOUBLE(D_TYPE_INFO_DOUBLE) && D_TYPE_IS_SIGNED(D_TYPE_INFO_DOUBLE),
        "predef_double",
        "D_TYPE_INFO_DOUBLE should be signed double",
        _counter) && result;

    // test 16: D_TYPE_INFO_LDOUBLE
    result = d_assert_standalone(
        D_TYPE_IS_LDOUBLE(D_TYPE_INFO_LDOUBLE) && D_TYPE_IS_SIGNED(D_TYPE_INFO_LDOUBLE),
        "predef_ldouble",
        "D_TYPE_INFO_LDOUBLE should be signed long double",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_predefined_cv_variants
  Tests predefined CV-qualified type constants.
  Tests the following:
  - D_TYPE_INFO_CONST_CHAR is const-qualified
  - D_TYPE_INFO_CONST_STRING is const char*
*/
bool
d_tests_sa_type_info_predefined_cv_variants
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: D_TYPE_INFO_CONST_CHAR
    result = d_assert_standalone(
        D_TYPE_IS_CONST(D_TYPE_INFO_CONST_CHAR),
        "predef_const_char_const",
        "D_TYPE_INFO_CONST_CHAR should be const",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CHAR_PLAIN(D_TYPE_INFO_CONST_CHAR),
        "predef_const_char_char",
        "D_TYPE_INFO_CONST_CHAR should be char",
        _counter) && result;

    // test 2: D_TYPE_INFO_CONST_STRING (const char*)
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_CONST_STRING),
        "predef_const_string_ptr",
        "D_TYPE_INFO_CONST_STRING should be pointer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CONST(D_TYPE_INFO_CONST_STRING),
        "predef_const_string_const",
        "D_TYPE_INFO_CONST_STRING should be const",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(D_TYPE_INFO_CONST_STRING) == 1,
        "predef_const_string_depth",
        "D_TYPE_INFO_CONST_STRING should have depth 1",
        _counter) && result;

    // test 3: D_TYPE_INFO_CONST_STRING_ARRAY (const char**)
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_CONST_STRING_ARRAY),
        "predef_const_string_array_ptr",
        "D_TYPE_INFO_CONST_STRING_ARRAY should be pointer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(D_TYPE_INFO_CONST_STRING_ARRAY) == 2,
        "predef_const_string_array_depth",
        "D_TYPE_INFO_CONST_STRING_ARRAY should have depth 2",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_predefined_arrays
  Tests predefined array type constants.
  Tests the following:
  - Array types have ARRAY flag
  - Array types have EXT flag
  - Base type is preserved
*/
bool
d_tests_sa_type_info_predefined_arrays
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: D_TYPE_INFO_CHAR_ARRAY
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_CHAR_ARRAY),
        "predef_char_array_array",
        "D_TYPE_INFO_CHAR_ARRAY should be array",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_HAS_EXT(D_TYPE_INFO_CHAR_ARRAY),
        "predef_char_array_ext",
        "D_TYPE_INFO_CHAR_ARRAY should have EXT flag",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(D_TYPE_INFO_CHAR_ARRAY),
        "predef_char_array_prim",
        "D_TYPE_INFO_CHAR_ARRAY should be primitive",
        _counter) && result;

    // test 2: D_TYPE_INFO_INT_ARRAY
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_INT_ARRAY),
        "predef_int_array_array",
        "D_TYPE_INFO_INT_ARRAY should be array",
        _counter) && result;

    // test 3: D_TYPE_INFO_BOOL_ARRAY
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_BOOL_ARRAY),
        "predef_bool_array_array",
        "D_TYPE_INFO_BOOL_ARRAY should be array",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_predefined_pointers
  Tests predefined pointer type constants.
  Tests the following:
  - Pointer types have POINTER flag
  - Pointer types have correct depth
  - Base type is preserved
*/
bool
d_tests_sa_type_info_predefined_pointers
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: D_TYPE_INFO_VOID_PTR
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_VOID_PTR),
        "predef_void_ptr_ptr",
        "D_TYPE_INFO_VOID_PTR should be pointer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(D_TYPE_INFO_VOID_PTR) == 1,
        "predef_void_ptr_depth",
        "D_TYPE_INFO_VOID_PTR should have depth 1",
        _counter) && result;

    // test 2: D_TYPE_INFO_BOOL_PTR
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_BOOL_PTR),
        "predef_bool_ptr_ptr",
        "D_TYPE_INFO_BOOL_PTR should be pointer",
        _counter) && result;

    // test 3: D_TYPE_INFO_CHAR_PTR
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_CHAR_PTR),
        "predef_char_ptr_ptr",
        "D_TYPE_INFO_CHAR_PTR should be pointer",
        _counter) && result;

    // test 4: D_TYPE_INFO_INT_PTR
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_INT_PTR),
        "predef_int_ptr_ptr",
        "D_TYPE_INFO_INT_PTR should be pointer",
        _counter) && result;

    // test 5: pointer array types
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_CHAR_PTR_ARRAY),
        "predef_char_ptr_array_array",
        "D_TYPE_INFO_CHAR_PTR_ARRAY should be array",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_CHAR_PTR_ARRAY),
        "predef_char_ptr_array_ptr",
        "D_TYPE_INFO_CHAR_PTR_ARRAY should be pointer",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_predefined_fixed_width
  Tests predefined fixed-width integer type aliases.
  Tests the following:
  - INT8, INT16, INT32, INT64 are aliases for correct types
  - UINT8, UINT16, UINT32, UINT64 are aliases for correct types
*/
bool
d_tests_sa_type_info_predefined_fixed_width
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: INT8 is alias for SCHAR
    result = d_assert_standalone(
        D_TYPE_INFO_INT8 == D_TYPE_INFO_SCHAR,
        "predef_int8_alias",
        "D_TYPE_INFO_INT8 should equal D_TYPE_INFO_SCHAR",
        _counter) && result;

    // test 2: INT16 is alias for SHORT
    result = d_assert_standalone(
        D_TYPE_INFO_INT16 == D_TYPE_INFO_SHORT,
        "predef_int16_alias",
        "D_TYPE_INFO_INT16 should equal D_TYPE_INFO_SHORT",
        _counter) && result;

    // test 3: INT32 is alias for INT
    result = d_assert_standalone(
        D_TYPE_INFO_INT32 == D_TYPE_INFO_INT,
        "predef_int32_alias",
        "D_TYPE_INFO_INT32 should equal D_TYPE_INFO_INT",
        _counter) && result;

    // test 4: INT64 is alias for LLONG
    result = d_assert_standalone(
        D_TYPE_INFO_INT64 == D_TYPE_INFO_LLONG,
        "predef_int64_alias",
        "D_TYPE_INFO_INT64 should equal D_TYPE_INFO_LLONG",
        _counter) && result;

    // test 5: UINT8 is alias for UCHAR
    result = d_assert_standalone(
        D_TYPE_INFO_UINT8 == D_TYPE_INFO_UCHAR,
        "predef_uint8_alias",
        "D_TYPE_INFO_UINT8 should equal D_TYPE_INFO_UCHAR",
        _counter) && result;

    // test 6: UINT16 is alias for USHORT
    result = d_assert_standalone(
        D_TYPE_INFO_UINT16 == D_TYPE_INFO_USHORT,
        "predef_uint16_alias",
        "D_TYPE_INFO_UINT16 should equal D_TYPE_INFO_USHORT",
        _counter) && result;

    // test 7: UINT32 is alias for UINT
    result = d_assert_standalone(
        D_TYPE_INFO_UINT32 == D_TYPE_INFO_UINT,
        "predef_uint32_alias",
        "D_TYPE_INFO_UINT32 should equal D_TYPE_INFO_UINT",
        _counter) && result;

    // test 8: UINT64 is alias for ULLONG
    result = d_assert_standalone(
        D_TYPE_INFO_UINT64 == D_TYPE_INFO_ULLONG,
        "predef_uint64_alias",
        "D_TYPE_INFO_UINT64 should equal D_TYPE_INFO_ULLONG",
        _counter) && result;

    // test 9: fixed-width arrays
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_INT32_ARRAY),
        "predef_int32_array",
        "D_TYPE_INFO_INT32_ARRAY should be array",
        _counter) && result;

    // test 10: fixed-width pointers
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_INT32_PTR),
        "predef_int32_ptr",
        "D_TYPE_INFO_INT32_PTR should be pointer",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_predefined_strings
  Tests predefined string type constants.
  Tests the following:
  - D_TYPE_INFO_STRING is const char*
  - D_TYPE_INFO_STRING_ARRAY is const char**
*/
bool
d_tests_sa_type_info_predefined_strings
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: D_TYPE_INFO_STRING
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_STRING),
        "predef_string_ptr",
        "D_TYPE_INFO_STRING should be pointer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CONST(D_TYPE_INFO_STRING),
        "predef_string_const",
        "D_TYPE_INFO_STRING should be const",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(D_TYPE_INFO_STRING) == 1,
        "predef_string_depth",
        "D_TYPE_INFO_STRING should have depth 1",
        _counter) && result;

    // test 2: D_TYPE_INFO_STRING_ARRAY
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_STRING_ARRAY),
        "predef_string_array_ptr",
        "D_TYPE_INFO_STRING_ARRAY should be pointer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(D_TYPE_INFO_STRING_ARRAY) == 2,
        "predef_string_array_depth",
        "D_TYPE_INFO_STRING_ARRAY should have depth 2",
        _counter) && result;

    // test 3: D_TYPE_INFO_STRING_PTR_ARRAY
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_STRING_PTR_ARRAY),
        "predef_string_ptr_array_array",
        "D_TYPE_INFO_STRING_PTR_ARRAY should be array",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_predefined_size_t
  Tests predefined size_t type constants.
  Tests the following:
  - D_TYPE_INFO_SIZE_T is correctly defined based on platform
  - Related pointer and array types are correct
*/
bool
d_tests_sa_type_info_predefined_size_t
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: D_TYPE_INFO_SIZE_T is primitive
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(D_TYPE_INFO_SIZE_T),
        "predef_size_t_prim",
        "D_TYPE_INFO_SIZE_T should be primitive",
        _counter) && result;

    // test 2: D_TYPE_INFO_SIZE_T is unsigned
    result = d_assert_standalone(
        !D_TYPE_IS_SIGNED(D_TYPE_INFO_SIZE_T),
        "predef_size_t_unsigned",
        "D_TYPE_INFO_SIZE_T should be unsigned",
        _counter) && result;

    // test 3: D_TYPE_INFO_SIZE_T_PTR is pointer
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_SIZE_T_PTR),
        "predef_size_t_ptr",
        "D_TYPE_INFO_SIZE_T_PTR should be pointer",
        _counter) && result;

    // test 4: D_TYPE_INFO_SIZE_T_ARRAY is array
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_SIZE_T_ARRAY),
        "predef_size_t_array",
        "D_TYPE_INFO_SIZE_T_ARRAY should be array",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_static_constants
  Tests the static const type constants for _Generic.
  Tests the following:
  - D_TYPE_C_* constants match D_TYPE_MAKE_PRIM results
  - Compound constants match builders
*/
bool
d_tests_sa_type_info_static_constants
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: D_TYPE_C_VOID
    result = d_assert_standalone(
        D_TYPE_C_VOID == D_TYPE_MAKE_PRIM(D_TYPE_PRIM_VOID),
        "static_c_void",
        "D_TYPE_C_VOID should match MAKE_PRIM",
        _counter) && result;

    // test 2: D_TYPE_C_INT
    result = d_assert_standalone(
        D_TYPE_C_INT == D_TYPE_MAKE_PRIM(D_TYPE_PRIM_INT),
        "static_c_int",
        "D_TYPE_C_INT should match MAKE_PRIM",
        _counter) && result;

    // test 3: D_TYPE_C_DOUBLE
    result = d_assert_standalone(
        D_TYPE_C_DOUBLE == D_TYPE_MAKE_PRIM(D_TYPE_PRIM_DOUBLE),
        "static_c_double",
        "D_TYPE_C_DOUBLE should match MAKE_PRIM",
        _counter) && result;

    // test 4: D_TYPE_C_STRUCT
    result = d_assert_standalone(
        D_TYPE_C_STRUCT == D_TYPE_STRUCT_(),
        "static_c_struct",
        "D_TYPE_C_STRUCT should match D_TYPE_STRUCT_()",
        _counter) && result;

    // test 5: D_TYPE_C_UNION
    result = d_assert_standalone(
        D_TYPE_C_UNION == D_TYPE_UNION_(),
        "static_c_union",
        "D_TYPE_C_UNION should match D_TYPE_UNION_()",
        _counter) && result;

    // test 6: D_TYPE_C_ENUM
    result = d_assert_standalone(
        D_TYPE_C_ENUM == D_TYPE_ENUM_(),
        "static_c_enum",
        "D_TYPE_C_ENUM should match D_TYPE_ENUM_()",
        _counter) && result;

    // test 7: D_TYPE_C_FUNCTION
    result = d_assert_standalone(
        D_TYPE_C_FUNCTION == D_TYPE_FUNCTION_(),
        "static_c_function",
        "D_TYPE_C_FUNCTION should match D_TYPE_FUNCTION_()",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_predefined_all
  Aggregation function that runs all predefined constant tests.
*/
bool
d_tests_sa_type_info_predefined_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Predefined Constants\n");
    printf("  -------------------------------\n");

    result = d_tests_sa_type_info_predefined_primitives(_counter) && result;
    result = d_tests_sa_type_info_predefined_cv_variants(_counter) && result;
    result = d_tests_sa_type_info_predefined_arrays(_counter) && result;
    result = d_tests_sa_type_info_predefined_pointers(_counter) && result;
    result = d_tests_sa_type_info_predefined_fixed_width(_counter) && result;
    result = d_tests_sa_type_info_predefined_strings(_counter) && result;
    result = d_tests_sa_type_info_predefined_size_t(_counter) && result;
    result = d_tests_sa_type_info_static_constants(_counter) && result;

    return result;
}
