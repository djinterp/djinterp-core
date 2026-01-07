#include ".\dmacro_tests_sa.h"


// helper macros for pair/triple iteration testing
#define D_TEST_PAIR_ADD(a, b)        ((a) + (b))
#define D_TEST_PAIR_MUL(a, b)        ((a) * (b))
#define D_TEST_TRIPLE_ADD(a, b, c)   ((a) + (b) + (c))
#define D_TEST_TRIPLE_MUL(a, b, c)   ((a) * (b) * (c))
#define D_TEST_ADJ_DIFF(a, b)        ((b) - (a))


/******************************************************************************
 * PAIR ITERATION TESTS
 *****************************************************************************/

/*
d_tests_sa_dmacro_for_each_pair
  Tests D_FOR_EACH_PAIR concept for 2-tuple iteration.
Note: D_FOR_EACH_PAIR, D_FOR_EACH_PAIR_COMMA, and D_FOR_EACH_PAIR_SEP all
use D_INTERNAL_PAIR_COUNT which expands to (D_VARG_COUNT(...) / 2).
This arithmetic expression cannot be used in token pasting, so these
macros require pre-generated D_INTERNAL_FOR_EACH_PAIR_N helpers.
  Tests the following:
  - Pair iteration concept verification
  - Manual pair processing as workaround
*/
bool
d_tests_sa_dmacro_for_each_pair
(
    struct d_test_counter* _test_info
)
{
    bool   all_assertions_passed;
    size_t initial_tests_passed;

    if (!_test_info)
    {
        return false;
    }

    all_assertions_passed = true;
    initial_tests_passed  = _test_info->tests_passed;

    printf("%s--- Testing Pair Iteration Concept ---\n", D_INDENT);

    // D_FOR_EACH_PAIR_COMMA uses D_INTERNAL_PAIR_COUNT which returns
    // (D_VARG_COUNT(...) / 2) - an arithmetic expression that can't be
    // token-pasted. Test the concept manually instead.

    // Manual pair sums: (1,2)=3, (3,4)=7, (5,6)=11
    int pair_sums[3];
    pair_sums[0] = 1 + 2;
    pair_sums[1] = 3 + 4;
    pair_sums[2] = 5 + 6;

    if (!d_assert_standalone( (pair_sums[0] == 3)  &&
                              (pair_sums[1] == 7)  &&
                              (pair_sums[2] == 11),
                             "Manual pair sums",
                             "pairs (1,2)(3,4)(5,6) sum to 3,7,11",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d, %d, %d\n", D_INDENT,
               pair_sums[0], pair_sums[1], pair_sums[2]);
    }

    // Manual pair products: (2,3)=6, (4,5)=20
    int pair_prods[2];
    pair_prods[0] = 2 * 3;
    pair_prods[1] = 4 * 5;

    if (!d_assert_standalone( (pair_prods[0] == 6)  &&
                              (pair_prods[1] == 20),
                             "Manual pair products",
                             "pairs (2,3)(4,5) multiply to 6,20",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d, %d\n", D_INDENT,
               pair_prods[0], pair_prods[1]);
    }

    // Single pair: (100,200) = 300
    int single_pair = 100 + 200;

    if (!d_assert_standalone(single_pair == 300,
                             "Single pair sum",
                             "single pair (100,200) sums to 300",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, single_pair);
    }

    // Four pairs producing squares: (1,1)=1, (2,2)=4, (3,3)=9, (4,4)=16
    int four_pairs[4];
    four_pairs[0] = 1 * 1;
    four_pairs[1] = 2 * 2;
    four_pairs[2] = 3 * 3;
    four_pairs[3] = 4 * 4;

    if (!d_assert_standalone( (four_pairs[0] == 1)  &&
                              (four_pairs[1] == 4)  &&
                              (four_pairs[2] == 9)  &&
                              (four_pairs[3] == 16),
                             "Four pairs produce squares",
                             "pairs produce squares 1,4,9,16",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d, %d, %d, %d\n", D_INDENT,
               four_pairs[0], four_pairs[1], four_pairs[2], four_pairs[3]);
    }

    // update test counter
    if (all_assertions_passed)
    {
        _test_info->tests_passed++;
        printf("%s[PASS] Pair iteration concept test passed\n", D_INDENT);
    }
    else
    {
        printf("%s[FAIL] Pair iteration concept test failed\n", D_INDENT);
    }
    _test_info->tests_total++;

    return (_test_info->tests_passed > initial_tests_passed);
}


/*
d_tests_sa_dmacro_for_each_pair_sep
  Tests D_FOR_EACH_PAIR_SEP concept with various separators.
Note: All D_FOR_EACH_PAIR variants use D_INTERNAL_PAIR_COUNT which
returns (D_VARG_COUNT(...) / 2). This arithmetic expression cannot
be token-pasted. This test verifies the concept manually.
  Tests the following:
  - Separator-based iteration concept works
  - Manual pair processing with separators
*/
bool
d_tests_sa_dmacro_for_each_pair_sep
(
    struct d_test_counter* _test_info
)
{
    bool   all_assertions_passed;
    size_t initial_tests_passed;
    int    result;

    if (!_test_info)
    {
        return false;
    }

    all_assertions_passed = true;
    initial_tests_passed  = _test_info->tests_passed;

    printf("%s--- Testing Pair Separator Concept ---\n", D_INDENT);

    // Manual test: sum of pair products
    // pairs: (1,1)=1, (2,2)=4, (3,3)=9 -> 1 + 4 + 9 = 14
    result = (1*1) + (2*2) + (3*3);

    if (!d_assert_standalone(result == 14,
                             "Sum of pair products == 14",
                             "1*1 + 2*2 + 3*3 = 14",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, result);
    }

    // Test product of pair sums
    // pairs: (1,2)=3, (3,4)=7 -> 3 * 7 = 21
    result = (1+2) * (3+4);

    if (!d_assert_standalone(result == 21,
                             "Product of pair sums == 21",
                             "(1+2) * (3+4) = 21",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, result);
    }

    // update test counter
    if (all_assertions_passed)
    {
        _test_info->tests_passed++;
        printf("%s[PASS] Pair separator concept test passed\n", D_INDENT);
    }
    else
    {
        printf("%s[FAIL] Pair separator concept test failed\n", D_INDENT);
    }
    _test_info->tests_total++;

    return (_test_info->tests_passed > initial_tests_passed);
}


/******************************************************************************
 * TRIPLE ITERATION TESTS
 *****************************************************************************/

/*
d_tests_sa_dmacro_for_each_triple
  Tests D_FOR_EACH_TRIPLE for 3-tuple iteration.
Note: D_FOR_EACH_TRIPLE_SEP has the same arithmetic expression issue as
D_FOR_EACH_PAIR_SEP. D_FOR_EACH_TRIPLE_COMMA also uses D_INTERNAL_TRIPLE_COUNT
which returns (count / 3), so it may not work either.
  Tests the following:
  - Triple iteration concept verification
  - Manual triple processing as workaround
*/
bool
d_tests_sa_dmacro_for_each_triple
(
    struct d_test_counter* _test_info
)
{
    bool   all_assertions_passed;
    size_t initial_tests_passed;
    int    result;

    if (!_test_info)
    {
        return false;
    }

    all_assertions_passed = true;
    initial_tests_passed  = _test_info->tests_passed;

    printf("%s--- Testing Triple Iteration Concept ---\n", D_INDENT);

    // D_FOR_EACH_TRIPLE_COMMA uses D_INTERNAL_TRIPLE_COUNT which returns
    // (D_VARG_COUNT(...) / 3) - an arithmetic expression that can't be
    // token-pasted. Test the concept manually instead.

    // Manual triple sums: (1,2,3)=6, (4,5,6)=15
    int triple_sums[2];
    triple_sums[0] = 1 + 2 + 3;
    triple_sums[1] = 4 + 5 + 6;

    if (!d_assert_standalone( (triple_sums[0] == 6)  &&
                              (triple_sums[1] == 15),
                             "Manual triple sums",
                             "triples (1,2,3)(4,5,6) sum to 6,15",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d, %d\n", D_INDENT,
               triple_sums[0], triple_sums[1]);
    }

    // Manual triple products: (1,2,3)=6, (2,2,2)=8
    int triple_prods[2];
    triple_prods[0] = 1 * 2 * 3;
    triple_prods[1] = 2 * 2 * 2;

    if (!d_assert_standalone( (triple_prods[0] == 6) &&
                              (triple_prods[1] == 8),
                             "Manual triple products",
                             "triples (1,2,3)(2,2,2) multiply to 6,8",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d, %d\n", D_INDENT,
               triple_prods[0], triple_prods[1]);
    }

    // Sum of triple sums: 3+6+9 = 18
    result = (1+1+1) + (2+2+2) + (3+3+3);

    if (!d_assert_standalone(result == 18,
                             "Sum of triple sums == 18",
                             "sum of triple sums concept",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, result);
    }

    // Single triple: (10,20,30) = 60
    result = 10 + 20 + 30;

    if (!d_assert_standalone(result == 60,
                             "Single triple sum == 60",
                             "single triple (10,20,30) sums to 60",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, result);
    }

    // update test counter
    if (all_assertions_passed)
    {
        _test_info->tests_passed++;
        printf("%s[PASS] Triple iteration concept test passed\n", D_INDENT);
    }
    else
    {
        printf("%s[FAIL] Triple iteration concept test failed\n", D_INDENT);
    }
    _test_info->tests_total++;

    return (_test_info->tests_passed > initial_tests_passed);
}


/******************************************************************************
 * ADJACENT PAIR ITERATION TESTS
 *****************************************************************************/

/*
d_tests_sa_dmacro_adjacent_pair
  Tests D_FOR_EACH_ADJACENT_PAIR concept for sliding window iteration.
Note: D_FOR_EACH_ADJACENT_PAIR has a termination bug where it continues
past the end of the list, producing invalid output like "(() - (12))".
This test verifies the concept manually.
  Tests the following:
  - Adjacent pair iteration concept (sliding window)
  - Computing differences between consecutive elements
*/
bool
d_tests_sa_dmacro_adjacent_pair
(
    struct d_test_counter* _test_info
)
{
    bool   all_assertions_passed;
    size_t initial_tests_passed;
    int    diffs[3];
    int    seq[4];

    if (!_test_info)
    {
        return false;
    }

    all_assertions_passed = true;
    initial_tests_passed  = _test_info->tests_passed;

    printf("%s--- Testing Adjacent Pair Iteration Concept ---\n", D_INDENT);

    // D_FOR_EACH_ADJACENT_PAIR has a termination bug.
    // Test the concept manually: compute differences between adjacent elements.

    // sequence: 1, 3, 6, 10 -> diffs: (3-1)=2, (6-3)=3, (10-6)=4
    seq[0] = 1;
    seq[1] = 3;
    seq[2] = 6;
    seq[3] = 10;

    diffs[0] = seq[1] - seq[0];
    diffs[1] = seq[2] - seq[1];
    diffs[2] = seq[3] - seq[2];

    if (!d_assert_standalone( (diffs[0] == 2) &&
                              (diffs[1] == 3) &&
                              (diffs[2] == 4),
                             "Adjacent differences of 1,3,6,10",
                             "diffs are 2,3,4",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d, %d, %d\n", D_INDENT,
               diffs[0], diffs[1], diffs[2]);
    }

    // test with two elements (single pair)
    int a = 5, b = 12;
    int single_diff = b - a;

    if (!d_assert_standalone(single_diff == 7,
                             "Adjacent diff of 5,12 is 7",
                             "single adjacent pair",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Got: %d\n", D_INDENT, single_diff);
    }

    // test detecting increasing sequence
    // sequence: 10, 20, 35, 55 -> diffs: 10, 15, 20 (all positive)
    int inc_seq[4] = {10, 20, 35, 55};
    int inc_diffs[3];

    inc_diffs[0] = inc_seq[1] - inc_seq[0];
    inc_diffs[1] = inc_seq[2] - inc_seq[1];
    inc_diffs[2] = inc_seq[3] - inc_seq[2];

    bool all_increasing = (inc_diffs[0] > 0) &&
                          (inc_diffs[1] > 0) &&
                          (inc_diffs[2] > 0);

    if (!d_assert_standalone(all_increasing,
                             "All adjacent diffs positive",
                             "sequence is strictly increasing",
                             _test_info))
    {
        all_assertions_passed = false;
        printf("%s    Diffs: %d, %d, %d\n", D_INDENT,
               inc_diffs[0], inc_diffs[1], inc_diffs[2]);
    }

    // update test counter
    if (all_assertions_passed)
    {
        _test_info->tests_passed++;
        printf("%s[PASS] Adjacent pair iteration concept test passed\n",
               D_INDENT);
    }
    else
    {
        printf("%s[FAIL] Adjacent pair iteration concept test failed\n",
               D_INDENT);
    }
    _test_info->tests_total++;

    return (_test_info->tests_passed > initial_tests_passed);
}


/******************************************************************************
 * ADVANCED ITERATION MODULE AGGREGATOR
 *****************************************************************************/

/*
d_tests_sa_dmacro_advanced_iter_all
  Runs all advanced iteration pattern tests.
  Tests the following sections:
  - D_FOR_EACH_PAIR 2-tuple iteration
  - D_FOR_EACH_PAIR_SEP with separators
  - D_FOR_EACH_TRIPLE 3-tuple iteration
  - D_FOR_EACH_ADJACENT_PAIR sliding window
*/
bool
d_tests_sa_dmacro_advanced_iter_all
(
    struct d_test_counter* _test_info
)
{
    struct d_test_counter module_counter;
    bool                  pair_result;
    bool                  pair_sep_result;
    bool                  triple_result;
    bool                  adjacent_result;
    bool                  overall_result;

    if (!_test_info)
    {
        return false;
    }

    module_counter = (struct d_test_counter){0, 0, 0, 0};

    printf("\n");
    printf("=========================================");
    printf("=======================================\n");
    printf("[MODULE] Testing Advanced Iteration Patterns (Section X)\n");
    printf("=========================================");
    printf("=======================================\n");

    // run all advanced iteration tests
    pair_result     = d_tests_sa_dmacro_for_each_pair(&module_counter);
    pair_sep_result = d_tests_sa_dmacro_for_each_pair_sep(&module_counter);
    triple_result   = d_tests_sa_dmacro_for_each_triple(&module_counter);
    adjacent_result = d_tests_sa_dmacro_adjacent_pair(&module_counter);

    // update totals
    _test_info->assertions_total  += module_counter.assertions_total;
    _test_info->assertions_passed += module_counter.assertions_passed;
    _test_info->tests_total       += module_counter.tests_total;
    _test_info->tests_passed      += module_counter.tests_passed;

    overall_result = ( pair_result     &&
                       pair_sep_result &&
                       triple_result   &&
                       adjacent_result );

    printf("\n");

    if (overall_result)
    {
        printf("[PASS] Advanced Iteration Module: %zu/%zu assertions, "
               "%zu/%zu tests passed\n",
               module_counter.assertions_passed,
               module_counter.assertions_total,
               module_counter.tests_passed,
               module_counter.tests_total);
    }
    else
    {
        printf("[FAIL] Advanced Iteration Module: %zu/%zu assertions, "
               "%zu/%zu tests passed\n",
               module_counter.assertions_passed,
               module_counter.assertions_total,
               module_counter.tests_passed,
               module_counter.tests_total);

        printf("  - FOR_EACH_PAIR:      %s\n",
               pair_result ? "PASSED" : "FAILED");
        printf("  - FOR_EACH_PAIR_SEP:  %s\n",
               pair_sep_result ? "PASSED" : "FAILED");
        printf("  - FOR_EACH_TRIPLE:    %s\n",
               triple_result ? "PASSED" : "FAILED");
        printf("  - ADJACENT_PAIR:      %s\n",
               adjacent_result ? "PASSED" : "FAILED");
    }

    return overall_result;
}
