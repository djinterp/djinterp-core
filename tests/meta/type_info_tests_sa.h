/******************************************************************************
* djinterp [test]                                           type_info_tests_sa.h
*
*   Unit test declarations for `type_info.h` module.
*   Provides comprehensive testing of all type_info utility macros and constants
* including type definitions, bit layout, builder macros, modifier macros,
* accessor/test macros, predefined constants, composite builders, and utility
* macros.
*
*
* path:      \tests\meta\type_info_tests_sa.h
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.02.04
******************************************************************************/

#ifndef DJINTERP_TESTS_TYPE_INFO_SA_
#define DJINTERP_TESTS_TYPE_INFO_SA_ 1

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "..\..\inc\test\test_standalone.h"
#include "..\..\inc\meta\type_info.h"
#include "..\..\inc\string_fn.h"


/******************************************************************************
 * I. TYPE DEFINITIONS AND BIT LAYOUT TESTS
 *****************************************************************************/
bool d_tests_sa_type_info_type_widths(struct d_test_counter* _counter);
bool d_tests_sa_type_info_bit_shifts(struct d_test_counter* _counter);
bool d_tests_sa_type_info_bit_masks(struct d_test_counter* _counter);
bool d_tests_sa_type_info_kind_constants(struct d_test_counter* _counter);
bool d_tests_sa_type_info_primitive_ids(struct d_test_counter* _counter);
bool d_tests_sa_type_info_compound_ids(struct d_test_counter* _counter);
bool d_tests_sa_type_info_prim_sizes_table(struct d_test_counter* _counter);
bool d_tests_sa_type_info_prim_signed_mask(struct d_test_counter* _counter);

// I.   aggregation function
bool d_tests_sa_type_info_definitions_all(struct d_test_counter* _counter);


/******************************************************************************
 * II. BUILDER MACRO TESTS
 *****************************************************************************/
bool d_tests_sa_type_info_make(struct d_test_counter* _counter);
bool d_tests_sa_type_info_make_prim(struct d_test_counter* _counter);
bool d_tests_sa_type_info_make_compound(struct d_test_counter* _counter);
bool d_tests_sa_type_info_make_ptr(struct d_test_counter* _counter);
bool d_tests_sa_type_info_primitive_builders(struct d_test_counter* _counter);
bool d_tests_sa_type_info_compound_builders(struct d_test_counter* _counter);

// II.  aggregation function
bool d_tests_sa_type_info_builders_all(struct d_test_counter* _counter);


/******************************************************************************
 * III. MODIFIER MACRO TESTS
 *****************************************************************************/
bool d_tests_sa_type_info_set_array(struct d_test_counter* _counter);
bool d_tests_sa_type_info_set_typedef(struct d_test_counter* _counter);
bool d_tests_sa_type_info_set_ext(struct d_test_counter* _counter);
bool d_tests_sa_type_info_set_custom(struct d_test_counter* _counter);
bool d_tests_sa_type_info_set_const(struct d_test_counter* _counter);
bool d_tests_sa_type_info_set_volatile(struct d_test_counter* _counter);
bool d_tests_sa_type_info_get_set_ptr_depth(struct d_test_counter* _counter);
bool d_tests_sa_type_info_add_sub_ptr(struct d_test_counter* _counter);
bool d_tests_sa_type_info_extended_modifiers(struct d_test_counter* _counter);

// III. aggregation function
bool d_tests_sa_type_info_modifiers_all(struct d_test_counter* _counter);


/******************************************************************************
 * IV. ACCESSOR AND TEST MACRO TESTS
 *****************************************************************************/
bool d_tests_sa_type_info_get_kind(struct d_test_counter* _counter);
bool d_tests_sa_type_info_get_sub(struct d_test_counter* _counter);
bool d_tests_sa_type_info_is_primitive(struct d_test_counter* _counter);
bool d_tests_sa_type_info_is_pointer(struct d_test_counter* _counter);
bool d_tests_sa_type_info_is_array(struct d_test_counter* _counter);
bool d_tests_sa_type_info_is_typedef(struct d_test_counter* _counter);
bool d_tests_sa_type_info_qualifiers(struct d_test_counter* _counter);
bool d_tests_sa_type_info_specific_primitives(struct d_test_counter* _counter);
bool d_tests_sa_type_info_type_categories(struct d_test_counter* _counter);
bool d_tests_sa_type_info_compound_tests(struct d_test_counter* _counter);
bool d_tests_sa_type_info_get_size(struct d_test_counter* _counter);

// IV.  aggregation function
bool d_tests_sa_type_info_accessors_all(struct d_test_counter* _counter);


/******************************************************************************
 * V. PREDEFINED CONSTANT TESTS
 *****************************************************************************/
bool d_tests_sa_type_info_predefined_primitives(struct d_test_counter* _counter);
bool d_tests_sa_type_info_predefined_cv_variants(struct d_test_counter* _counter);
bool d_tests_sa_type_info_predefined_arrays(struct d_test_counter* _counter);
bool d_tests_sa_type_info_predefined_pointers(struct d_test_counter* _counter);
bool d_tests_sa_type_info_predefined_fixed_width(struct d_test_counter* _counter);
bool d_tests_sa_type_info_predefined_strings(struct d_test_counter* _counter);
bool d_tests_sa_type_info_predefined_size_t(struct d_test_counter* _counter);
bool d_tests_sa_type_info_static_constants(struct d_test_counter* _counter);

// V.   aggregation function
bool d_tests_sa_type_info_predefined_all(struct d_test_counter* _counter);


/******************************************************************************
 * VI. COMPOSITE BUILDER TESTS
 *****************************************************************************/
bool d_tests_sa_type_info_ptr_to(struct d_test_counter* _counter);
bool d_tests_sa_type_info_array_of(struct d_test_counter* _counter);
bool d_tests_sa_type_info_const_of(struct d_test_counter* _counter);
bool d_tests_sa_type_info_volatile_of(struct d_test_counter* _counter);
bool d_tests_sa_type_info_cv_of(struct d_test_counter* _counter);
bool d_tests_sa_type_info_custom_of(struct d_test_counter* _counter);
bool d_tests_sa_type_info_ptr_array(struct d_test_counter* _counter);
bool d_tests_sa_type_info_const_ptr_variants(struct d_test_counter* _counter);

// VI.  aggregation function
bool d_tests_sa_type_info_composite_all(struct d_test_counter* _counter);


/******************************************************************************
 * VII. UTILITY MACRO TESTS
 *****************************************************************************/
bool d_tests_sa_type_info_strip_cv(struct d_test_counter* _counter);
bool d_tests_sa_type_info_strip_ptr(struct d_test_counter* _counter);
bool d_tests_sa_type_info_strip_all_ptr(struct d_test_counter* _counter);
bool d_tests_sa_type_info_base(struct d_test_counter* _counter);
bool d_tests_sa_type_info_eq(struct d_test_counter* _counter);
bool d_tests_sa_type_info_eq_base(struct d_test_counter* _counter);
bool d_tests_sa_type_info_compat(struct d_test_counter* _counter);
bool d_tests_sa_type_info_to_base(struct d_test_counter* _counter);
bool d_tests_sa_type_info_to_ptr(struct d_test_counter* _counter);

// VII. aggregation function
bool d_tests_sa_type_info_utility_all(struct d_test_counter* _counter);


/******************************************************************************
 * MODULE-LEVEL AGGREGATION
 *****************************************************************************/
bool d_tests_sa_type_info_run_all(struct d_test_counter* _counter);


#endif  // DJINTERP_TESTS_TYPE_INFO_SA_
