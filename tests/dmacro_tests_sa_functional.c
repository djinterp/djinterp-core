#include ".\dmacro_tests_sa.h"


/******************************************************************************
 * CONCAT_ALL TESTS
 *****************************************************************************/

/*
d_tests_sa_dmacro_concat_all
  Tests D_CONCAT_ALL and D_INTERNAL_CONCAT_ALL_HELPER macros.
Note: D_CONCAT_ALL is not available on MSVC traditional preprocessor.
  Tests the following:
  - D_CONCAT_ALL concatenates multiple tokens into one
  - Works with 2, 3, 4+ tokens
  - Result can be used as identifier
*/
bool
d_tests_sa_dmacro_concat_all
(
    struct d_test_counter* _test_info
)
{
    bool        all_assertions_passed;
    size_t      initial_tests_passed;

#if !D_DMACRO_IS_MSVC_COMPAT
    const char* str_result;
    int         val;
#endif

    if (!_test_info)
    {
        return false;
    }

    all_assertions_passed = true;
    initial_tests_passed  = _test_info->tests_passed;

    printf("%s--- Testing D_CONCAT_ALL ---\n", D_INDENT);

#if D_DMACRO_IS_MSVC_COMPAT
    // D_CONCAT_ALL is not available on MSVC traditional preprocessor
    printf("%s    (D_CONCAT_ALL not available on MSVC - skipped)\n", D_INDENT);

    if (!d_assert_standalone(1,
                             "D_CONCAT_ALL skipped on MSVC",
                             "use D_CAT2/3/4/5 instead",
                             _test_info))
    {
        all_assertions_passed = false;
    }
#else
    // test D_CONCAT_ALL with 2 tokens
    // D_CONCAT_ALL(hello, world) -> helloworld
    #define helloworld "concatenated_2"
    str_result = D_STRINGIFY(D_CONCAT_ALL(hello, world));

    if (!d_assert_standalone(strcmp(str_result, "helloworld") == 0,
                             "D_CONCAT_ALL(hello, world) == \"helloworld\"",
                             "two token concatenation",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: \"%s\"\n", D_INDENT, str_result);
    }
    #undef helloworld

    // test D_CONCAT_ALL with 3 tokens
    // D_CONCAT_ALL(a, b, c) -> abc
    str_result = D_STRINGIFY(D_CONCAT_ALL(a, b, c));

    if (!d_assert_standalone(strcmp(str_result, "abc") == 0,
                             "D_CONCAT_ALL(a, b, c) == \"abc\"",
                             "three token concatenation",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: \"%s\"\n", D_INDENT, str_result);
    }

    // test D_CONCAT_ALL with 4 tokens
    str_result = D_STRINGIFY(D_CONCAT_ALL(one, two, three, four));

    if (!d_assert_standalone(strcmp(str_result, "onetwothreefour") == 0,
                             "D_CONCAT_ALL(one,two,three,four)",
                             "four token concatenation",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: \"%s\"\n", D_INDENT, str_result);
    }

    // test D_CONCAT_ALL creating a usable identifier
    #define test_func 42
    val = D_CONCAT_ALL(test, _, func);

    if (!d_assert_standalone(val == 42,
                             "D_CONCAT_ALL creates usable identifier",
                             "concatenated tokens form valid identifier",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, val);
    }
    #undef test_func
#endif  // !D_DMACRO_IS_MSVC_COMPAT

    // update test counter
    if (all_assertions_passed)
    {
        _test_info->tests_passed++;
        printf("%s[PASS] D_CONCAT_ALL test passed\n", D_INDENT);
    }
    else
    {
        printf("%s[FAIL] D_CONCAT_ALL test failed\n", D_INDENT);
    }
    _test_info->tests_total++;

    return (_test_info->tests_passed > initial_tests_passed);
}


/******************************************************************************
 * CAT_N TESTS (D_CAT2, D_CAT3, D_CAT4, D_CAT5)
 *****************************************************************************/

/*
d_tests_sa_dmacro_cat_n
  Tests D_CAT2, D_CAT3, D_CAT4, D_CAT5 convenience macros.
  Tests the following:
  - D_CAT2 concatenates exactly 2 tokens
  - D_CAT3 concatenates exactly 3 tokens
  - D_CAT4 concatenates exactly 4 tokens
  - D_CAT5 concatenates exactly 5 tokens
  - All produce valid identifiers
*/
bool
d_tests_sa_dmacro_cat_n
(
    struct d_test_counter* _test_info
)
{
    bool        all_assertions_passed;
    size_t      initial_tests_passed;
    const char* str_result;
    int         val;

    if (!_test_info)
    {
        return false;
    }

    all_assertions_passed = true;
    initial_tests_passed  = _test_info->tests_passed;

    printf("%s--- Testing D_CAT2/3/4/5 ---\n", D_INDENT);

    // test D_CAT2
    str_result = D_STRINGIFY(D_CAT2(hello, world));

    if (!d_assert_standalone(strcmp(str_result, "helloworld") == 0,
                             "D_CAT2(hello, world) == \"helloworld\"",
                             "CAT2 concatenates 2 tokens",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: \"%s\"\n", D_INDENT, str_result);
    }

    // test D_CAT3
    str_result = D_STRINGIFY(D_CAT3(one, two, three));

    if (!d_assert_standalone(strcmp(str_result, "onetwothree") == 0,
                             "D_CAT3(one, two, three) == \"onetwothree\"",
                             "CAT3 concatenates 3 tokens",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: \"%s\"\n", D_INDENT, str_result);
    }

    // test D_CAT4
    str_result = D_STRINGIFY(D_CAT4(a, b, c, d));

    if (!d_assert_standalone(strcmp(str_result, "abcd") == 0,
                             "D_CAT4(a, b, c, d) == \"abcd\"",
                             "CAT4 concatenates 4 tokens",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: \"%s\"\n", D_INDENT, str_result);
    }

    // test D_CAT5
    str_result = D_STRINGIFY(D_CAT5(v, w, x, y, z));

    if (!d_assert_standalone(strcmp(str_result, "vwxyz") == 0,
                             "D_CAT5(v, w, x, y, z) == \"vwxyz\"",
                             "CAT5 concatenates 5 tokens",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: \"%s\"\n", D_INDENT, str_result);
    }

    // test CAT macros creating usable identifiers
    #define prefix_suffix 100
    val = D_CAT2(prefix, _suffix);

    if (!d_assert_standalone(val == 100,
                             "D_CAT2 creates usable identifier",
                             "prefix_suffix should resolve to 100",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, val);
    }
    #undef prefix_suffix

    // test CAT3 with underscores for namespacing
    #define my_module_init 200
    val = D_CAT3(my, _module, _init);

    if (!d_assert_standalone(val == 200,
                             "D_CAT3 creates namespaced identifier",
                             "my_module_init should resolve to 200",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, val);
    }
    #undef my_module_init

    // test CAT4 building complex identifier
    #define d_array_push_back 300
    val = D_CAT4(d, _array, _push, _back);

    if (!d_assert_standalone(val == 300,
                             "D_CAT4 creates complex identifier",
                             "d_array_push_back should resolve to 300",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, val);
    }
    #undef d_array_push_back

    // test CAT5 building very complex identifier  
    #define d_hash_map_get_value 400
    val = D_CAT5(d, _hash, _map, _get, _value);

    if (!d_assert_standalone(val == 400,
                             "D_CAT5 creates complex identifier",
                             "d_hash_map_get_value should resolve to 400",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, val);
    }
    #undef d_hash_map_get_value

    // update test counter
    if (all_assertions_passed)
    {
        _test_info->tests_passed++;
        printf("%s[PASS] D_CAT2/3/4/5 test passed\n", D_INDENT);
    }
    else
    {
        printf("%s[FAIL] D_CAT2/3/4/5 test failed\n", D_INDENT);
    }
    _test_info->tests_total++;

    return (_test_info->tests_passed > initial_tests_passed);
}


/******************************************************************************
 * FUNCTIONAL STYLE MODULE AGGREGATOR
 *****************************************************************************/

/*
d_tests_sa_dmacro_functional_all
  Runs all functional style operation tests.
  Tests the following sections:
  - D_CONCAT_ALL for multi-token concatenation
  - D_CAT2/3/4/5 for fixed-argument concatenation
*/
bool
d_tests_sa_dmacro_functional_all
(
    struct d_test_counter* _test_info
)
{
    struct d_test_counter module_counter;
    bool                  concat_all_result;
    bool                  cat_n_result;
    bool                  overall_result;

    if (!_test_info)
    {
        return false;
    }

    module_counter = (struct d_test_counter){0, 0, 0, 0};

    printf("\n");
    printf("=========================================");
    printf("=======================================\n");
    printf("[MODULE] Testing Functional Style Operations (Section XII)\n");
    printf("=========================================");
    printf("=======================================\n");

    // run all functional style tests
    concat_all_result = d_tests_sa_dmacro_concat_all(&module_counter);
    cat_n_result      = d_tests_sa_dmacro_cat_n(&module_counter);

    // update totals
    _test_info->assertions_total  += module_counter.assertions_total;
    _test_info->assertions_passed += module_counter.assertions_passed;
    _test_info->tests_total       += module_counter.tests_total;
    _test_info->tests_passed      += module_counter.tests_passed;

    overall_result = (concat_all_result && cat_n_result);

    printf("\n");

    if (overall_result)
    {
        printf("[PASS] Functional Style Module: %zu/%zu assertions, "
               "%zu/%zu tests passed\n",
               module_counter.assertions_passed,
               module_counter.assertions_total,
               module_counter.tests_passed,
               module_counter.tests_total);
    }
    else
    {
        printf("[FAIL] Functional Style Module: %zu/%zu assertions, "
               "%zu/%zu tests passed\n",
               module_counter.assertions_passed,
               module_counter.assertions_total,
               module_counter.tests_passed,
               module_counter.tests_total);

        printf("  - D_CONCAT_ALL:  %s\n", concat_all_result ? "PASSED" : "FAILED");
        printf("  - D_CAT2/3/4/5:  %s\n", cat_n_result ? "PASSED" : "FAILED");
    }

    return overall_result;
}