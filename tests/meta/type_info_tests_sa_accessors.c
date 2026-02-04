#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_get_kind
  Tests the D_TYPE_GET_KIND macro for extracting kind flags.
  Tests the following:
  - Returns correct kind for primitives
  - Returns correct kind for pointers
  - Returns correct kind for arrays
  - Returns correct kind for typedefs
  - Returns combined kinds
*/
bool
d_tests_sa_type_info_get_kind
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;
    d_type_info16 kind;

    result = true;

    // test 1: primitive kind
    info   = D_TYPE_INFO_INT;
    kind   = D_TYPE_GET_KIND(info);
    result = d_assert_standalone(
        kind == D_TYPE_PRIMITIVE,
        "get_kind_primitive",
        "int should have PRIMITIVE kind",
        _counter) && result;

    // test 2: compound (no primitive flag)
    info   = D_TYPE_STRUCT_();
    kind   = D_TYPE_GET_KIND(info);
    result = d_assert_standalone(
        kind == 0,
        "get_kind_compound",
        "struct should have no kind flags",
        _counter) && result;

    // test 3: pointer kind
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 1);
    kind   = D_TYPE_GET_KIND(info);
    result = d_assert_standalone(
        (kind & D_TYPE_POINTER) != 0,
        "get_kind_pointer",
        "int* should have POINTER kind",
        _counter) && result;

    // test 4: primitive pointer (combined)
    result = d_assert_standalone(
        (kind & D_TYPE_PRIMITIVE) != 0 && (kind & D_TYPE_POINTER) != 0,
        "get_kind_combined",
        "int* should have PRIMITIVE and POINTER",
        _counter) && result;

    // test 5: array kind
    info   = D_TYPE_SET_ARRAY(D_TYPE_INFO_INT);
    kind   = D_TYPE_GET_KIND(info);
    result = d_assert_standalone(
        (kind & D_TYPE_ARRAY) != 0,
        "get_kind_array",
        "int[] should have ARRAY kind",
        _counter) && result;

    // test 6: typedef kind
    info   = D_TYPE_SET_TYPEDEF(D_TYPE_INFO_INT);
    kind   = D_TYPE_GET_KIND(info);
    result = d_assert_standalone(
        (kind & D_TYPE_TYPEDEF) != 0,
        "get_kind_typedef",
        "typedef should have TYPEDEF kind",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_get_sub
  Tests D_TYPE_GET_SUB, D_TYPE_GET_PRIM, and D_TYPE_GET_COMPOUND macros.
  Tests the following:
  - Returns correct primitive ID for primitives
  - Returns correct compound ID for compounds
  - GET_PRIM and GET_COMPOUND are aliases
*/
bool
d_tests_sa_type_info_get_sub
(
    struct d_test_counter* _counter
)
{
    bool     result;
    unsigned sub;

    result = true;

    // test 1: get primitive ID for int
    sub    = D_TYPE_GET_SUB(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        sub == D_TYPE_PRIM_INT,
        "get_sub_int",
        "int should have subtype ID 7",
        _counter) && result;

    // test 2: GET_PRIM is same as GET_SUB
    result = d_assert_standalone(
        D_TYPE_GET_PRIM(D_TYPE_INFO_INT) == D_TYPE_GET_SUB(D_TYPE_INFO_INT),
        "get_prim_alias",
        "GET_PRIM should equal GET_SUB",
        _counter) && result;

    // test 3: get compound ID for struct
    sub    = D_TYPE_GET_COMPOUND(D_TYPE_STRUCT_());
    result = d_assert_standalone(
        sub == D_TYPE_STRUCT,
        "get_compound_struct",
        "struct should have compound ID 1",
        _counter) && result;

    // test 4: GET_COMPOUND is same as GET_SUB
    result = d_assert_standalone(
        D_TYPE_GET_COMPOUND(D_TYPE_ENUM_()) == D_TYPE_GET_SUB(D_TYPE_ENUM_()),
        "get_compound_alias",
        "GET_COMPOUND should equal GET_SUB",
        _counter) && result;

    // test 5: all primitive IDs
    result = d_assert_standalone(
        D_TYPE_GET_PRIM(D_TYPE_INFO_VOID) == D_TYPE_PRIM_VOID,
        "get_prim_void",
        "void should have correct ID",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PRIM(D_TYPE_INFO_DOUBLE) == D_TYPE_PRIM_DOUBLE,
        "get_prim_double",
        "double should have correct ID",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_is_primitive
  Tests the D_TYPE_IS_PRIMITIVE macro.
  Tests the following:
  - Returns true for all primitive types
  - Returns false for compound types
  - Preserved when combined with modifiers
*/
bool
d_tests_sa_type_info_is_primitive
(
    struct d_test_counter* _counter
)
{
    bool result;
    int  i;

    result = true;

    // test 1: all D_TYPE_INFO_* primitives are primitive
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(D_TYPE_INFO_VOID),
        "is_prim_void",
        "void should be primitive",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(D_TYPE_INFO_INT),
        "is_prim_int",
        "int should be primitive",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(D_TYPE_INFO_DOUBLE),
        "is_prim_double",
        "double should be primitive",
        _counter) && result;

    // test 2: all generated primitives are primitive
    for (i = 0; i < D_TYPE_PRIM_COUNT; i++)
    {
        d_type_info16 info;

        info = D_TYPE_MAKE_PRIM((unsigned)i);

        if (!D_TYPE_IS_PRIMITIVE(info))
        {
            result = d_assert_standalone(
                false,
                "is_prim_all",
                "All MAKE_PRIM results should be primitive",
                _counter) && result;

            break;
        }
    }

    if (i == D_TYPE_PRIM_COUNT)
    {
        result = d_assert_standalone(
            true,
            "is_prim_all",
            "All MAKE_PRIM results should be primitive",
            _counter) && result;
    }

    // test 3: compounds are NOT primitive
    result = d_assert_standalone(
        !D_TYPE_IS_PRIMITIVE(D_TYPE_STRUCT_()),
        "is_prim_struct_false",
        "struct should not be primitive",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_PRIMITIVE(D_TYPE_ENUM_()),
        "is_prim_enum_false",
        "enum should not be primitive",
        _counter) && result;

    // test 4: primitive + modifiers still primitive
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(D_TYPE_SET_CONST(D_TYPE_INFO_INT)),
        "is_prim_const_int",
        "const int should be primitive",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(D_TYPE_SET_ARRAY(D_TYPE_INFO_INT)),
        "is_prim_array_int",
        "int[] should be primitive",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_is_pointer
  Tests the D_TYPE_IS_POINTER macro.
  Tests the following:
  - Returns true for pointer types
  - Returns false for non-pointer types
  - Works with various pointer depths
*/
bool
d_tests_sa_type_info_is_pointer
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: non-pointer is not pointer
    result = d_assert_standalone(
        !D_TYPE_IS_POINTER(D_TYPE_INFO_INT),
        "is_ptr_int_false",
        "int should not be pointer",
        _counter) && result;

    // test 2: single pointer is pointer
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 1)),
        "is_ptr_int_ptr",
        "int* should be pointer",
        _counter) && result;

    // test 3: double pointer is pointer
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 2)),
        "is_ptr_double",
        "int** should be pointer",
        _counter) && result;

    // test 4: predefined pointer types
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_VOID_PTR),
        "is_ptr_void_ptr",
        "void* should be pointer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_POINTER(D_TYPE_INFO_INT_PTR),
        "is_ptr_int_ptr_predef",
        "D_TYPE_INFO_INT_PTR should be pointer",
        _counter) && result;

    // test 5: struct is not pointer
    result = d_assert_standalone(
        !D_TYPE_IS_POINTER(D_TYPE_STRUCT_()),
        "is_ptr_struct_false",
        "struct should not be pointer",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_is_array
  Tests the D_TYPE_IS_ARRAY macro.
  Tests the following:
  - Returns true for array types
  - Returns false for non-array types
*/
bool
d_tests_sa_type_info_is_array
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: non-array is not array
    result = d_assert_standalone(
        !D_TYPE_IS_ARRAY(D_TYPE_INFO_INT),
        "is_array_int_false",
        "int should not be array",
        _counter) && result;

    // test 2: SET_ARRAY creates array
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_SET_ARRAY(D_TYPE_INFO_INT)),
        "is_array_set",
        "SET_ARRAY should create array",
        _counter) && result;

    // test 3: predefined array types
    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_INT_ARRAY),
        "is_array_int_array",
        "D_TYPE_INFO_INT_ARRAY should be array",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_ARRAY(D_TYPE_INFO_CHAR_ARRAY),
        "is_array_char_array",
        "D_TYPE_INFO_CHAR_ARRAY should be array",
        _counter) && result;

    // test 4: pointer is not array
    result = d_assert_standalone(
        !D_TYPE_IS_ARRAY(D_TYPE_INFO_INT_PTR),
        "is_array_ptr_false",
        "int* should not be array",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_is_typedef
  Tests the D_TYPE_IS_TYPEDEF macro.
  Tests the following:
  - Returns true for typedef types
  - Returns false for non-typedef types
*/
bool
d_tests_sa_type_info_is_typedef
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: non-typedef is not typedef
    result = d_assert_standalone(
        !D_TYPE_IS_TYPEDEF(D_TYPE_INFO_INT),
        "is_typedef_int_false",
        "int should not be typedef",
        _counter) && result;

    // test 2: SET_TYPEDEF creates typedef
    result = d_assert_standalone(
        D_TYPE_IS_TYPEDEF(D_TYPE_SET_TYPEDEF(D_TYPE_INFO_INT)),
        "is_typedef_set",
        "SET_TYPEDEF should create typedef",
        _counter) && result;

    // test 3: typedef + other modifiers
    result = d_assert_standalone(
        D_TYPE_IS_TYPEDEF(D_TYPE_SET_TYPEDEF(D_TYPE_SET_CONST(D_TYPE_INFO_INT))),
        "is_typedef_const",
        "const typedef should be typedef",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_qualifiers
  Tests D_TYPE_IS_CONST, D_TYPE_IS_VOLATILE, and D_TYPE_IS_CV macros.
  Tests the following:
  - Correctly detects const qualifier
  - Correctly detects volatile qualifier
  - IS_CV detects either or both
*/
bool
d_tests_sa_type_info_qualifiers
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;

    result = true;

    // test 1: unqualified type has no qualifiers
    result = d_assert_standalone(
        !D_TYPE_IS_CONST(D_TYPE_INFO_INT),
        "qual_int_no_const",
        "int should not be const",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_VOLATILE(D_TYPE_INFO_INT),
        "qual_int_no_volatile",
        "int should not be volatile",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_CV(D_TYPE_INFO_INT),
        "qual_int_no_cv",
        "int should not be CV-qualified",
        _counter) && result;

    // test 2: const qualified
    info   = D_TYPE_SET_CONST(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        D_TYPE_IS_CONST(info),
        "qual_const_detected",
        "const int should be const",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CV(info),
        "qual_const_is_cv",
        "const int should be CV-qualified",
        _counter) && result;

    // test 3: volatile qualified
    info   = D_TYPE_SET_VOLATILE(D_TYPE_INFO_INT);
    result = d_assert_standalone(
        D_TYPE_IS_VOLATILE(info),
        "qual_volatile_detected",
        "volatile int should be volatile",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CV(info),
        "qual_volatile_is_cv",
        "volatile int should be CV-qualified",
        _counter) && result;

    // test 4: const volatile (both)
    info   = D_TYPE_SET_VOLATILE(D_TYPE_SET_CONST(D_TYPE_INFO_INT));
    result = d_assert_standalone(
        D_TYPE_IS_CONST(info) && D_TYPE_IS_VOLATILE(info),
        "qual_both",
        "const volatile int should have both",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CV(info),
        "qual_cv_both",
        "const volatile should be CV-qualified",
        _counter) && result;

    // test 5: predefined const char
    result = d_assert_standalone(
        D_TYPE_IS_CONST(D_TYPE_INFO_CONST_CHAR),
        "qual_const_char",
        "D_TYPE_INFO_CONST_CHAR should be const",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_specific_primitives
  Tests specific primitive type checks (IS_VOID, IS_BOOL, IS_CHAR, etc.).
  Tests the following:
  - Each IS_* macro correctly identifies its type
  - Each IS_* macro returns false for other types
*/
bool
d_tests_sa_type_info_specific_primitives
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: IS_VOID
    result = d_assert_standalone(
        D_TYPE_IS_VOID(D_TYPE_INFO_VOID),
        "is_void_true",
        "void should be void",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_VOID(D_TYPE_INFO_INT),
        "is_void_false",
        "int should not be void",
        _counter) && result;

    // test 2: IS_BOOL
    result = d_assert_standalone(
        D_TYPE_IS_BOOL(D_TYPE_INFO_BOOL),
        "is_bool_true",
        "bool should be bool",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_BOOL(D_TYPE_INFO_INT),
        "is_bool_false",
        "int should not be bool",
        _counter) && result;

    // test 3: IS_CHAR (any char type)
    result = d_assert_standalone(
        D_TYPE_IS_CHAR(D_TYPE_INFO_CHAR),
        "is_char_plain",
        "char should be char",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CHAR(D_TYPE_INFO_SCHAR),
        "is_char_schar",
        "signed char should be char",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_CHAR(D_TYPE_INFO_UCHAR),
        "is_char_uchar",
        "unsigned char should be char",
        _counter) && result;

    // test 4: IS_CHAR_PLAIN (only plain char)
    result = d_assert_standalone(
        D_TYPE_IS_CHAR_PLAIN(D_TYPE_INFO_CHAR),
        "is_char_plain_true",
        "char should be plain char",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_CHAR_PLAIN(D_TYPE_INFO_SCHAR),
        "is_char_plain_schar_false",
        "signed char should not be plain char",
        _counter) && result;

    // test 5: IS_INT
    result = d_assert_standalone(
        D_TYPE_IS_INT(D_TYPE_INFO_INT),
        "is_int_true",
        "int should be int",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_INT(D_TYPE_INFO_LONG),
        "is_int_long_false",
        "long should not be int",
        _counter) && result;

    // test 6: IS_FLOAT and IS_DOUBLE
    result = d_assert_standalone(
        D_TYPE_IS_FLOAT(D_TYPE_INFO_FLOAT),
        "is_float_true",
        "float should be float",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_DOUBLE(D_TYPE_INFO_DOUBLE),
        "is_double_true",
        "double should be double",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_LDOUBLE(D_TYPE_INFO_LDOUBLE),
        "is_ldouble_true",
        "long double should be ldouble",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_type_categories
  Tests type category macros (IS_INTEGER, IS_FLOATING, IS_SIGNED, etc.).
  Tests the following:
  - IS_INTEGER identifies integer types
  - IS_FLOATING identifies floating-point types
  - IS_SIGNED/IS_UNSIGNED work correctly
  - IS_INTEGRAL, IS_ARITHMETIC, IS_SCALAR work correctly
*/
bool
d_tests_sa_type_info_type_categories
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: IS_INTEGER
    result = d_assert_standalone(
        D_TYPE_IS_INTEGER(D_TYPE_INFO_INT),
        "is_integer_int",
        "int should be integer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_INTEGER(D_TYPE_INFO_SCHAR),
        "is_integer_schar",
        "signed char should be integer",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_INTEGER(D_TYPE_INFO_ULLONG),
        "is_integer_ullong",
        "unsigned long long should be integer",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_INTEGER(D_TYPE_INFO_FLOAT),
        "is_integer_float_false",
        "float should not be integer",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_INTEGER(D_TYPE_INFO_VOID),
        "is_integer_void_false",
        "void should not be integer",
        _counter) && result;

    // test 2: IS_FLOATING
    result = d_assert_standalone(
        D_TYPE_IS_FLOATING(D_TYPE_INFO_FLOAT),
        "is_floating_float",
        "float should be floating",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_FLOATING(D_TYPE_INFO_DOUBLE),
        "is_floating_double",
        "double should be floating",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_FLOATING(D_TYPE_INFO_LDOUBLE),
        "is_floating_ldouble",
        "long double should be floating",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_FLOATING(D_TYPE_INFO_INT),
        "is_floating_int_false",
        "int should not be floating",
        _counter) && result;

    // test 3: IS_SIGNED / IS_UNSIGNED
    result = d_assert_standalone(
        D_TYPE_IS_SIGNED(D_TYPE_INFO_INT),
        "is_signed_int",
        "int should be signed",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_UNSIGNED(D_TYPE_INFO_UINT),
        "is_unsigned_uint",
        "unsigned int should be unsigned",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_SIGNED(D_TYPE_INFO_UINT),
        "is_signed_uint_false",
        "unsigned int should not be signed",
        _counter) && result;

    // test 4: IS_INTEGRAL (bool + char + integer)
    result = d_assert_standalone(
        D_TYPE_IS_INTEGRAL(D_TYPE_INFO_BOOL),
        "is_integral_bool",
        "bool should be integral",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_INTEGRAL(D_TYPE_INFO_CHAR),
        "is_integral_char",
        "char should be integral",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_INTEGRAL(D_TYPE_INFO_INT),
        "is_integral_int",
        "int should be integral",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_INTEGRAL(D_TYPE_INFO_FLOAT),
        "is_integral_float_false",
        "float should not be integral",
        _counter) && result;

    // test 5: IS_ARITHMETIC (integral + floating)
    result = d_assert_standalone(
        D_TYPE_IS_ARITHMETIC(D_TYPE_INFO_INT),
        "is_arith_int",
        "int should be arithmetic",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_ARITHMETIC(D_TYPE_INFO_DOUBLE),
        "is_arith_double",
        "double should be arithmetic",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_ARITHMETIC(D_TYPE_INFO_VOID),
        "is_arith_void_false",
        "void should not be arithmetic",
        _counter) && result;

    // test 6: IS_SCALAR (arithmetic + pointer + enum)
    result = d_assert_standalone(
        D_TYPE_IS_SCALAR(D_TYPE_INFO_INT),
        "is_scalar_int",
        "int should be scalar",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_SCALAR(D_TYPE_INFO_INT_PTR),
        "is_scalar_ptr",
        "int* should be scalar",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_SCALAR(D_TYPE_ENUM_()),
        "is_scalar_enum",
        "enum should be scalar",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_compound_tests
  Tests compound type detection macros.
  Tests the following:
  - IS_COMPOUND detects all compound types
  - IS_STRUCT, IS_UNION, IS_ENUM, IS_FUNCTION work correctly
  - IS_AGGREGATE detects arrays and compound types
*/
bool
d_tests_sa_type_info_compound_tests
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: IS_COMPOUND
    result = d_assert_standalone(
        D_TYPE_IS_COMPOUND(D_TYPE_STRUCT_()),
        "is_compound_struct",
        "struct should be compound",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_COMPOUND(D_TYPE_UNION_()),
        "is_compound_union",
        "union should be compound",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_COMPOUND(D_TYPE_ENUM_()),
        "is_compound_enum",
        "enum should be compound",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_COMPOUND(D_TYPE_INFO_INT),
        "is_compound_int_false",
        "int should not be compound",
        _counter) && result;

    // test 2: IS_STRUCT
    result = d_assert_standalone(
        D_TYPE_IS_STRUCT(D_TYPE_STRUCT_()),
        "is_struct_true",
        "struct should be struct",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_STRUCT(D_TYPE_UNION_()),
        "is_struct_union_false",
        "union should not be struct",
        _counter) && result;

    // test 3: IS_UNION
    result = d_assert_standalone(
        D_TYPE_IS_UNION(D_TYPE_UNION_()),
        "is_union_true",
        "union should be union",
        _counter) && result;

    // test 4: IS_ENUM
    result = d_assert_standalone(
        D_TYPE_IS_ENUM(D_TYPE_ENUM_()),
        "is_enum_true",
        "enum should be enum",
        _counter) && result;

    // test 5: IS_FUNCTION
    result = d_assert_standalone(
        D_TYPE_IS_FUNCTION(D_TYPE_FUNCTION_()),
        "is_function_true",
        "function should be function",
        _counter) && result;

    // test 6: IS_AGGREGATE (array + struct + union)
    result = d_assert_standalone(
        D_TYPE_IS_AGGREGATE(D_TYPE_SET_ARRAY(D_TYPE_INFO_INT)),
        "is_aggregate_array",
        "int[] should be aggregate",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_AGGREGATE(D_TYPE_STRUCT_()),
        "is_aggregate_struct",
        "struct should be aggregate",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_AGGREGATE(D_TYPE_UNION_()),
        "is_aggregate_union",
        "union should be aggregate",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_AGGREGATE(D_TYPE_INFO_INT),
        "is_aggregate_int_false",
        "int should not be aggregate",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_get_size
  Tests the D_TYPE_GET_SIZE macro.
  Tests the following:
  - Returns correct size for primitives
  - Returns sizeof(void*) for pointers
  - Returns sizeof(void*) for compound types
*/
bool
d_tests_sa_type_info_get_size
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: void size is 0
    result = d_assert_standalone(
        D_TYPE_GET_SIZE(D_TYPE_INFO_VOID) == 0,
        "get_size_void",
        "void size should be 0",
        _counter) && result;

    // test 2: int size
    result = d_assert_standalone(
        D_TYPE_GET_SIZE(D_TYPE_INFO_INT) == sizeof(int),
        "get_size_int",
        "int size should match sizeof(int)",
        _counter) && result;

    // test 3: char size
    result = d_assert_standalone(
        D_TYPE_GET_SIZE(D_TYPE_INFO_CHAR) == sizeof(char),
        "get_size_char",
        "char size should match sizeof(char)",
        _counter) && result;

    // test 4: double size
    result = d_assert_standalone(
        D_TYPE_GET_SIZE(D_TYPE_INFO_DOUBLE) == sizeof(double),
        "get_size_double",
        "double size should match sizeof(double)",
        _counter) && result;

    // test 5: pointer size
    result = d_assert_standalone(
        D_TYPE_GET_SIZE(D_TYPE_INFO_INT_PTR) == sizeof(void*),
        "get_size_ptr",
        "int* size should match sizeof(void*)",
        _counter) && result;

    // test 6: struct size defaults to sizeof(void*)
    result = d_assert_standalone(
        D_TYPE_GET_SIZE(D_TYPE_STRUCT_()) == sizeof(void*),
        "get_size_struct",
        "struct size should default to sizeof(void*)",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_accessors_all
  Aggregation function that runs all accessor and test macro tests.
*/
bool
d_tests_sa_type_info_accessors_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Accessor and Test Macros\n");
    printf("  -----------------------------------\n");

    result = d_tests_sa_type_info_get_kind(_counter) && result;
    result = d_tests_sa_type_info_get_sub(_counter) && result;
    result = d_tests_sa_type_info_is_primitive(_counter) && result;
    result = d_tests_sa_type_info_is_pointer(_counter) && result;
    result = d_tests_sa_type_info_is_array(_counter) && result;
    result = d_tests_sa_type_info_is_typedef(_counter) && result;
    result = d_tests_sa_type_info_qualifiers(_counter) && result;
    result = d_tests_sa_type_info_specific_primitives(_counter) && result;
    result = d_tests_sa_type_info_type_categories(_counter) && result;
    result = d_tests_sa_type_info_compound_tests(_counter) && result;
    result = d_tests_sa_type_info_get_size(_counter) && result;

    return result;
}
