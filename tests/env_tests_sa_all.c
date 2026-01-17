#include ".\env_tests_sa.h"


/*
d_tests_sa_env_build_all
  Runs all build configuration tests.
  Tests the following:
  - build configuration definition
  - build configuration values
  - build configuration consistency
  - build detection logic
  - build type string
  - conditional compilation
  - build configuration usage
*/
bool
d_tests_sa_env_build_all
(
    struct d_test_counter* _test_info
)
{
    struct d_test_counter module_counter;
    bool defined_result;
    bool values_result;
    bool consistency_result;
    bool detection_result;
    bool string_result;
    bool conditional_result;
    bool usage_result;
    bool overall_result;

    if (!_test_info)
    {
        return false;
    }

    module_counter = (struct d_test_counter){0, 0, 0, 0};

    printf("\n[MODULE] Testing Build Configuration\n");
    printf("========================================="
           "=======================================\n");

    defined_result      = d_tests_sa_env_build_config_defined(&module_counter);
    values_result       = d_tests_sa_env_build_config_values(&module_counter);
    consistency_result  = d_tests_sa_env_build_config_consistency(&module_counter);
    detection_result    = d_tests_sa_env_build_detection_logic(&module_counter);
    string_result       = d_tests_sa_env_build_type_string(&module_counter);
    conditional_result  = d_tests_sa_env_build_conditional_compilation(&module_counter);
    usage_result        = d_tests_sa_env_build_usage_examples(&module_counter);

    // update totals
    _test_info->assertions_total  += module_counter.assertions_total;
    _test_info->assertions_passed += module_counter.assertions_passed;
    _test_info->tests_total       += module_counter.tests_total;
    _test_info->tests_passed      += module_counter.tests_passed;

    overall_result = ( defined_result      &&
                       values_result       &&
                       consistency_result  &&
                       detection_result    &&
                       string_result       &&
                       conditional_result  &&
                       usage_result );

    printf("\n");

    if (overall_result)
    {
        printf("[PASS] Build Configuration Module: %zu/%zu assertions, %zu/%zu tests passed\n",
               module_counter.assertions_passed,
               module_counter.assertions_total,
               module_counter.tests_passed,
               module_counter.tests_total);
    }
    else
    {
        printf("[FAIL] Build Configuration Module: %zu/%zu assertions, %zu/%zu tests passed\n",
               module_counter.assertions_passed,
               module_counter.assertions_total,
               module_counter.tests_passed,
               module_counter.tests_total);

        printf("  - Configuration Definition:    %s\n",
               defined_result ? "PASSED" : "FAILED");
        printf("  - Configuration Values:        %s\n",
               values_result ? "PASSED" : "FAILED");
        printf("  - Configuration Consistency:   %s\n",
               consistency_result ? "PASSED" : "FAILED");
        printf("  - Detection Logic:             %s\n",
               detection_result ? "PASSED" : "FAILED");
        printf("  - Build Type String:           %s\n",
               string_result ? "PASSED" : "FAILED");
        printf("  - Conditional Compilation:     %s\n",
               conditional_result ? "PASSED" : "FAILED");
        printf("  - Usage Examples:              %s\n",
               usage_result ? "PASSED" : "FAILED");
    }

    return overall_result;
}