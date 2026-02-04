#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_make
  Tests the D_TYPE_MAKE macro for creating type descriptors.
  Tests the following:
  - Correctly combines kind flags and subtype
  - Kind is stored in bits 0-3
  - Subtype is stored in bits 4-7
  - Edge cases with maximum values
*/
bool
d_tests_sa_type_info_make
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;

    result = true;

    // test 1: make with PRIMITIVE kind and subtype 0
    info   = D_TYPE_MAKE(D_TYPE_PRIMITIVE, 0);
    result = d_assert_standalone(
        (info & D_TYPE_KIND_MASK) == D_TYPE_PRIMITIVE,
        "make_prim_kind",
        "Kind should be PRIMITIVE",
        _counter) && result;

    result = d_assert_standalone(
        ((info & D_TYPE_SUB_MASK) >> D_TYPE_SUB_SHIFT) == 0,
        "make_prim_sub_0",
        "Subtype should be 0",
        _counter) && result;

    // test 2: make with PRIMITIVE kind and subtype 7 (int)
    info   = D_TYPE_MAKE(D_TYPE_PRIMITIVE, 7);
    result = d_assert_standalone(
        (info & D_TYPE_KIND_MASK) == D_TYPE_PRIMITIVE,
        "make_prim7_kind",
        "Kind should be PRIMITIVE",
        _counter) && result;

    result = d_assert_standalone(
        ((info & D_TYPE_SUB_MASK) >> D_TYPE_SUB_SHIFT) == 7,
        "make_prim7_sub",
        "Subtype should be 7",
        _counter) && result;

    // test 3: make with POINTER kind
    info   = D_TYPE_MAKE(D_TYPE_POINTER, 0);
    result = d_assert_standalone(
        (info & D_TYPE_POINTER) != 0,
        "make_ptr_kind",
        "POINTER bit should be set",
        _counter) && result;

    // test 4: make with combined kinds (PRIMITIVE | POINTER)
    info   = D_TYPE_MAKE(D_TYPE_PRIMITIVE | D_TYPE_POINTER, 5);
    result = d_assert_standalone(
        (info & D_TYPE_PRIMITIVE) != 0 && (info & D_TYPE_POINTER) != 0,
        "make_combined_kinds",
        "Both PRIMITIVE and POINTER bits should be set",
        _counter) && result;

    // test 5: subtype maximum value (15)
    info   = D_TYPE_MAKE(D_TYPE_PRIMITIVE, 15);
    result = d_assert_standalone(
        ((info & D_TYPE_SUB_MASK) >> D_TYPE_SUB_SHIFT) == 15,
        "make_subtype_max",
        "Subtype should be 15 (maximum)",
        _counter) && result;

    // test 6: subtype is masked to 4 bits (value > 15 should be truncated)
    info   = D_TYPE_MAKE(D_TYPE_PRIMITIVE, 0x1F);
    result = d_assert_standalone(
        ((info & D_TYPE_SUB_MASK) >> D_TYPE_SUB_SHIFT) == 0x0F,
        "make_subtype_truncate",
        "Subtype should be masked to 4 bits",
        _counter) && result;

    // test 7: make with ARRAY kind
    info   = D_TYPE_MAKE(D_TYPE_ARRAY, 0);
    result = d_assert_standalone(
        (info & D_TYPE_ARRAY) != 0,
        "make_array_kind",
        "ARRAY bit should be set",
        _counter) && result;

    // test 8: make with TYPEDEF kind
    info   = D_TYPE_MAKE(D_TYPE_TYPEDEF, 0);
    result = d_assert_standalone(
        (info & D_TYPE_TYPEDEF) != 0,
        "make_typedef_kind",
        "TYPEDEF bit should be set",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_make_prim
  Tests the D_TYPE_MAKE_PRIM macro for creating primitive type descriptors.
  Tests the following:
  - Correctly sets PRIMITIVE kind flag
  - Correctly sets subtype to primitive ID
  - Correctly sets signed bit based on primitive type
*/
bool
d_tests_sa_type_info_make_prim
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;

    result = true;

    // test 1: make void primitive
    info   = D_TYPE_MAKE_PRIM(D_TYPE_PRIM_VOID);
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(info),
        "make_prim_void_flag",
        "void should have PRIMITIVE flag",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PRIM(info) == D_TYPE_PRIM_VOID,
        "make_prim_void_id",
        "void should have ID 0",
        _counter) && result;

    // test 2: make int primitive (signed)
    info   = D_TYPE_MAKE_PRIM(D_TYPE_PRIM_INT);
    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(info),
        "make_prim_int_flag",
        "int should have PRIMITIVE flag",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PRIM(info) == D_TYPE_PRIM_INT,
        "make_prim_int_id",
        "int should have correct ID",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_SIGNED(info),
        "make_prim_int_signed",
        "int should be signed",
        _counter) && result;

    // test 3: make unsigned int (not signed)
    info   = D_TYPE_MAKE_PRIM(D_TYPE_PRIM_UINT);
    result = d_assert_standalone(
        !D_TYPE_IS_SIGNED(info),
        "make_prim_uint_unsigned",
        "unsigned int should not be signed",
        _counter) && result;

    // test 4: make double (signed/floating)
    info   = D_TYPE_MAKE_PRIM(D_TYPE_PRIM_DOUBLE);
    result = d_assert_standalone(
        D_TYPE_IS_SIGNED(info),
        "make_prim_double_signed",
        "double should be signed",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PRIM(info) == D_TYPE_PRIM_DOUBLE,
        "make_prim_double_id",
        "double should have correct ID",
        _counter) && result;

    // test 5: make signed char
    info   = D_TYPE_MAKE_PRIM(D_TYPE_PRIM_SCHAR);
    result = d_assert_standalone(
        D_TYPE_IS_SIGNED(info),
        "make_prim_schar_signed",
        "signed char should be signed",
        _counter) && result;

    // test 6: make unsigned char
    info   = D_TYPE_MAKE_PRIM(D_TYPE_PRIM_UCHAR);
    result = d_assert_standalone(
        !D_TYPE_IS_SIGNED(info),
        "make_prim_uchar_unsigned",
        "unsigned char should not be signed",
        _counter) && result;

    // test 7: all primitives have PRIMITIVE flag set
    {
        int  i;
        bool all_have_flag;

        all_have_flag = true;

        for (i = 0; i < D_TYPE_PRIM_COUNT; i++)
        {
            info = D_TYPE_MAKE_PRIM((unsigned)i);

            if (!D_TYPE_IS_PRIMITIVE(info))
            {
                all_have_flag = false;
                break;
            }
        }

        result = d_assert_standalone(
            all_have_flag,
            "make_prim_all_have_flag",
            "All primitives should have PRIMITIVE flag",
            _counter) && result;
    }

    return result;
}


/*
d_tests_sa_type_info_make_compound
  Tests the D_TYPE_MAKE_COMPOUND macro for creating compound type descriptors.
  Tests the following:
  - Correctly sets subtype without PRIMITIVE flag
  - All compound types have correct subtypes
*/
bool
d_tests_sa_type_info_make_compound
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;

    result = true;

    // test 1: make struct compound
    info   = D_TYPE_MAKE_COMPOUND(D_TYPE_STRUCT);
    result = d_assert_standalone(
        !D_TYPE_IS_PRIMITIVE(info),
        "make_compound_struct_no_prim",
        "struct should not have PRIMITIVE flag",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_COMPOUND(info) == D_TYPE_STRUCT,
        "make_compound_struct_id",
        "struct should have correct compound ID",
        _counter) && result;

    // test 2: make union compound
    info   = D_TYPE_MAKE_COMPOUND(D_TYPE_UNION);
    result = d_assert_standalone(
        D_TYPE_GET_COMPOUND(info) == D_TYPE_UNION,
        "make_compound_union_id",
        "union should have correct compound ID",
        _counter) && result;

    // test 3: make enum compound
    info   = D_TYPE_MAKE_COMPOUND(D_TYPE_ENUM);
    result = d_assert_standalone(
        D_TYPE_GET_COMPOUND(info) == D_TYPE_ENUM,
        "make_compound_enum_id",
        "enum should have correct compound ID",
        _counter) && result;

    // test 4: make function compound
    info   = D_TYPE_MAKE_COMPOUND(D_TYPE_FUNCTION);
    result = d_assert_standalone(
        D_TYPE_GET_COMPOUND(info) == D_TYPE_FUNCTION,
        "make_compound_function_id",
        "function should have correct compound ID",
        _counter) && result;

    // test 5: make custom compound
    info   = D_TYPE_MAKE_COMPOUND(D_TYPE_CUSTOM);
    result = d_assert_standalone(
        D_TYPE_GET_COMPOUND(info) == D_TYPE_CUSTOM,
        "make_compound_custom_id",
        "custom should have correct compound ID",
        _counter) && result;

    // test 6: compound types have no kind flags set (only subtype)
    info   = D_TYPE_MAKE_COMPOUND(D_TYPE_STRUCT);
    result = d_assert_standalone(
        (info & D_TYPE_KIND_MASK) == 0,
        "make_compound_no_kind_flags",
        "Compound type should have no kind flags",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_make_ptr
  Tests the D_TYPE_MAKE_PTR macro for creating pointer type descriptors.
  Tests the following:
  - Correctly embeds base type in lower 16 bits
  - Correctly sets POINTER flag
  - Correctly sets pointer depth in bits 16-23
  - Edge cases with depth 0, 1, and max
*/
bool
d_tests_sa_type_info_make_ptr
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info32 info;

    result = true;

    // test 1: make pointer to int with depth 1
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 1);
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(info),
        "make_ptr_int_flag",
        "int* should have POINTER flag",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 1,
        "make_ptr_int_depth",
        "int* should have depth 1",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_IS_PRIMITIVE(info),
        "make_ptr_int_base_prim",
        "int* base should be primitive",
        _counter) && result;

    // test 2: make pointer with depth 2 (double pointer)
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_CHAR, 2);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 2,
        "make_ptr_char_depth2",
        "char** should have depth 2",
        _counter) && result;

    // test 3: make pointer with depth 255 (maximum)
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_VOID, 255);
    result = d_assert_standalone(
        D_TYPE_GET_PTR_DEPTH(info) == 255,
        "make_ptr_max_depth",
        "Pointer should support depth 255",
        _counter) && result;

    // test 4: base type is preserved in lower 16 bits
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_DOUBLE, 1);
    result = d_assert_standalone(
        D_TYPE_GET_BASE(info) == (D_TYPE_INFO_DOUBLE | D_TYPE_POINTER),
        "make_ptr_base_preserved",
        "Base type should be preserved",
        _counter) && result;

    // test 5: make pointer to const char
    {
        d_type_info16 const_char;

        const_char = D_TYPE_SET_CONST(D_TYPE_INFO_CHAR);
        info       = D_TYPE_MAKE_PTR(const_char, 1);

        result = d_assert_standalone(
            D_TYPE_IS_CONST(info),
            "make_ptr_const_preserved",
            "const qualifier should be preserved in pointer",
            _counter) && result;
    }

    // test 6: depth 0 handling (no pointer flag in depth byte)
    info   = D_TYPE_MAKE_PTR(D_TYPE_INFO_INT, 0);
    result = d_assert_standalone(
        D_TYPE_IS_POINTER(info),
        "make_ptr_depth0_has_flag",
        "Depth 0 still sets POINTER flag",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_primitive_builders
  Tests the D_TYPE_*_() primitive builder macros.
  Tests the following:
  - Each builder creates correct primitive type
  - Signed types have signed bit set
  - Unsigned types do not have signed bit set
*/
bool
d_tests_sa_type_info_primitive_builders
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;

    result = true;

    // test 1: D_TYPE_VOID_()
    info   = D_TYPE_VOID_();
    result = d_assert_standalone(
        D_TYPE_IS_VOID(info),
        "builder_void",
        "D_TYPE_VOID_() should create void",
        _counter) && result;

    // test 2: D_TYPE_BOOL_()
    info   = D_TYPE_BOOL_();
    result = d_assert_standalone(
        D_TYPE_IS_BOOL(info),
        "builder_bool",
        "D_TYPE_BOOL_() should create bool",
        _counter) && result;

    // test 3: D_TYPE_CHAR_()
    info   = D_TYPE_CHAR_();
    result = d_assert_standalone(
        D_TYPE_IS_CHAR_PLAIN(info),
        "builder_char",
        "D_TYPE_CHAR_() should create plain char",
        _counter) && result;

    // test 4: D_TYPE_SCHAR_()
    info   = D_TYPE_SCHAR_();
    result = d_assert_standalone(
        D_TYPE_IS_SCHAR(info) && D_TYPE_IS_SIGNED(info),
        "builder_schar",
        "D_TYPE_SCHAR_() should create signed char",
        _counter) && result;

    // test 5: D_TYPE_UCHAR_()
    info   = D_TYPE_UCHAR_();
    result = d_assert_standalone(
        D_TYPE_IS_UCHAR(info) && !D_TYPE_IS_SIGNED(info),
        "builder_uchar",
        "D_TYPE_UCHAR_() should create unsigned char",
        _counter) && result;

    // test 6: D_TYPE_SHORT_()
    info   = D_TYPE_SHORT_();
    result = d_assert_standalone(
        D_TYPE_IS_SHORT(info) && D_TYPE_IS_SIGNED(info),
        "builder_short",
        "D_TYPE_SHORT_() should create signed short",
        _counter) && result;

    // test 7: D_TYPE_USHORT_()
    info   = D_TYPE_USHORT_();
    result = d_assert_standalone(
        D_TYPE_IS_USHORT(info) && !D_TYPE_IS_SIGNED(info),
        "builder_ushort",
        "D_TYPE_USHORT_() should create unsigned short",
        _counter) && result;

    // test 8: D_TYPE_INT_()
    info   = D_TYPE_INT_();
    result = d_assert_standalone(
        D_TYPE_IS_INT(info) && D_TYPE_IS_SIGNED(info),
        "builder_int",
        "D_TYPE_INT_() should create signed int",
        _counter) && result;

    // test 9: D_TYPE_UINT_()
    info   = D_TYPE_UINT_();
    result = d_assert_standalone(
        D_TYPE_IS_UINT(info) && !D_TYPE_IS_SIGNED(info),
        "builder_uint",
        "D_TYPE_UINT_() should create unsigned int",
        _counter) && result;

    // test 10: D_TYPE_LONG_()
    info   = D_TYPE_LONG_();
    result = d_assert_standalone(
        D_TYPE_IS_LONG(info) && D_TYPE_IS_SIGNED(info),
        "builder_long",
        "D_TYPE_LONG_() should create signed long",
        _counter) && result;

    // test 11: D_TYPE_ULONG_()
    info   = D_TYPE_ULONG_();
    result = d_assert_standalone(
        D_TYPE_IS_ULONG(info) && !D_TYPE_IS_SIGNED(info),
        "builder_ulong",
        "D_TYPE_ULONG_() should create unsigned long",
        _counter) && result;

    // test 12: D_TYPE_LLONG_()
    info   = D_TYPE_LLONG_();
    result = d_assert_standalone(
        D_TYPE_IS_LLONG(info) && D_TYPE_IS_SIGNED(info),
        "builder_llong",
        "D_TYPE_LLONG_() should create signed long long",
        _counter) && result;

    // test 13: D_TYPE_ULLONG_()
    info   = D_TYPE_ULLONG_();
    result = d_assert_standalone(
        D_TYPE_IS_ULLONG(info) && !D_TYPE_IS_SIGNED(info),
        "builder_ullong",
        "D_TYPE_ULLONG_() should create unsigned long long",
        _counter) && result;

    // test 14: D_TYPE_FLOAT_()
    info   = D_TYPE_FLOAT_();
    result = d_assert_standalone(
        D_TYPE_IS_FLOAT(info) && D_TYPE_IS_SIGNED(info),
        "builder_float",
        "D_TYPE_FLOAT_() should create signed float",
        _counter) && result;

    // test 15: D_TYPE_DOUBLE_()
    info   = D_TYPE_DOUBLE_();
    result = d_assert_standalone(
        D_TYPE_IS_DOUBLE(info) && D_TYPE_IS_SIGNED(info),
        "builder_double",
        "D_TYPE_DOUBLE_() should create signed double",
        _counter) && result;

    // test 16: D_TYPE_LDOUBLE_()
    info   = D_TYPE_LDOUBLE_();
    result = d_assert_standalone(
        D_TYPE_IS_LDOUBLE(info) && D_TYPE_IS_SIGNED(info),
        "builder_ldouble",
        "D_TYPE_LDOUBLE_() should create signed long double",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_compound_builders
  Tests the D_TYPE_*_() compound builder macros.
  Tests the following:
  - Each builder creates correct compound type
  - Compound types do not have PRIMITIVE flag
*/
bool
d_tests_sa_type_info_compound_builders
(
    struct d_test_counter* _counter
)
{
    bool          result;
    d_type_info16 info;

    result = true;

    // test 1: D_TYPE_STRUCT_()
    info   = D_TYPE_STRUCT_();
    result = d_assert_standalone(
        D_TYPE_IS_STRUCT(info),
        "builder_struct",
        "D_TYPE_STRUCT_() should create struct",
        _counter) && result;

    result = d_assert_standalone(
        !D_TYPE_IS_PRIMITIVE(info),
        "builder_struct_not_prim",
        "struct should not be primitive",
        _counter) && result;

    // test 2: D_TYPE_UNION_()
    info   = D_TYPE_UNION_();
    result = d_assert_standalone(
        D_TYPE_IS_UNION(info),
        "builder_union",
        "D_TYPE_UNION_() should create union",
        _counter) && result;

    // test 3: D_TYPE_ENUM_()
    info   = D_TYPE_ENUM_();
    result = d_assert_standalone(
        D_TYPE_IS_ENUM(info),
        "builder_enum",
        "D_TYPE_ENUM_() should create enum",
        _counter) && result;

    // test 4: D_TYPE_FUNCTION_()
    info   = D_TYPE_FUNCTION_();
    result = d_assert_standalone(
        D_TYPE_IS_FUNCTION(info),
        "builder_function",
        "D_TYPE_FUNCTION_() should create function",
        _counter) && result;

    // test 5: D_TYPE_CUSTOM_()
    info   = D_TYPE_CUSTOM_();
    result = d_assert_standalone(
        D_TYPE_IS_CUSTOM_KIND(info),
        "builder_custom",
        "D_TYPE_CUSTOM_() should create custom compound",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_builders_all
  Aggregation function that runs all builder macro tests.
*/
bool
d_tests_sa_type_info_builders_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Builder Macros\n");
    printf("  ------------------------\n");

    result = d_tests_sa_type_info_make(_counter) && result;
    result = d_tests_sa_type_info_make_prim(_counter) && result;
    result = d_tests_sa_type_info_make_compound(_counter) && result;
    result = d_tests_sa_type_info_make_ptr(_counter) && result;
    result = d_tests_sa_type_info_primitive_builders(_counter) && result;
    result = d_tests_sa_type_info_compound_builders(_counter) && result;

    return result;
}
