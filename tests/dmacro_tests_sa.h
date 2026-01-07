/******************************************************************************
* djinterp [test]                                           dmacro_tests_sa.h
*
*   This is a test file for `dmacro.h` unit tests.
*   For the file itself, go to `\inc\dmacro.h`.
*   Note: this module tests fundamental macro utilities that are dependencies
* of other djinterp modules, so it uses `test_standalone.h` rather than DTest
* for unit testing. Any modules that are not dependencies of DTest should use
* DTest for unit tests.
*
*
* path:      \test\dmacro_tests_sa.h
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.12.30
******************************************************************************/

#ifndef DJINTERP_TESTING_DMACRO_
#define DJINTERP_TESTING_DMACRO_ 1

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "..\inc\djinterp.h"
#include "..\inc\dmacro.h"
#include "..\inc\string_fn.h"
#include "..\inc\test\test_standalone.h"


// =============================================================================
// 0.   CONFIGURATION SYSTEM TEST FUNCTIONS
// =============================================================================

// configuration constant tests
bool d_tests_sa_dmacro_cfg_constants(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_cfg_user_options(struct d_test_counter* _test_info);

// effective value tests
bool d_tests_sa_dmacro_cfg_effective_values(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_cfg_public_aliases(struct d_test_counter* _test_info);

// query macro tests
bool d_tests_sa_dmacro_cfg_query_macros(struct d_test_counter* _test_info);

// environment integration tests
bool d_tests_sa_dmacro_cfg_env_integration(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_cfg_variant_consistency(struct d_test_counter* _test_info);

// configuration module aggregator
bool d_tests_sa_dmacro_cfg_all(struct d_test_counter* _test_info);


// =============================================================================
// I.   TOKEN MANIPULATION TEST FUNCTIONS (Section I of dmacro.h)
// =============================================================================

// token pasting tests (D_CONCAT, D_INTERNAL_CONCAT_HELPER)
bool d_tests_sa_dmacro_concat_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_concat_with_macros(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_concat_edge_cases(struct d_test_counter* _test_info);

// stringification tests (D_STRINGIFY, D_TOSTR)
bool d_tests_sa_dmacro_stringify_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_stringify_vs_tostr(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_stringify_edge_cases(struct d_test_counter* _test_info);

// expansion control tests (D_EXPAND, D_EMPTY, D_DEFER, D_OBSTRUCT, D_UNPACK)
bool d_tests_sa_dmacro_expand_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_empty_macro(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_defer_macro(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_obstruct_macro(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_unpack_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_unpack_nested(struct d_test_counter* _test_info);

// token manipulation module aggregator
bool d_tests_sa_dmacro_token_all(struct d_test_counter* _test_info);


// =============================================================================
// II.  ARGUMENT COUNTING UTILITIES TEST FUNCTIONS (Section II of dmacro.h)
// =============================================================================

// D_VARG_COUNT tests
bool d_tests_sa_dmacro_varg_count_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_varg_count_medium(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_varg_count_large(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_varg_count_types(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_varg_count_edge_cases(struct d_test_counter* _test_info);

// D_HAS_ARGS tests
bool d_tests_sa_dmacro_has_args_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_has_args_types(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_has_args_large_counts(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_has_args_conditionals(struct d_test_counter* _test_info);

// combined and boundary tests
bool d_tests_sa_dmacro_argcount_combined(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_argcount_boundary(struct d_test_counter* _test_info);

// argument counting module aggregator
bool d_tests_sa_dmacro_argcount_all(struct d_test_counter* _test_info);


// =============================================================================
// III. MACRO EXPANSION AND EVALUATION TEST FUNCTIONS (Section III of dmacro.h)
// =============================================================================

// D_INC tests
bool d_tests_sa_dmacro_inc_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_inc_medium(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_inc_large(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_inc_chained(struct d_test_counter* _test_info);

// D_EVAL tests
bool d_tests_sa_dmacro_eval_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_eval_nested(struct d_test_counter* _test_info);

// iteration macro tests (MSVC-compatible, use D_STRINGIFY only)
bool d_tests_sa_dmacro_for_each_stringify(struct d_test_counter* _test_info);

// practical patterns test
bool d_tests_sa_dmacro_eval_practical(struct d_test_counter* _test_info);

// macro expansion and evaluation module aggregator
bool d_tests_sa_dmacro_eval_all(struct d_test_counter* _test_info);


// =============================================================================
// IV.  ARRAY UTILITIES TEST FUNCTIONS (Section IV of dmacro.h)
// =============================================================================

// compile-time array sizing tests
bool d_tests_sa_dmacro_array_count_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_array_count_initialized(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_array_count_structs(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_array_count_safe(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_array_count_t(struct d_test_counter* _test_info);

// array initializer tests
bool d_tests_sa_dmacro_make_array(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_make_string_array(struct d_test_counter* _test_info);

// array utilities module aggregator
bool d_tests_sa_dmacro_array_all(struct d_test_counter* _test_info);


// =============================================================================
// III. ARGUMENT COUNTING UTILITIES TEST FUNCTIONS (Section III of dmacro.h)
// =============================================================================

// argument counting tests
bool d_tests_sa_dmacro_varg_count_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_varg_count_limits(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_has_args(struct d_test_counter* _test_info);

// positional argument extraction tests
bool d_tests_sa_dmacro_varg_get_arg(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_varg_get_aliases(struct d_test_counter* _test_info);

// argument utilities module aggregator
bool d_tests_sa_dmacro_args_all(struct d_test_counter* _test_info);


// =============================================================================
// IV.  MACRO EXPANSION AND EVALUATION TEST FUNCTIONS (Section IV of dmacro.h)
// =============================================================================

// cascading expansion tests
bool d_tests_sa_dmacro_eval_basic(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_eval_nested(struct d_test_counter* _test_info);

// evaluation module aggregator
bool d_tests_sa_dmacro_eval_all(struct d_test_counter* _test_info);


// =============================================================================
// V.   BOOLEAN AND CONDITIONAL LOGIC TEST FUNCTIONS (Section V of dmacro.h)
// =============================================================================

// probe mechanism tests
bool d_tests_sa_dmacro_probe_check(struct d_test_counter* _test_info);

// parentheses detection tests
bool d_tests_sa_dmacro_is_paren(struct d_test_counter* _test_info);

// conditional expansion tests
bool d_tests_sa_dmacro_if_macros(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_iif_macros(struct d_test_counter* _test_info);

// boolean logic tests
bool d_tests_sa_dmacro_not_compl(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_bool_macro(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_and_or(struct d_test_counter* _test_info);

// boolean logic module aggregator
bool d_tests_sa_dmacro_boolean_all(struct d_test_counter* _test_info);


// =============================================================================
// VI.  ARGUMENT SELECTION TEST FUNCTIONS (Section VI of dmacro.h)
// =============================================================================

// convenience alias tests
bool d_tests_sa_dmacro_first_second_third(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_head_rest_tail(struct d_test_counter* _test_info);

// parentheses handling tests
bool d_tests_sa_dmacro_remove_parentheses(struct d_test_counter* _test_info);

// argument selection module aggregator
bool d_tests_sa_dmacro_selection_all(struct d_test_counter* _test_info);


// =============================================================================
// VII. CORE ITERATION INFRASTRUCTURE TEST FUNCTIONS (Section VII of dmacro.h)
// =============================================================================

// map termination tests
bool d_tests_sa_dmacro_map_end_detection(struct d_test_counter* _test_info);

// core mapping tests
bool d_tests_sa_dmacro_map_internals(struct d_test_counter* _test_info);

// iteration infrastructure module aggregator
bool d_tests_sa_dmacro_iteration_core_all(struct d_test_counter* _test_info);


// =============================================================================
// VIII. FOR_EACH IMPLEMENTATIONS TEST FUNCTIONS (Section VIII of dmacro.h)
// =============================================================================

// basic iteration tests
bool d_tests_sa_dmacro_for_each_basic(struct d_test_counter* _test_info);

// separated iteration tests
bool d_tests_sa_dmacro_for_each_sep(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_for_each_comma(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_for_each_semicolon(struct d_test_counter* _test_info);

// data-passing iteration tests
bool d_tests_sa_dmacro_for_each_data(struct d_test_counter* _test_info);

// for_each module aggregator
bool d_tests_sa_dmacro_for_each_all(struct d_test_counter* _test_info);


// =============================================================================
// IX.  PAIR AND INDEXED ITERATION TEST FUNCTIONS (Section IX of dmacro.h)
// =============================================================================

// pair iteration tests
bool d_tests_sa_dmacro_for_each_pair(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_for_each_pair_sep(struct d_test_counter* _test_info);

// indexed iteration tests
bool d_tests_sa_dmacro_for_each_indexed(struct d_test_counter* _test_info);

// pair/indexed module aggregator
bool d_tests_sa_dmacro_pair_indexed_all(struct d_test_counter* _test_info);


// =============================================================================
// X.   MEMBER ACCESS ITERATION TEST FUNCTIONS (Section X of dmacro.h)
// =============================================================================

// pointer member access tests
bool d_tests_sa_dmacro_for_each_member_ptr(struct d_test_counter* _test_info);

// direct member access tests
bool d_tests_sa_dmacro_for_each_member_dot(struct d_test_counter* _test_info);

// generic operator access tests
bool d_tests_sa_dmacro_for_each_op(struct d_test_counter* _test_info);

// member access module aggregator
bool d_tests_sa_dmacro_member_access_all(struct d_test_counter* _test_info);


// =============================================================================
// XI.  ADVANCED ITERATION PATTERNS TEST FUNCTIONS (Section XI of dmacro.h)
// =============================================================================

// adjacent pair tests
bool d_tests_sa_dmacro_adjacent_pair(struct d_test_counter* _test_info);

// triple iteration tests
bool d_tests_sa_dmacro_for_each_triple(struct d_test_counter* _test_info);

// advanced iteration module aggregator
bool d_tests_sa_dmacro_advanced_iter_all(struct d_test_counter* _test_info);


// =============================================================================
// XII. CODE GENERATION UTILITIES TEST FUNCTIONS (Section XII of dmacro.h)
// =============================================================================

// struct generation tests
bool d_tests_sa_dmacro_make_struct(struct d_test_counter* _test_info);

// enum generation tests
bool d_tests_sa_dmacro_make_enum(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_make_enum_valued(struct d_test_counter* _test_info);

// function generation tests
bool d_tests_sa_dmacro_make_function(struct d_test_counter* _test_info);

// initializer generation tests
bool d_tests_sa_dmacro_make_initializer(struct d_test_counter* _test_info);

// switch case generation tests
bool d_tests_sa_dmacro_make_switch(struct d_test_counter* _test_info);

// code generation module aggregator
bool d_tests_sa_dmacro_codegen_all(struct d_test_counter* _test_info);


// =============================================================================
// XIII. FUNCTIONAL STYLE OPERATIONS TEST FUNCTIONS (Section XIII of dmacro.h)
// =============================================================================

// mapping operation tests
bool d_tests_sa_dmacro_map_transform(struct d_test_counter* _test_info);

// concatenation operation tests
bool d_tests_sa_dmacro_concat_all(struct d_test_counter* _test_info);
bool d_tests_sa_dmacro_cat_n(struct d_test_counter* _test_info);

// functional style module aggregator
bool d_tests_sa_dmacro_functional_all(struct d_test_counter* _test_info);


// =============================================================================
// XIV. POINTER ARRAY INITIALIZATION TEST FUNCTIONS (Section XIV of dmacro.h)
// =============================================================================

// data-comma iteration tests
bool d_tests_sa_dmacro_for_each_data_comma(struct d_test_counter* _test_info);

// struct array init tests
bool d_tests_sa_dmacro_struct_array_init(struct d_test_counter* _test_info);

// pointer array module aggregator
bool d_tests_sa_dmacro_ptr_array_all(struct d_test_counter* _test_info);


// =============================================================================
// XV.  UTILITY OPERATORS TEST FUNCTIONS (Section XV of dmacro.h)
// =============================================================================

// debug/test operator tests
bool d_tests_sa_dmacro_utility_ops(struct d_test_counter* _test_info);

// utility operators module aggregator
bool d_tests_sa_dmacro_utility_all(struct d_test_counter* _test_info);


// =============================================================================
// XVI. COMPILE-TIME ASSERTIONS TEST FUNCTIONS (Section XVI of dmacro.h)
// =============================================================================

// size/type check tests
bool d_tests_sa_dmacro_assert_same_size(struct d_test_counter* _test_info);

// compile-time assertions module aggregator
bool d_tests_sa_dmacro_static_assert_all(struct d_test_counter* _test_info);


// =============================================================================
// MASTER TEST SUITE
// =============================================================================

// d_tests_dmacro_run_all
//   function: runs all dmacro tests and returns the root test object.
struct d_test_object* d_tests_dmacro_run_all(void);

// d_tests_sa_dmacro_all
//   function: runs all dmacro tests and updates the counter.
bool d_tests_sa_dmacro_all(struct d_test_counter* _test_info);


#endif  // DJINTERP_TESTING_DMACRO_