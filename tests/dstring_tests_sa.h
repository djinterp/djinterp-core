/******************************************************************************
* djinterp [test]                                             dstring_tests_sa.h
*
*   Unit test declarations for the d_string safe string module. Tests are
* organized by functional category matching the sections in dstring.h.
* Each section has its own .c implementation file to keep test definitions
* manageable.
*
* TABLE OF CONTENTS
* =================
* I.   CREATION & DESTRUCTION TESTS     (dstring_tests_creation.c)
* II.  CAPACITY MANAGEMENT TESTS        (dstring_tests_capacity.c)
* III. ACCESS FUNCTION TESTS            (dstring_tests_access.c)
* IV.  SAFE COPY TESTS                  (dstring_tests_copy.c)
* V.   CONCATENATION TESTS              (dstring_tests_concat.c)
* VI.  DUPLICATION TESTS                (dstring_tests_dup.c)
* VII. COMPARISON TESTS                 (dstring_tests_compare.c)
* VIII.SEARCH TESTS                     (dstring_tests_search.c)
* IX.  MODIFICATION TESTS               (dstring_tests_modify.c)
* X.   CASE CONVERSION TESTS            (dstring_tests_case.c)
* XI.  REVERSAL TESTS                   (dstring_tests_reverse.c)
* XII. TRIMMING TESTS                   (dstring_tests_trim.c)
* XIII.TOKENIZATION TESTS               (dstring_tests_token.c)
* XIV. JOIN TESTS                       (dstring_tests_join.c)
* XV.  UTILITY TESTS                    (dstring_tests_util.c)
* XVI. ERROR STRING TESTS               (dstring_tests_error.c)
* XVII.FORMATTED STRING TESTS           (dstring_tests_format.c)
*
* path:      \inc\test\dstring_tests_sa.h
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.12.30
******************************************************************************/

#ifndef DJINTERP_DSTRING_TESTS_SA_
#define DJINTERP_DSTRING_TESTS_SA_ 1

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "..\dstring.h"
#include ".\test_standalone.h"


/******************************************************************************
* Test Suite Configuration
******************************************************************************/

// DSTRING_TEST_VERBOSE
//   definition: when set to 1, enables verbose output during test execution.
#ifndef DSTRING_TEST_VERBOSE
    #define DSTRING_TEST_VERBOSE 0
#endif


/******************************************************************************
* I. CREATION & DESTRUCTION TESTS
******************************************************************************/

// test_dstring_new
//   function: tests d_string_new() - creates empty string with default capacity.
struct d_test_object* test_dstring_new(void);

// test_dstring_new_with_capacity
//   function: tests d_string_new_with_capacity() - creates empty string with
// specified initial capacity.
struct d_test_object* test_dstring_new_with_capacity(void);

// test_dstring_new_from_cstr
//   function: tests d_string_new_from_cstr() - creates string from C string.
struct d_test_object* test_dstring_new_from_cstr(void);

// test_dstring_new_from_cstr_n
//   function: tests d_string_new_from_cstr_n() - creates string from C string
// with maximum length.
struct d_test_object* test_dstring_new_from_cstr_n(void);

// test_dstring_new_from_buffer
//   function: tests d_string_new_from_buffer() - creates string from raw
// buffer with specified length.
struct d_test_object* test_dstring_new_from_buffer(void);

// test_dstring_new_copy
//   function: tests d_string_new_copy() - creates deep copy of existing string.
struct d_test_object* test_dstring_new_copy(void);

// test_dstring_new_fill
//   function: tests d_string_new_fill() - creates string filled with character.
struct d_test_object* test_dstring_new_fill(void);

// test_dstring_new_formatted
//   function: tests d_string_new_formatted() - creates string using printf
// format specifiers.
struct d_test_object* test_dstring_new_formatted(void);

// test_dstring_free
//   function: tests d_string_free() - frees string and its contents.
struct d_test_object* test_dstring_free(void);

// test_dstring_free_contents
//   function: tests d_string_free_contents() - frees string contents but not
// the structure itself.
struct d_test_object* test_dstring_free_contents(void);

// test_dstring_creation_all
//   function: runs all creation and destruction tests, returns aggregate
// test object containing all results.
struct d_test_object* test_dstring_creation_all(void);


/******************************************************************************
* II. CAPACITY MANAGEMENT TESTS
******************************************************************************/

struct d_test_object* test_dstring_reserve(void);
struct d_test_object* test_dstring_shrink_to_fit(void);
struct d_test_object* test_dstring_capacity(void);
struct d_test_object* test_dstring_resize(void);
struct d_test_object* test_dstring_capacity_all(void);


/******************************************************************************
* III. ACCESS FUNCTION TESTS
******************************************************************************/

struct d_test_object* test_dstring_length(void);
struct d_test_object* test_dstring_size(void);
struct d_test_object* test_dstring_cstr(void);
struct d_test_object* test_dstring_data(void);
struct d_test_object* test_dstring_is_empty(void);
struct d_test_object* test_dstring_char_at(void);
struct d_test_object* test_dstring_set_char(void);
struct d_test_object* test_dstring_front(void);
struct d_test_object* test_dstring_back(void);
struct d_test_object* test_dstring_access_all(void);


/******************************************************************************
* IV. SAFE COPY TESTS
******************************************************************************/

struct d_test_object* test_dstring_copy_s(void);
struct d_test_object* test_dstring_copy_cstr_s(void);
struct d_test_object* test_dstring_ncopy_s(void);
struct d_test_object* test_dstring_ncopy_cstr_s(void);
struct d_test_object* test_dstring_to_buffer_s(void);
struct d_test_object* test_dstring_copy_all(void);


/******************************************************************************
* V. CONCATENATION TESTS
******************************************************************************/

struct d_test_object* test_dstring_cat_s(void);
struct d_test_object* test_dstring_cat_cstr_s(void);
struct d_test_object* test_dstring_ncat_s(void);
struct d_test_object* test_dstring_ncat_cstr_s(void);
struct d_test_object* test_dstring_concat_all(void);


/******************************************************************************
* VI. DUPLICATION TESTS
******************************************************************************/

struct d_test_object* test_dstring_dup(void);
struct d_test_object* test_dstring_ndup(void);
struct d_test_object* test_dstring_substr(void);
struct d_test_object* test_dstring_dup_all(void);


/******************************************************************************
* VII. COMPARISON TESTS
******************************************************************************/

struct d_test_object* test_dstring_cmp(void);
struct d_test_object* test_dstring_cmp_cstr(void);
struct d_test_object* test_dstring_ncmp(void);
struct d_test_object* test_dstring_ncmp_cstr(void);
struct d_test_object* test_dstring_casecmp(void);
struct d_test_object* test_dstring_casecmp_cstr(void);
struct d_test_object* test_dstring_ncasecmp(void);
struct d_test_object* test_dstring_ncasecmp_cstr(void);
struct d_test_object* test_dstring_equals(void);
struct d_test_object* test_dstring_equals_cstr(void);
struct d_test_object* test_dstring_equals_ignore_case(void);
struct d_test_object* test_dstring_equals_cstr_ignore_case(void);
struct d_test_object* test_dstring_compare_all(void);


/******************************************************************************
* VIII. SEARCH TESTS
******************************************************************************/

struct d_test_object* test_dstring_find_char(void);
struct d_test_object* test_dstring_find_char_from(void);
struct d_test_object* test_dstring_rfind_char(void);
struct d_test_object* test_dstring_chr(void);
struct d_test_object* test_dstring_rchr(void);
struct d_test_object* test_dstring_chrnul(void);
struct d_test_object* test_dstring_find(void);
struct d_test_object* test_dstring_find_cstr(void);
struct d_test_object* test_dstring_find_from(void);
struct d_test_object* test_dstring_find_cstr_from(void);
struct d_test_object* test_dstring_rfind(void);
struct d_test_object* test_dstring_rfind_cstr(void);
struct d_test_object* test_dstring_str(void);
struct d_test_object* test_dstring_casefind(void);
struct d_test_object* test_dstring_casefind_cstr(void);
struct d_test_object* test_dstring_casestr(void);
struct d_test_object* test_dstring_contains(void);
struct d_test_object* test_dstring_contains_cstr(void);
struct d_test_object* test_dstring_contains_char(void);
struct d_test_object* test_dstring_starts_with(void);
struct d_test_object* test_dstring_starts_with_cstr(void);
struct d_test_object* test_dstring_ends_with(void);
struct d_test_object* test_dstring_ends_with_cstr(void);
struct d_test_object* test_dstring_spn(void);
struct d_test_object* test_dstring_cspn(void);
struct d_test_object* test_dstring_pbrk(void);
struct d_test_object* test_dstring_search_all(void);


/******************************************************************************
* IX. MODIFICATION TESTS
******************************************************************************/

struct d_test_object* test_dstring_assign(void);
struct d_test_object* test_dstring_assign_cstr(void);
struct d_test_object* test_dstring_assign_buffer(void);
struct d_test_object* test_dstring_assign_char(void);
struct d_test_object* test_dstring_append(void);
struct d_test_object* test_dstring_append_cstr(void);
struct d_test_object* test_dstring_append_buffer(void);
struct d_test_object* test_dstring_append_char(void);
struct d_test_object* test_dstring_append_formatted(void);
struct d_test_object* test_dstring_prepend(void);
struct d_test_object* test_dstring_prepend_cstr(void);
struct d_test_object* test_dstring_prepend_char(void);
struct d_test_object* test_dstring_insert(void);
struct d_test_object* test_dstring_insert_cstr(void);
struct d_test_object* test_dstring_insert_char(void);
struct d_test_object* test_dstring_erase(void);
struct d_test_object* test_dstring_erase_char(void);
struct d_test_object* test_dstring_clear(void);
struct d_test_object* test_dstring_replace(void);
struct d_test_object* test_dstring_replace_cstr(void);
struct d_test_object* test_dstring_replace_all(void);
struct d_test_object* test_dstring_replace_all_cstr(void);
struct d_test_object* test_dstring_replace_char(void);
struct d_test_object* test_dstring_modify_all(void);


/******************************************************************************
* X. CASE CONVERSION TESTS
******************************************************************************/

struct d_test_object* test_dstring_to_lower(void);
struct d_test_object* test_dstring_to_upper(void);
struct d_test_object* test_dstring_lower(void);
struct d_test_object* test_dstring_upper(void);
struct d_test_object* test_dstring_case_all(void);


/******************************************************************************
* XI. REVERSAL TESTS
******************************************************************************/

struct d_test_object* test_dstring_reverse(void);
struct d_test_object* test_dstring_reversed(void);
struct d_test_object* test_dstring_reversal_all(void);


/******************************************************************************
* XII. TRIMMING TESTS
******************************************************************************/

struct d_test_object* test_dstring_trim(void);
struct d_test_object* test_dstring_trim_left(void);
struct d_test_object* test_dstring_trim_right(void);
struct d_test_object* test_dstring_trim_chars(void);
struct d_test_object* test_dstring_trimmed(void);
struct d_test_object* test_dstring_trimmed_left(void);
struct d_test_object* test_dstring_trimmed_right(void);
struct d_test_object* test_dstring_trim_all(void);


/******************************************************************************
* XIII. TOKENIZATION TESTS
******************************************************************************/

struct d_test_object* test_dstring_tokenize(void);
struct d_test_object* test_dstring_split(void);
struct d_test_object* test_dstring_split_free(void);
struct d_test_object* test_dstring_token_all(void);


/******************************************************************************
* XIV. JOIN TESTS
******************************************************************************/

struct d_test_object* test_dstring_join(void);
struct d_test_object* test_dstring_join_cstr(void);
struct d_test_object* test_dstring_concat(void);
struct d_test_object* test_dstring_join_all(void);


/******************************************************************************
* XV. UTILITY TESTS
******************************************************************************/

struct d_test_object* test_dstring_is_valid(void);
struct d_test_object* test_dstring_is_ascii(void);
struct d_test_object* test_dstring_is_numeric(void);
struct d_test_object* test_dstring_is_alpha(void);
struct d_test_object* test_dstring_is_alnum(void);
struct d_test_object* test_dstring_is_whitespace(void);
struct d_test_object* test_dstring_count_char(void);
struct d_test_object* test_dstring_count_substr(void);
struct d_test_object* test_dstring_hash(void);
struct d_test_object* test_dstring_util_all(void);


/******************************************************************************
* XVI. ERROR STRING TESTS
******************************************************************************/

struct d_test_object* test_dstring_error(void);
struct d_test_object* test_dstring_error_r(void);
struct d_test_object* test_dstring_error_all(void);


/******************************************************************************
* XVII. FORMATTED STRING TESTS
******************************************************************************/

struct d_test_object* test_dstring_printf(void);
struct d_test_object* test_dstring_vprintf(void);
struct d_test_object* test_dstring_sprintf(void);
struct d_test_object* test_dstring_format_all(void);


/******************************************************************************
* MASTER TEST RUNNER
******************************************************************************/

// test_dstring_all
//   function: runs all d_string unit tests and returns aggregate results.
struct d_test_object* test_dstring_all(void);


#endif  // DJINTERP_DSTRING_TESTS_SA_
