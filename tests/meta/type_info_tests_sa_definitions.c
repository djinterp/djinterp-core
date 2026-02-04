#include ".\type_info_tests_sa.h"


/*
d_tests_sa_type_info_type_widths
  Tests that the type definitions have the correct bit widths.
  Tests the following:
  - d_type_info16 is 16 bits (2 bytes)
  - d_type_info32 is 32 bits (4 bytes)
  - d_type_info_ex is 32 bits (4 bytes)
  - d_type_info64 is 64 bits (8 bytes)
*/
bool
d_tests_sa_type_info_type_widths
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: d_type_info16 should be 16 bits (2 bytes)
    result = d_assert_standalone(
        sizeof(d_type_info16) == 2,
        "type_info16_size",
        "d_type_info16 should be 2 bytes (16 bits)",
        _counter) && result;

    // test 2: d_type_info32 should be 32 bits (4 bytes)
    result = d_assert_standalone(
        sizeof(d_type_info32) == 4,
        "type_info32_size",
        "d_type_info32 should be 4 bytes (32 bits)",
        _counter) && result;

    // test 3: d_type_info_ex should be 32 bits (4 bytes)
    result = d_assert_standalone(
        sizeof(d_type_info_ex) == 4,
        "type_info_ex_size",
        "d_type_info_ex should be 4 bytes (32 bits)",
        _counter) && result;

    // test 4: d_type_info64 should be 64 bits (8 bytes)
    result = d_assert_standalone(
        sizeof(d_type_info64) == 8,
        "type_info64_size",
        "d_type_info64 should be 8 bytes (64 bits)",
        _counter) && result;

    // test 5: underlying types are correct unsigned integers
    result = d_assert_standalone(
        (d_type_info16)(-1) > 0,
        "type_info16_unsigned",
        "d_type_info16 should be unsigned",
        _counter) && result;

    result = d_assert_standalone(
        (d_type_info32)(-1) > 0,
        "type_info32_unsigned",
        "d_type_info32 should be unsigned",
        _counter) && result;

    result = d_assert_standalone(
        (d_type_info64)(-1) > 0,
        "type_info64_unsigned",
        "d_type_info64 should be unsigned",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_bit_shifts
  Tests that the bit shift constants are defined correctly.
  Tests the following:
  - Kind flags shift (bits 0-3)
  - Subtype shift (bits 4-7)
  - Signed shift (bit 8)
  - Context shifts (bits 8-10)
  - Qualifier shifts (bits 11-12)
  - Extended/Custom shifts (bits 14-15)
  - Pointer depth shift (bits 16-23)
  - Extended 32-bit flag shifts (bits 24-31)
*/
bool
d_tests_sa_type_info_bit_shifts
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: kind flags shift starts at bit 0
    result = d_assert_standalone(
        D_TYPE_KIND_SHIFT == 0u,
        "kind_shift",
        "D_TYPE_KIND_SHIFT should be 0",
        _counter) && result;

    // test 2: kind flags use 4 bits
    result = d_assert_standalone(
        D_TYPE_KIND_BITS == 4u,
        "kind_bits",
        "D_TYPE_KIND_BITS should be 4",
        _counter) && result;

    // test 3: subtype shift starts at bit 4
    result = d_assert_standalone(
        D_TYPE_SUB_SHIFT == 4u,
        "sub_shift",
        "D_TYPE_SUB_SHIFT should be 4",
        _counter) && result;

    // test 4: subtype uses 4 bits
    result = d_assert_standalone(
        D_TYPE_SUB_BITS == 4u,
        "sub_bits",
        "D_TYPE_SUB_BITS should be 4",
        _counter) && result;

    // test 5: signed bit at position 8
    result = d_assert_standalone(
        D_TYPE_SIGNED_SHIFT == 8u,
        "signed_shift",
        "D_TYPE_SIGNED_SHIFT should be 8",
        _counter) && result;

    // test 6: context bits at positions 8-10
    result = d_assert_standalone(
        D_TYPE_CTX0_SHIFT == 8u,
        "ctx0_shift",
        "D_TYPE_CTX0_SHIFT should be 8",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_CTX1_SHIFT == 9u,
        "ctx1_shift",
        "D_TYPE_CTX1_SHIFT should be 9",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_CTX2_SHIFT == 10u,
        "ctx2_shift",
        "D_TYPE_CTX2_SHIFT should be 10",
        _counter) && result;

    // test 7: const qualifier at bit 11
    result = d_assert_standalone(
        D_TYPE_CONST_SHIFT == 11u,
        "const_shift",
        "D_TYPE_CONST_SHIFT should be 11",
        _counter) && result;

    // test 8: volatile qualifier at bit 12
    result = d_assert_standalone(
        D_TYPE_VOLATILE_SHIFT == 12u,
        "volatile_shift",
        "D_TYPE_VOLATILE_SHIFT should be 12",
        _counter) && result;

    // test 9: reserved bit at position 13
    result = d_assert_standalone(
        D_TYPE_RSV_SHIFT == 13u,
        "rsv_shift",
        "D_TYPE_RSV_SHIFT should be 13",
        _counter) && result;

    // test 10: extended flag at bit 14
    result = d_assert_standalone(
        D_TYPE_EXT_SHIFT == 14u,
        "ext_shift",
        "D_TYPE_EXT_SHIFT should be 14",
        _counter) && result;

    // test 11: custom flag at bit 15
    result = d_assert_standalone(
        D_TYPE_CUSTOM_SHIFT == 15u,
        "custom_shift",
        "D_TYPE_CUSTOM_SHIFT should be 15",
        _counter) && result;

    // test 12: pointer depth starts at bit 16
    result = d_assert_standalone(
        D_TYPE_PTR_DEPTH_SHIFT == 16u,
        "ptr_depth_shift",
        "D_TYPE_PTR_DEPTH_SHIFT should be 16",
        _counter) && result;

    // test 13: pointer depth uses 8 bits
    result = d_assert_standalone(
        D_TYPE_PTR_DEPTH_BITS == 8u,
        "ptr_depth_bits",
        "D_TYPE_PTR_DEPTH_BITS should be 8",
        _counter) && result;

    // test 14: pointer depth max is 255
    result = d_assert_standalone(
        D_TYPE_PTR_DEPTH_MAX == 255u,
        "ptr_depth_max",
        "D_TYPE_PTR_DEPTH_MAX should be 255",
        _counter) && result;

    // test 15: extended 32-bit flag shifts (bits 24-31)
    result = d_assert_standalone(
        D_TYPE_RESTRICT_SHIFT == 24u,
        "restrict_shift",
        "D_TYPE_RESTRICT_SHIFT should be 24",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_ATOMIC_SHIFT == 25u,
        "atomic_shift",
        "D_TYPE_ATOMIC_SHIFT should be 25",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_STATIC_SHIFT == 26u,
        "static_shift",
        "D_TYPE_STATIC_SHIFT should be 26",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_EXTERN_SHIFT == 27u,
        "extern_shift",
        "D_TYPE_EXTERN_SHIFT should be 27",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_INLINE_SHIFT == 28u,
        "inline_shift",
        "D_TYPE_INLINE_SHIFT should be 28",
        _counter) && result;

    result = d_assert_standalone(
        D_TYPE_NORETURN_SHIFT == 29u,
        "noreturn_shift",
        "D_TYPE_NORETURN_SHIFT should be 29",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_bit_masks
  Tests that the bit masks are correctly defined.
  Tests the following:
  - Kind mask covers bits 0-3
  - Subtype mask covers bits 4-7
  - Context mask covers bits 8-10
  - Individual bit masks are correct powers of 2
  - Pointer depth mask covers bits 16-23
*/
bool
d_tests_sa_type_info_bit_masks
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: kind mask should be 0x0F (bits 0-3)
    result = d_assert_standalone(
        D_TYPE_KIND_MASK == 0x000Fu,
        "kind_mask_value",
        "D_TYPE_KIND_MASK should be 0x000F",
        _counter) && result;

    // test 2: subtype mask should be 0xF0 (bits 4-7)
    result = d_assert_standalone(
        D_TYPE_SUB_MASK == 0x00F0u,
        "sub_mask_value",
        "D_TYPE_SUB_MASK should be 0x00F0",
        _counter) && result;

    // test 3: context mask should cover bits 8-10
    result = d_assert_standalone(
        D_TYPE_CTX_MASK == 0x0700u,
        "ctx_mask_value",
        "D_TYPE_CTX_MASK should be 0x0700",
        _counter) && result;

    // test 4: signed bit is correct
    result = d_assert_standalone(
        D_TYPE_SIGNED_BIT == 0x0100u,
        "signed_bit_value",
        "D_TYPE_SIGNED_BIT should be 0x0100",
        _counter) && result;

    // test 5: const bit is correct
    result = d_assert_standalone(
        D_TYPE_CONST_BIT == 0x0800u,
        "const_bit_value",
        "D_TYPE_CONST_BIT should be 0x0800",
        _counter) && result;

    // test 6: volatile bit is correct
    result = d_assert_standalone(
        D_TYPE_VOLATILE_BIT == 0x1000u,
        "volatile_bit_value",
        "D_TYPE_VOLATILE_BIT should be 0x1000",
        _counter) && result;

    // test 7: reserved bit is correct
    result = d_assert_standalone(
        D_TYPE_RSV_BIT == 0x2000u,
        "rsv_bit_value",
        "D_TYPE_RSV_BIT should be 0x2000",
        _counter) && result;

    // test 8: ext bit is correct
    result = d_assert_standalone(
        D_TYPE_EXT_BIT == 0x4000u,
        "ext_bit_value",
        "D_TYPE_EXT_BIT should be 0x4000",
        _counter) && result;

    // test 9: custom bit is correct
    result = d_assert_standalone(
        D_TYPE_CUSTOM_BIT == 0x8000u,
        "custom_bit_value",
        "D_TYPE_CUSTOM_BIT should be 0x8000",
        _counter) && result;

    // test 10: pointer depth mask (64-bit) covers bits 16-23
    result = d_assert_standalone(
        D_TYPE_PTR_DEPTH_MASK_U64 == 0x00FF0000ull,
        "ptr_depth_mask_value",
        "D_TYPE_PTR_DEPTH_MASK_U64 should be 0x00FF0000",
        _counter) && result;

    // test 11: context bit 0
    result = d_assert_standalone(
        D_TYPE_CTX0_BIT == 0x0100u,
        "ctx0_bit_value",
        "D_TYPE_CTX0_BIT should be 0x0100",
        _counter) && result;

    // test 12: context bit 1
    result = d_assert_standalone(
        D_TYPE_CTX1_BIT == 0x0200u,
        "ctx1_bit_value",
        "D_TYPE_CTX1_BIT should be 0x0200",
        _counter) && result;

    // test 13: context bit 2
    result = d_assert_standalone(
        D_TYPE_CTX2_BIT == 0x0400u,
        "ctx2_bit_value",
        "D_TYPE_CTX2_BIT should be 0x0400",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_kind_constants
  Tests the kind flag constants (stored in bits 0-3).
  Tests the following:
  - D_TYPE_PRIMITIVE is bit 0
  - D_TYPE_POINTER is bit 1
  - D_TYPE_ARRAY is bit 2
  - D_TYPE_TYPEDEF is bit 3
  - Kinds can be combined
*/
bool
d_tests_sa_type_info_kind_constants
(
    struct d_test_counter* _counter
)
{
    bool           result;
    d_type_info16  combined;

    result = true;

    // test 1: primitive flag is bit 0
    result = d_assert_standalone(
        D_TYPE_PRIMITIVE == 0x0001u,
        "primitive_flag",
        "D_TYPE_PRIMITIVE should be 0x0001 (bit 0)",
        _counter) && result;

    // test 2: pointer flag is bit 1
    result = d_assert_standalone(
        D_TYPE_POINTER == 0x0002u,
        "pointer_flag",
        "D_TYPE_POINTER should be 0x0002 (bit 1)",
        _counter) && result;

    // test 3: array flag is bit 2
    result = d_assert_standalone(
        D_TYPE_ARRAY == 0x0004u,
        "array_flag",
        "D_TYPE_ARRAY should be 0x0004 (bit 2)",
        _counter) && result;

    // test 4: typedef flag is bit 3
    result = d_assert_standalone(
        D_TYPE_TYPEDEF == 0x0008u,
        "typedef_flag",
        "D_TYPE_TYPEDEF should be 0x0008 (bit 3)",
        _counter) && result;

    // test 5: kinds can be combined (e.g., primitive pointer)
    combined = D_TYPE_PRIMITIVE | D_TYPE_POINTER;
    result   = d_assert_standalone(
        combined == 0x0003u,
        "combined_prim_ptr",
        "PRIMITIVE | POINTER should be 0x0003",
        _counter) && result;

    // test 6: all four kinds combined
    combined = D_TYPE_PRIMITIVE | D_TYPE_POINTER | D_TYPE_ARRAY | D_TYPE_TYPEDEF;
    result   = d_assert_standalone(
        combined == 0x000Fu,
        "all_kinds_combined",
        "All four kinds combined should be 0x000F",
        _counter) && result;

    // test 7: D_TYPE_KIND_MASK_ alias works
    result = d_assert_standalone(
        D_TYPE_KIND_MASK_ == D_TYPE_KIND_MASK,
        "kind_mask_alias",
        "D_TYPE_KIND_MASK_ should equal D_TYPE_KIND_MASK",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_primitive_ids
  Tests the primitive type ID constants (0-15).
  Tests the following:
  - All primitive IDs are in range 0-15
  - Each primitive has a unique ID
  - D_TYPE_PRIM_COUNT is 16
*/
bool
d_tests_sa_type_info_primitive_ids
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: void is ID 0
    result = d_assert_standalone(
        D_TYPE_PRIM_VOID == 0u,
        "prim_void_id",
        "D_TYPE_PRIM_VOID should be 0",
        _counter) && result;

    // test 2: bool is ID 1
    result = d_assert_standalone(
        D_TYPE_PRIM_BOOL == 1u,
        "prim_bool_id",
        "D_TYPE_PRIM_BOOL should be 1",
        _counter) && result;

    // test 3: char is ID 2
    result = d_assert_standalone(
        D_TYPE_PRIM_CHAR == 2u,
        "prim_char_id",
        "D_TYPE_PRIM_CHAR should be 2",
        _counter) && result;

    // test 4: signed char is ID 3
    result = d_assert_standalone(
        D_TYPE_PRIM_SCHAR == 3u,
        "prim_schar_id",
        "D_TYPE_PRIM_SCHAR should be 3",
        _counter) && result;

    // test 5: unsigned char is ID 4
    result = d_assert_standalone(
        D_TYPE_PRIM_UCHAR == 4u,
        "prim_uchar_id",
        "D_TYPE_PRIM_UCHAR should be 4",
        _counter) && result;

    // test 6: short is ID 5
    result = d_assert_standalone(
        D_TYPE_PRIM_SHORT == 5u,
        "prim_short_id",
        "D_TYPE_PRIM_SHORT should be 5",
        _counter) && result;

    // test 7: unsigned short is ID 6
    result = d_assert_standalone(
        D_TYPE_PRIM_USHORT == 6u,
        "prim_ushort_id",
        "D_TYPE_PRIM_USHORT should be 6",
        _counter) && result;

    // test 8: int is ID 7
    result = d_assert_standalone(
        D_TYPE_PRIM_INT == 7u,
        "prim_int_id",
        "D_TYPE_PRIM_INT should be 7",
        _counter) && result;

    // test 9: unsigned int is ID 8
    result = d_assert_standalone(
        D_TYPE_PRIM_UINT == 8u,
        "prim_uint_id",
        "D_TYPE_PRIM_UINT should be 8",
        _counter) && result;

    // test 10: long is ID 9
    result = d_assert_standalone(
        D_TYPE_PRIM_LONG == 9u,
        "prim_long_id",
        "D_TYPE_PRIM_LONG should be 9",
        _counter) && result;

    // test 11: unsigned long is ID 10
    result = d_assert_standalone(
        D_TYPE_PRIM_ULONG == 10u,
        "prim_ulong_id",
        "D_TYPE_PRIM_ULONG should be 10",
        _counter) && result;

    // test 12: long long is ID 11
    result = d_assert_standalone(
        D_TYPE_PRIM_LLONG == 11u,
        "prim_llong_id",
        "D_TYPE_PRIM_LLONG should be 11",
        _counter) && result;

    // test 13: unsigned long long is ID 12
    result = d_assert_standalone(
        D_TYPE_PRIM_ULLONG == 12u,
        "prim_ullong_id",
        "D_TYPE_PRIM_ULLONG should be 12",
        _counter) && result;

    // test 14: float is ID 13
    result = d_assert_standalone(
        D_TYPE_PRIM_FLOAT == 13u,
        "prim_float_id",
        "D_TYPE_PRIM_FLOAT should be 13",
        _counter) && result;

    // test 15: double is ID 14
    result = d_assert_standalone(
        D_TYPE_PRIM_DOUBLE == 14u,
        "prim_double_id",
        "D_TYPE_PRIM_DOUBLE should be 14",
        _counter) && result;

    // test 16: long double is ID 15
    result = d_assert_standalone(
        D_TYPE_PRIM_LDOUBLE == 15u,
        "prim_ldouble_id",
        "D_TYPE_PRIM_LDOUBLE should be 15",
        _counter) && result;

    // test 17: primitive count is 16
    result = d_assert_standalone(
        D_TYPE_PRIM_COUNT == 16u,
        "prim_count",
        "D_TYPE_PRIM_COUNT should be 16",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_compound_ids
  Tests the compound type ID constants.
  Tests the following:
  - D_TYPE_NONE is 0
  - D_TYPE_STRUCT is 1
  - D_TYPE_UNION is 2
  - D_TYPE_ENUM is 3
  - D_TYPE_FUNCTION is 4
  - D_TYPE_CUSTOM is 5
*/
bool
d_tests_sa_type_info_compound_ids
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: none is 0
    result = d_assert_standalone(
        D_TYPE_NONE == 0u,
        "compound_none_id",
        "D_TYPE_NONE should be 0",
        _counter) && result;

    // test 2: struct is 1
    result = d_assert_standalone(
        D_TYPE_STRUCT == 1u,
        "compound_struct_id",
        "D_TYPE_STRUCT should be 1",
        _counter) && result;

    // test 3: union is 2
    result = d_assert_standalone(
        D_TYPE_UNION == 2u,
        "compound_union_id",
        "D_TYPE_UNION should be 2",
        _counter) && result;

    // test 4: enum is 3
    result = d_assert_standalone(
        D_TYPE_ENUM == 3u,
        "compound_enum_id",
        "D_TYPE_ENUM should be 3",
        _counter) && result;

    // test 5: function is 4
    result = d_assert_standalone(
        D_TYPE_FUNCTION == 4u,
        "compound_function_id",
        "D_TYPE_FUNCTION should be 4",
        _counter) && result;

    // test 6: custom is 5
    result = d_assert_standalone(
        D_TYPE_CUSTOM == 5u,
        "compound_custom_id",
        "D_TYPE_CUSTOM should be 5",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_prim_sizes_table
  Tests the d_type_prim_sizes lookup table.
  Tests the following:
  - Table has correct number of entries
  - Each entry matches sizeof() for the corresponding type
  - void has size 0
*/
bool
d_tests_sa_type_info_prim_sizes_table
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    // test 1: void size is 0
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_VOID] == 0,
        "prim_size_void",
        "void size should be 0",
        _counter) && result;

    // test 2: bool size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_BOOL] == sizeof(_Bool),
        "prim_size_bool",
        "_Bool size should match sizeof(_Bool)",
        _counter) && result;

    // test 3: char size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_CHAR] == sizeof(char),
        "prim_size_char",
        "char size should match sizeof(char)",
        _counter) && result;

    // test 4: signed char size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_SCHAR] == sizeof(signed char),
        "prim_size_schar",
        "signed char size should match sizeof(signed char)",
        _counter) && result;

    // test 5: unsigned char size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_UCHAR] == sizeof(unsigned char),
        "prim_size_uchar",
        "unsigned char size should match sizeof(unsigned char)",
        _counter) && result;

    // test 6: short size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_SHORT] == sizeof(short),
        "prim_size_short",
        "short size should match sizeof(short)",
        _counter) && result;

    // test 7: unsigned short size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_USHORT] == sizeof(unsigned short),
        "prim_size_ushort",
        "unsigned short size should match sizeof(unsigned short)",
        _counter) && result;

    // test 8: int size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_INT] == sizeof(int),
        "prim_size_int",
        "int size should match sizeof(int)",
        _counter) && result;

    // test 9: unsigned int size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_UINT] == sizeof(unsigned int),
        "prim_size_uint",
        "unsigned int size should match sizeof(unsigned int)",
        _counter) && result;

    // test 10: long size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_LONG] == sizeof(long),
        "prim_size_long",
        "long size should match sizeof(long)",
        _counter) && result;

    // test 11: unsigned long size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_ULONG] == sizeof(unsigned long),
        "prim_size_ulong",
        "unsigned long size should match sizeof(unsigned long)",
        _counter) && result;

    // test 12: long long size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_LLONG] == sizeof(long long),
        "prim_size_llong",
        "long long size should match sizeof(long long)",
        _counter) && result;

    // test 13: unsigned long long size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_ULLONG] == sizeof(unsigned long long),
        "prim_size_ullong",
        "unsigned long long size should match sizeof(unsigned long long)",
        _counter) && result;

    // test 14: float size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_FLOAT] == sizeof(float),
        "prim_size_float",
        "float size should match sizeof(float)",
        _counter) && result;

    // test 15: double size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_DOUBLE] == sizeof(double),
        "prim_size_double",
        "double size should match sizeof(double)",
        _counter) && result;

    // test 16: long double size
    result = d_assert_standalone(
        d_type_prim_sizes[D_TYPE_PRIM_LDOUBLE] == sizeof(long double),
        "prim_size_ldouble",
        "long double size should match sizeof(long double)",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_prim_signed_mask
  Tests the D_TYPE_PRIM_SIGNED_MASK constant.
  Tests the following:
  - Signed types have their bit set
  - Unsigned types do not have their bit set
  - void and bool are not signed
*/
bool
d_tests_sa_type_info_prim_signed_mask
(
    struct d_test_counter* _counter
)
{
    bool     result;
    uint16_t mask;

    result = true;
    mask   = D_TYPE_PRIM_SIGNED_MASK;

    // test 1: void is not signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_VOID)) == 0,
        "signed_mask_void",
        "void should not be marked as signed",
        _counter) && result;

    // test 2: bool is not signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_BOOL)) == 0,
        "signed_mask_bool",
        "_Bool should not be marked as signed",
        _counter) && result;

    // test 3: char is not signed (implementation-defined, but mask says no)
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_CHAR)) == 0,
        "signed_mask_char",
        "char should not be marked as signed in mask",
        _counter) && result;

    // test 4: signed char IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_SCHAR)) != 0,
        "signed_mask_schar",
        "signed char should be marked as signed",
        _counter) && result;

    // test 5: unsigned char is not signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_UCHAR)) == 0,
        "signed_mask_uchar",
        "unsigned char should not be marked as signed",
        _counter) && result;

    // test 6: short IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_SHORT)) != 0,
        "signed_mask_short",
        "short should be marked as signed",
        _counter) && result;

    // test 7: unsigned short is not signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_USHORT)) == 0,
        "signed_mask_ushort",
        "unsigned short should not be marked as signed",
        _counter) && result;

    // test 8: int IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_INT)) != 0,
        "signed_mask_int",
        "int should be marked as signed",
        _counter) && result;

    // test 9: unsigned int is not signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_UINT)) == 0,
        "signed_mask_uint",
        "unsigned int should not be marked as signed",
        _counter) && result;

    // test 10: long IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_LONG)) != 0,
        "signed_mask_long",
        "long should be marked as signed",
        _counter) && result;

    // test 11: unsigned long is not signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_ULONG)) == 0,
        "signed_mask_ulong",
        "unsigned long should not be marked as signed",
        _counter) && result;

    // test 12: long long IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_LLONG)) != 0,
        "signed_mask_llong",
        "long long should be marked as signed",
        _counter) && result;

    // test 13: unsigned long long is not signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_ULLONG)) == 0,
        "signed_mask_ullong",
        "unsigned long long should not be marked as signed",
        _counter) && result;

    // test 14: float IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_FLOAT)) != 0,
        "signed_mask_float",
        "float should be marked as signed",
        _counter) && result;

    // test 15: double IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_DOUBLE)) != 0,
        "signed_mask_double",
        "double should be marked as signed",
        _counter) && result;

    // test 16: long double IS signed
    result = d_assert_standalone(
        (mask & (1u << D_TYPE_PRIM_LDOUBLE)) != 0,
        "signed_mask_ldouble",
        "long double should be marked as signed",
        _counter) && result;

    return result;
}


/*
d_tests_sa_type_info_definitions_all
  Aggregation function that runs all type definition and bit layout tests.
*/
bool
d_tests_sa_type_info_definitions_all
(
    struct d_test_counter* _counter
)
{
    bool result;

    result = true;

    printf("\n  [SECTION] Type Definitions and Bit Layout\n");
    printf("  ------------------------------------------\n");

    result = d_tests_sa_type_info_type_widths(_counter) && result;
    result = d_tests_sa_type_info_bit_shifts(_counter) && result;
    result = d_tests_sa_type_info_bit_masks(_counter) && result;
    result = d_tests_sa_type_info_kind_constants(_counter) && result;
    result = d_tests_sa_type_info_primitive_ids(_counter) && result;
    result = d_tests_sa_type_info_compound_ids(_counter) && result;
    result = d_tests_sa_type_info_prim_sizes_table(_counter) && result;
    result = d_tests_sa_type_info_prim_signed_mask(_counter) && result;

    return result;
}
