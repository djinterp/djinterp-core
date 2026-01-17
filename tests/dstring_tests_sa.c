#include "dstring_tests_sa.h"


/*
d_tests_sa_strcpy_s
  Tests the `d_strcpy_s` function which safely copies strings with bounds checking.
  Tests the following:
  - successful string copying
  - destination buffer size validation
  - null parameter handling
  - source string length validation
  - buffer overflow protection
*/
bool
d_tests_sa_strcpy_s
(
    struct d_test_counter* _test_info
)
{
    bool   test_result;
    char   dest[20];
    int    result;
    size_t i;

    test_result = true;

    printf("Testing `d_strcpy_s`...\n");

    // Test 1: Successful string copy
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strcpy_s(dest, sizeof(dest), "Hello");

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hello") == 0),
        "`d_strcpy_s` successfully copies valid string",
        "`d_strcpy_s` failed to copy valid string",
        _test_info
    );

    // Test 2: NULL destination parameter
    result = d_strcpy_s(NULL, sizeof(dest), "Hello");

    test_result &= d_assert_standalone(
        result == EINVAL,
        "`d_strcpy_s` correctly handles NULL destination",
        "`d_strcpy_s` failed to handle NULL destination",
        _test_info
    );

    // Test 3: Zero destination size
    result = d_strcpy_s(dest, 0, "Hello");

    test_result &= d_assert_standalone(
        result == ERANGE,
        "`d_strcpy_s` correctly handles zero destination size",
        "`d_strcpy_s` failed to handle zero destination size",
        _test_info
    );

    // Test 4: NULL source parameter
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strcpy_s(dest, sizeof(dest), NULL);

    test_result &= d_assert_standalone(
        (result == EINVAL) && (dest[0] == '\0'),
        "`d_strcpy_s` correctly handles NULL source and clears destination",
        "`d_strcpy_s` failed to handle NULL source",
        _test_info
    );

    // Test 5: Source string too long for destination
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strcpy_s(dest, 5, "This string is too long");

    test_result &= d_assert_standalone(
        (result == ERANGE) && (dest[0] == '\0'),
        "`d_strcpy_s` correctly handles buffer overflow and clears destination",
        "`d_strcpy_s` failed to handle buffer overflow",
        _test_info
    );

    // Test 6: Empty source string
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strcpy_s(dest, sizeof(dest), "");

    test_result &= d_assert_standalone(
        (result == 0) && (dest[0] == '\0'),
        "`d_strcpy_s` correctly handles empty source string",
        "`d_strcpy_s` failed to handle empty source string",
        _test_info
    );

    // Test 7: Exact fit (including null terminator)
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strcpy_s(dest, 6, "Hello");

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hello") == 0),
        "`d_strcpy_s` correctly handles exact buffer fit",
        "`d_strcpy_s` failed with exact buffer fit",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
d_tests_sa_strncpy_s
  Tests the `d_strncpy_s` function which safely copies bounded strings.
  Tests the following:
  - successful bounded string copying
  - count parameter validation
  - null parameter handling
  - destination size validation
  - proper null termination
*/
bool
d_tests_sa_strncpy_s
(
    struct d_test_counter* _test_info
)
{
    bool   test_result;
    char   dest[20];
    int    result;
    size_t i;

    test_result = true;

    printf("Testing `d_strncpy_s`...\n");

    // Test 1: Successful bounded copy
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strncpy_s(dest, sizeof(dest), "Hello World", 5);

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hello") == 0),
        "`d_strncpy_s` successfully copies bounded string",
        "`d_strncpy_s` failed to copy bounded string",
        _test_info
    );

    // Test 2: Count larger than source length
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strncpy_s(dest, sizeof(dest), "Hi", 10);

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hi") == 0),
        "`d_strncpy_s` handles count larger than source length",
        "`d_strncpy_s` failed with large count",
        _test_info
    );

    // Test 3: NULL destination parameter
    result = d_strncpy_s(NULL, sizeof(dest), "Hello", 5);

    test_result &= d_assert_standalone(
        result == EINVAL,
        "`d_strncpy_s` correctly handles NULL destination",
        "`d_strncpy_s` failed to handle NULL destination",
        _test_info
    );

    // Test 4: Zero destination size
    result = d_strncpy_s(dest, 0, "Hello", 5);

    test_result &= d_assert_standalone(
        result == ERANGE,
        "`d_strncpy_s` correctly handles zero destination size",
        "`d_strncpy_s` failed to handle zero destination size",
        _test_info
    );

    // Test 5: NULL source parameter
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strncpy_s(dest, sizeof(dest), NULL, 5);

    test_result &= d_assert_standalone(
        (result == EINVAL) && (dest[0] == '\0'),
        "`d_strncpy_s` correctly handles NULL source and clears destination",
        "`d_strncpy_s` failed to handle NULL source",
        _test_info
    );

    // Test 6: Result too long for destination
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strncpy_s(dest, 3, "Hello", 5);

    test_result &= d_assert_standalone(
        (result == ERANGE) && (dest[0] == '\0'),
        "`d_strncpy_s` correctly handles buffer overflow and clears destination",
        "`d_strncpy_s` failed to handle buffer overflow",
        _test_info
    );

    // Test 7: Zero count
    for (i = 0; i < sizeof(dest); i++)
    {
        dest[i] = 'X';
    }

    result = d_strncpy_s(dest, sizeof(dest), "Hello", 0);

    test_result &= d_assert_standalone(
        (result == 0) && (dest[0] == '\0'),
        "`d_strncpy_s` correctly handles zero count",
        "`d_strncpy_s` failed to handle zero count",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
d_tests_sa_strcat_s
  Tests the `d_strcat_s` function which safely concatenates strings.
  Tests the following:
  - successful string concatenation
  - destination buffer size validation
  - null parameter handling
  - existing destination string handling
  - buffer overflow protection
*/
bool
d_tests_sa_strcat_s
(
    struct d_test_counter* _test_info
)
{
    bool   test_result;
    char   dest[20];
    int    result;
    size_t i;

    test_result = true;

    printf("Testing `d_strcat_s`...\n");

    // Test 1: Successful string concatenation
    d_strcpy_s(dest, sizeof(dest), "Hello");
    result = d_strcat_s(dest, sizeof(dest), " World");

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hello World") == 0),
        "`d_strcat_s` successfully concatenates strings",
        "`d_strcat_s` failed to concatenate strings",
        _test_info
    );

    // Test 2: NULL destination parameter
    result = d_strcat_s(NULL, sizeof(dest), "Hello");

    test_result &= d_assert_standalone(
        result == EINVAL,
        "`d_strcat_s` correctly handles NULL destination",
        "`d_strcat_s` failed to handle NULL destination",
        _test_info
    );

    // Test 3: NULL source parameter
    d_strcpy_s(dest, sizeof(dest), "Hello");
    result = d_strcat_s(dest, sizeof(dest), NULL);

    test_result &= d_assert_standalone(
        (result == EINVAL) && (dest[0] == '\0'),
        "`d_strcat_s` correctly handles NULL source and clears destination",
        "`d_strcat_s` failed to handle NULL source",
        _test_info
    );

    // Test 4: Zero destination size
    result = d_strcat_s(dest, 0, "Hello");

    test_result &= d_assert_standalone(
        result == ERANGE,
        "`d_strcat_s` correctly handles zero destination size",
        "`d_strcat_s` failed to handle zero destination size",
        _test_info
    );

    // Test 5: Buffer overflow protection
    d_strcpy_s(dest, sizeof(dest), "Very long string");
    result = d_strcat_s(dest, sizeof(dest), " that will overflow");

    test_result &= d_assert_standalone(
        (result == ERANGE) && (dest[0] == '\0'),
        "`d_strcat_s` correctly handles buffer overflow and clears destination",
        "`d_strcat_s` failed to handle buffer overflow",
        _test_info
    );

    // Test 6: Empty source string
    d_strcpy_s(dest, sizeof(dest), "Hello");
    result = d_strcat_s(dest, sizeof(dest), "");

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hello") == 0),
        "`d_strcat_s` correctly handles empty source string",
        "`d_strcat_s` failed to handle empty source string",
        _test_info
    );

    // Test 7: Empty destination string
    dest[0] = '\0';
    result = d_strcat_s(dest, sizeof(dest), "Hello");

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hello") == 0),
        "`d_strcat_s` correctly handles empty destination string",
        "`d_strcat_s` failed to handle empty destination string",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
d_tests_sa_strncat_s
  Tests the `d_strncat_s` function which safely concatenates bounded strings.
  Tests the following:
  - successful bounded string concatenation
  - count parameter validation
  - destination buffer size validation
  - null parameter handling
  - buffer overflow protection
*/
bool
d_tests_sa_strncat_s
(
    struct d_test_counter* _test_info
)
{
    bool   test_result;
    char   dest[20];
    int    result;
    size_t i;

    test_result = true;

    printf("Testing `d_strncat_s`...\n");

    // Test 1: Successful bounded concatenation
    d_strcpy_s(dest, sizeof(dest), "Hello");
    result = d_strncat_s(dest, sizeof(dest), " World!", 6);

    test_result &= d_assert_standalone(
        ( (result == 0) &&
          (strcmp(dest, "Hello World") == 0) ),
        "`d_strncat_s` successfully concatenates bounded string",
        "`d_strncat_s` failed to concatenate bounded string",
        _test_info
    );

    // Test 2: Count larger than source length
    d_strcpy_s(dest, sizeof(dest), "Hello");
    result = d_strncat_s(dest, sizeof(dest), " Hi", 10);

    test_result &= d_assert_standalone(
        ( (result == 0) &&
          (strcmp(dest, "Hello Hi") == 0) ),
        "`d_strncat_s` handles count larger than source length",
        "`d_strncat_s` failed with large count",
        _test_info
    );

    // Test 3: NULL destination parameter
    result = d_strncat_s(NULL, sizeof(dest), "Hello", 5);

    test_result &= d_assert_standalone(
        result == EINVAL,
        "`d_strncat_s` correctly handles NULL destination",
        "`d_strncat_s` failed to handle NULL destination",
        _test_info
    );

    // Test 4: NULL source parameter
    d_strcpy_s(dest, sizeof(dest), "Hello");
    result = d_strncat_s(dest, sizeof(dest), NULL, 5);

    test_result &= d_assert_standalone(
        (result == EINVAL) && (dest[0] == '\0'),
        "`d_strncat_s` correctly handles NULL source and clears destination",
        "`d_strncat_s` failed to handle NULL source",
        _test_info
    );

    // Test 5: Zero destination size
    result = d_strncat_s(dest, 0, "Hello", 5);

    test_result &= d_assert_standalone(
        result == ERANGE,
        "`d_strncat_s` correctly handles zero destination size",
        "`d_strncat_s` failed to handle zero destination size",
        _test_info
    );

    // Test 6: Buffer overflow protection
    d_strcpy_s(dest, sizeof(dest), "Long string");
    result = d_strncat_s(dest, sizeof(dest), " overflow test", 20);

    test_result &= d_assert_standalone(
        (result == ERANGE) && (dest[0] == '\0'),
        "`d_strncat_s` correctly handles buffer overflow and clears destination",
        "`d_strncat_s` failed to handle buffer overflow",
        _test_info
    );

    // Test 7: Zero count
    d_strcpy_s(dest, sizeof(dest), "Hello");
    result = d_strncat_s(dest, sizeof(dest), " World", 0);

    test_result &= d_assert_standalone(
        (result == 0) && (strcmp(dest, "Hello") == 0),
        "`d_strncat_s` correctly handles zero count",
        "`d_strncat_s` failed to handle zero count",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        STRING DUPLICATION FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strdup
  Tests the `d_strdup` function which duplicates strings by allocating memory.
  Tests the following:
  - successful string duplication
  - null parameter handling
  - empty string duplication
  - memory allocation validation
*/
bool
d_tests_sa_strdup
(
    struct d_test_counter* _test_info
)
{
    bool  test_result;
    char* result;

    test_result = true;

    printf("Testing `d_strdup`...\n");

    // Test 1: Successful string duplication
    result = d_strdup("Hello World");

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "Hello World") == 0),
        "`d_strdup` successfully duplicates string",
        "`d_strdup` failed to duplicate string",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    // Test 2: NULL source parameter
    result = d_strdup(NULL);

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strdup` correctly handles NULL source",
        "`d_strdup` failed to handle NULL source",
        _test_info
    );

    // Test 3: Empty string duplication
    result = d_strdup("");

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "") == 0),
        "`d_strdup` correctly duplicates empty string",
        "`d_strdup` failed to duplicate empty string",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    // Test 4: Long string duplication
    result = d_strdup("This is a much longer string that tests the allocation of larger memory blocks");

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "This is a much longer string that tests the allocation of larger memory blocks") == 0),
        "`d_strdup` correctly duplicates long string",
        "`d_strdup` failed to duplicate long string",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    // Test 5: Single character string
    result = d_strdup("X");

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "X") == 0),
        "`d_strdup` correctly duplicates single character",
        "`d_strdup` failed to duplicate single character",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
d_tests_sa_strndup
  Tests the `d_strndup` function which duplicates bounded strings.
  Tests the following:
  - successful bounded string duplication
  - count parameter validation
  - null parameter handling
  - count larger than source length
  - proper null termination
*/
bool
d_tests_sa_strndup
(
    struct d_test_counter* _test_info
)
{
    bool  test_result;
    char* result;

    test_result = true;

    printf("Testing `d_strndup`...\n");

    // Test 1: Successful bounded duplication
    result = d_strndup("Hello World", 5);

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "Hello") == 0),
        "`d_strndup` successfully duplicates bounded string",
        "`d_strndup` failed to duplicate bounded string",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    // Test 2: Count larger than source length
    result = d_strndup("Hi", 10);

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "Hi") == 0),
        "`d_strndup` handles count larger than source length",
        "`d_strndup` failed with large count",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    // Test 3: NULL source parameter
    result = d_strndup(NULL, 5);

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strndup` correctly handles NULL source",
        "`d_strndup` failed to handle NULL source",
        _test_info
    );

    // Test 4: Zero count
    result = d_strndup("Hello", 0);

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "") == 0),
        "`d_strndup` correctly handles zero count",
        "`d_strndup` failed to handle zero count",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    // Test 5: Full string duplication (count equals length)
    result = d_strndup("Hello", 5);

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "Hello") == 0),
        "`d_strndup` correctly duplicates full string",
        "`d_strndup` failed to duplicate full string",
        _test_info
    );

    if (result)
    {
        free(result);
        result = NULL;
    }

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                    CASE-INSENSITIVE COMPARISON FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strcasecmp
  Tests the `d_strcasecmp` function which compares strings ignoring case.
  Tests the following:
  - case-insensitive equal strings
  - case-insensitive unequal strings
  - null parameter handling
  - mixed case comparisons
  - special character handling
*/
bool
d_tests_sa_strcasecmp
(
    struct d_test_counter* _test_info
)
{
    bool test_result;
    int  result;

    test_result = true;

    printf("Testing `d_strcasecmp`...\n");

    // Test 1: Case-insensitive equal strings
    result = d_strcasecmp("Hello", "hello");

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strcasecmp` correctly identifies case-insensitive equal strings",
        "`d_strcasecmp` failed with case-insensitive equal strings",
        _test_info
    );

    // Test 2: Case-insensitive unequal strings (first < second)
    result = d_strcasecmp("Apple", "BANANA");

    test_result &= d_assert_standalone(
        result < 0,
        "`d_strcasecmp` correctly orders strings (first < second)",
        "`d_strcasecmp` failed with string ordering (first < second)",
        _test_info
    );

    // Test 3: Case-insensitive unequal strings (first > second)
    result = d_strcasecmp("ZEBRA", "apple");

    test_result &= d_assert_standalone(
        result > 0,
        "`d_strcasecmp` correctly orders strings (first > second)",
        "`d_strcasecmp` failed with string ordering (first > second)",
        _test_info
    );

    // Test 4: Both strings NULL
    result = d_strcasecmp(NULL, NULL);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strcasecmp` correctly handles both NULL strings",
        "`d_strcasecmp` failed with both NULL strings",
        _test_info
    );

    // Test 5: First string NULL
    result = d_strcasecmp(NULL, "Hello");

    test_result &= d_assert_standalone(
        result < 0,
        "`d_strcasecmp` correctly handles NULL first string",
        "`d_strcasecmp` failed with NULL first string",
        _test_info
    );

    // Test 6: Second string NULL
    result = d_strcasecmp("Hello", NULL);

    test_result &= d_assert_standalone(
        result > 0,
        "`d_strcasecmp` correctly handles NULL second string",
        "`d_strcasecmp` failed with NULL second string",
        _test_info
    );

    // Test 7: Mixed case comparison
    result = d_strcasecmp("HeLLo WoRLd", "hello WORLD");

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strcasecmp` correctly handles mixed case comparison",
        "`d_strcasecmp` failed with mixed case comparison",
        _test_info
    );

    // Test 8: Empty strings
    result = d_strcasecmp("", "");

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strcasecmp` correctly handles empty strings",
        "`d_strcasecmp` failed with empty strings",
        _test_info
    );

    // Test 9: One empty, one non-empty
    result = d_strcasecmp("", "Hello");

    test_result &= d_assert_standalone(
        result < 0,
        "`d_strcasecmp` correctly handles empty vs non-empty string",
        "`d_strcasecmp` failed with empty vs non-empty string",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
d_tests_sa_strncasecmp
  Tests the `d_strncasecmp` function which compares bounded strings ignoring case.
  Tests the following:
  - bounded case-insensitive comparison
  - count parameter validation
  - null parameter handling
  - zero count handling
  - partial string comparison
*/
bool
d_tests_sa_strncasecmp
(
    struct d_test_counter* _test_info
)
{
    bool test_result;
    int  result;

    test_result = true;

    printf("Testing `d_strncasecmp`...\n");

    // Test 1: Bounded case-insensitive equal comparison
    result = d_strncasecmp("Hello World", "hello world", 5);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strncasecmp` correctly compares bounded case-insensitive strings",
        "`d_strncasecmp` failed with bounded case-insensitive comparison",
        _test_info
    );

    // Test 2: Bounded comparison with difference beyond count
    result = d_strncasecmp("Hello ABC", "hello XYZ", 5);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strncasecmp` ignores differences beyond count",
        "`d_strncasecmp` failed to ignore differences beyond count",
        _test_info
    );

    // Test 3: Bounded comparison with difference within count
    result = d_strncasecmp("Apple", "BANANA", 1);

    test_result &= d_assert_standalone(
        result < 0,
        "`d_strncasecmp` detects differences within count",
        "`d_strncasecmp` failed to detect differences within count",
        _test_info
    );

    // Test 4: Zero count
    result = d_strncasecmp("Different", "Strings", 0);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strncasecmp` correctly handles zero count",
        "`d_strncasecmp` failed to handle zero count",
        _test_info
    );

    // Test 5: Both strings NULL
    result = d_strncasecmp(NULL, NULL, 5);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strncasecmp` correctly handles both NULL strings",
        "`d_strncasecmp` failed with both NULL strings",
        _test_info
    );

    // Test 6: First string NULL
    result = d_strncasecmp(NULL, "Hello", 5);

    test_result &= d_assert_standalone(
        result < 0,
        "`d_strncasecmp` correctly handles NULL first string",
        "`d_strncasecmp` failed with NULL first string",
        _test_info
    );

    // Test 7: Second string NULL
    result = d_strncasecmp("Hello", NULL, 5);

    test_result &= d_assert_standalone(
        result > 0,
        "`d_strncasecmp` correctly handles NULL second string",
        "`d_strncasecmp` failed with NULL second string",
        _test_info
    );

    // Test 8: Count larger than string lengths
    result = d_strncasecmp("Hi", "hi", 10);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strncasecmp` correctly handles count larger than string length",
        "`d_strncasecmp` failed with large count",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        TOKENIZATION FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strtok_r
  Tests the `d_strtok_r` function which provides thread-safe string tokenization.
  Tests the following:
  - successful string tokenization
  - multiple delimiter handling
  - null parameter handling
  - empty token handling
  - continuing tokenization
*/
bool
d_tests_sa_strtok_r
(
    struct d_test_counter* _test_info
)
{
    bool  test_result;
    char  str[] = "Hello,World;Test:String";
    char  str2[] = "One::Two:::Three";
    char  str3[] = "NoDelimiters";
    char  str4[] = ",,,";
    char* token;
    char* saveptr;

    test_result = true;

    printf("Testing `d_strtok_r`...\n");

    // Test 1: Basic tokenization
    token = d_strtok_r(str, ",;:", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "Hello") == 0),
        "`d_strtok_r` correctly returns first token",
        "`d_strtok_r` failed to return first token",
        _test_info
    );

    // continue tokenization
    token = d_strtok_r(NULL, ",;:", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "World") == 0),
        "`d_strtok_r` correctly returns second token",
        "`d_strtok_r` failed to return second token",
        _test_info
    );

    // continue tokenization
    token = d_strtok_r(NULL, ",;:", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "Test") == 0),
        "`d_strtok_r` correctly returns third token",
        "`d_strtok_r` failed to return third token",
        _test_info
    );

    // continue tokenization
    token = d_strtok_r(NULL, ",;:", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "String") == 0),
        "`d_strtok_r` correctly returns fourth token",
        "`d_strtok_r` failed to return fourth token",
        _test_info
    );

    // no more tokens
    token = d_strtok_r(NULL, ",;:", &saveptr);

    test_result &= d_assert_standalone(
        token == NULL,
        "`d_strtok_r` correctly returns NULL when no more tokens",
        "`d_strtok_r` failed to return NULL when no more tokens",
        _test_info
    );

    // Test 2: Multiple consecutive delimiters
    token = d_strtok_r(str2, ":", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "One") == 0),
        "`d_strtok_r` handles multiple consecutive delimiters (first token)",
        "`d_strtok_r` failed with multiple consecutive delimiters (first token)",
        _test_info
    );

    token = d_strtok_r(NULL, ":", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "Two") == 0),
        "`d_strtok_r` handles multiple consecutive delimiters (second token)",
        "`d_strtok_r` failed with multiple consecutive delimiters (second token)",
        _test_info
    );

    token = d_strtok_r(NULL, ":", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "Three") == 0),
        "`d_strtok_r` handles multiple consecutive delimiters (third token)",
        "`d_strtok_r` failed with multiple consecutive delimiters (third token)",
        _test_info
    );

    // Test 3: String with no delimiters
    token = d_strtok_r(str3, ",;:", &saveptr);

    test_result &= d_assert_standalone(
        (token != NULL) && (strcmp(token, "NoDelimiters") == 0),
        "`d_strtok_r` correctly handles string with no delimiters",
        "`d_strtok_r` failed with string with no delimiters",
        _test_info
    );

    token = d_strtok_r(NULL, ",;:", &saveptr);

    test_result &= d_assert_standalone(
        token == NULL,
        "`d_strtok_r` returns NULL after single token with no delimiters",
        "`d_strtok_r` failed to return NULL after single token",
        _test_info
    );

    // Test 4: String with only delimiters
    token = d_strtok_r(str4, ",", &saveptr);

    test_result &= d_assert_standalone(
        token == NULL,
        "`d_strtok_r` correctly handles string with only delimiters",
        "`d_strtok_r` failed with string containing only delimiters",
        _test_info
    );

    // Test 5: NULL delimiters parameter
    token = d_strtok_r(str, NULL, &saveptr);

    test_result &= d_assert_standalone(
        token == NULL,
        "`d_strtok_r` correctly handles NULL delimiters",
        "`d_strtok_r` failed to handle NULL delimiters",
        _test_info
    );

    // Test 6: NULL saveptr parameter
    token = d_strtok_r(str, ",", NULL);

    test_result &= d_assert_standalone(
        token == NULL,
        "`d_strtok_r` correctly handles NULL saveptr",
        "`d_strtok_r` failed to handle NULL saveptr",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        LENGTH CALCULATION FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strnlen
  Tests the `d_strnlen` function which calculates string length with a maximum limit.
  Tests the following:
  - normal string length calculation
  - maximum length enforcement
  - null parameter handling
  - empty string handling
  - strings shorter than maximum
*/
bool
d_tests_sa_strnlen
(
    struct d_test_counter* _test_info
)
{
    bool   test_result;
    size_t result;

    test_result = true;

    printf("Testing `d_strnlen`...\n");

    // Test 1: Normal string shorter than maximum
    result = d_strnlen("Hello", 10);

    test_result &= d_assert_standalone(
        result == 5,
        "`d_strnlen` correctly calculates length for string shorter than maximum",
        "`d_strnlen` failed with string shorter than maximum",
        _test_info
    );

    // Test 2: String longer than maximum
    result = d_strnlen("Hello World", 5);

    test_result &= d_assert_standalone(
        result == 5,
        "`d_strnlen` correctly enforces maximum length",
        "`d_strnlen` failed to enforce maximum length",
        _test_info
    );

    // Test 3: String exactly equal to maximum
    result = d_strnlen("Hello", 5);

    test_result &= d_assert_standalone(
        result == 5,
        "`d_strnlen` correctly handles string equal to maximum length",
        "`d_strnlen` failed with string equal to maximum length",
        _test_info
    );

    // Test 4: NULL string parameter
    result = d_strnlen(NULL, 10);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strnlen` correctly handles NULL string",
        "`d_strnlen` failed to handle NULL string",
        _test_info
    );

    // Test 5: Empty string
    result = d_strnlen("", 10);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strnlen` correctly handles empty string",
        "`d_strnlen` failed to handle empty string",
        _test_info
    );

    // Test 6: Zero maximum length
    result = d_strnlen("Hello", 0);

    test_result &= d_assert_standalone(
        result == 0,
        "`d_strnlen` correctly handles zero maximum length",
        "`d_strnlen` failed to handle zero maximum length",
        _test_info
    );

    // Test 7: Single character string
    result = d_strnlen("X", 10);

    test_result &= d_assert_standalone(
        result == 1,
        "`d_strnlen` correctly handles single character string",
        "`d_strnlen` failed with single character string",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        SEARCH FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strcasestr
  Tests the `d_strcasestr` function which finds substrings ignoring case.
  Tests the following:
  - case-insensitive substring search
  - null parameter handling
  - substring not found
  - empty substring handling
  - substring at beginning and end
*/
bool
d_tests_sa_strcasestr
(
    struct d_test_counter* _test_info
)
{
    bool  test_result;
    char* result;

    test_result = true;

    printf("Testing `d_strcasestr`...\n");

    // Test 1: Case-insensitive substring found
    result = d_strcasestr("Hello World", "WOR");

    test_result &= d_assert_standalone(
        (result != NULL) && (strncmp(result, "World", 5) == 0),
        "`d_strcasestr` correctly finds case-insensitive substring",
        "`d_strcasestr` failed to find case-insensitive substring",
        _test_info
    );

    // Test 2: Substring not found
    result = d_strcasestr("Hello World", "xyz");

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strcasestr` correctly returns NULL when substring not found",
        "`d_strcasestr` failed to return NULL when substring not found",
        _test_info
    );

    // Test 3: NULL haystack parameter
    result = d_strcasestr(NULL, "test");

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strcasestr` correctly handles NULL haystack",
        "`d_strcasestr` failed to handle NULL haystack",
        _test_info
    );

    // Test 4: NULL needle parameter
    result = d_strcasestr("Hello World", NULL);

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strcasestr` correctly handles NULL needle",
        "`d_strcasestr` failed to handle NULL needle",
        _test_info
    );

    // Test 5: Empty needle
    result = d_strcasestr("Hello World", "");

    test_result &= d_assert_standalone(
        result != NULL,
        "`d_strcasestr` correctly handles empty needle",
        "`d_strcasestr` failed to handle empty needle",
        _test_info
    );

    // Test 6: Substring at beginning
    result = d_strcasestr("Hello World", "HEL");

    test_result &= d_assert_standalone(
        (result != NULL) && (strncmp(result, "Hello", 5) == 0),
        "`d_strcasestr` correctly finds substring at beginning",
        "`d_strcasestr` failed to find substring at beginning",
        _test_info
    );

    // Test 7: Substring at end
    result = d_strcasestr("Hello World", "RLD");

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "rld") == 0),
        "`d_strcasestr` correctly finds substring at end",
        "`d_strcasestr` failed to find substring at end",
        _test_info
    );

    // Test 8: Exact match
    result = d_strcasestr("Hello", "HELLO");

    test_result &= d_assert_standalone(
        (result != NULL) && (strcmp(result, "Hello") == 0),
        "`d_strcasestr` correctly handles exact match with different case",
        "`d_strcasestr` failed with exact match",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        CASE CONVERSION FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strlwr
  Tests the `d_strlwr` function which converts strings to lowercase.
  Tests the following:
  - successful lowercase conversion
  - mixed case handling
  - null parameter handling
  - already lowercase strings
  - special characters and numbers
*/
bool
d_tests_sa_strlwr
(
    struct d_test_counter* _test_info
)
{
    bool test_result;
    char str[50];
    char* result;

    test_result = true;

    printf("Testing `d_strlwr`...\n");

    // Test 1: Mixed case string conversion
    d_strcpy_s(str, sizeof(str), "Hello WORLD");
    result = d_strlwr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "hello world") == 0),
        "`d_strlwr` correctly converts mixed case string to lowercase",
        "`d_strlwr` failed to convert mixed case string",
        _test_info
    );

    // Test 2: Already lowercase string
    d_strcpy_s(str, sizeof(str), "already lowercase");
    result = d_strlwr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "already lowercase") == 0),
        "`d_strlwr` correctly handles already lowercase string",
        "`d_strlwr` failed with already lowercase string",
        _test_info
    );

    // Test 3: All uppercase string
    d_strcpy_s(str, sizeof(str), "ALL UPPERCASE");
    result = d_strlwr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "all uppercase") == 0),
        "`d_strlwr` correctly converts all uppercase string",
        "`d_strlwr` failed to convert all uppercase string",
        _test_info
    );

    // Test 4: NULL parameter
    result = d_strlwr(NULL);

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strlwr` correctly handles NULL parameter",
        "`d_strlwr` failed to handle NULL parameter",
        _test_info
    );

    // Test 5: Empty string
    d_strcpy_s(str, sizeof(str), "");
    result = d_strlwr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "") == 0),
        "`d_strlwr` correctly handles empty string",
        "`d_strlwr` failed to handle empty string",
        _test_info
    );

    // Test 6: String with numbers and special characters
    d_strcpy_s(str, sizeof(str), "Hello123!@#");
    result = d_strlwr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "hello123!@#") == 0),
        "`d_strlwr` correctly handles string with numbers and special characters",
        "`d_strlwr` failed with numbers and special characters",
        _test_info
    );

    // Test 7: Single character
    d_strcpy_s(str, sizeof(str), "A");
    result = d_strlwr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "a") == 0),
        "`d_strlwr` correctly converts single uppercase character",
        "`d_strlwr` failed with single character",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
d_tests_sa_strupr
  Tests the `d_strupr` function which converts strings to uppercase.
  Tests the following:
  - successful uppercase conversion
  - mixed case handling
  - null parameter handling
  - already uppercase strings
  - special characters and numbers
*/
bool
d_tests_sa_strupr
(
    struct d_test_counter* _test_info
)
{
    bool test_result;
    char str[50];
    char* result;

    test_result = true;

    printf("Testing `d_strupr`...\n");

    // Test 1: Mixed case string conversion
    d_strcpy_s(str, sizeof(str), "Hello world");
    result = d_strupr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "HELLO WORLD") == 0),
        "`d_strupr` correctly converts mixed case string to uppercase",
        "`d_strupr` failed to convert mixed case string",
        _test_info
    );

    // Test 2: Already uppercase string
    d_strcpy_s(str, sizeof(str), "ALREADY UPPERCASE");
    result = d_strupr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "ALREADY UPPERCASE") == 0),
        "`d_strupr` correctly handles already uppercase string",
        "`d_strupr` failed with already uppercase string",
        _test_info
    );

    // Test 3: All lowercase string
    d_strcpy_s(str, sizeof(str), "all lowercase");
    result = d_strupr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "ALL LOWERCASE") == 0),
        "`d_strupr` correctly converts all lowercase string",
        "`d_strupr` failed to convert all lowercase string",
        _test_info
    );

    // Test 4: NULL parameter
    result = d_strupr(NULL);

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strupr` correctly handles NULL parameter",
        "`d_strupr` failed to handle NULL parameter",
        _test_info
    );

    // Test 5: Empty string
    d_strcpy_s(str, sizeof(str), "");
    result = d_strupr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "") == 0),
        "`d_strupr` correctly handles empty string",
        "`d_strupr` failed to handle empty string",
        _test_info
    );

    // Test 6: String with numbers and special characters
    d_strcpy_s(str, sizeof(str), "hello123!@#");
    result = d_strupr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "HELLO123!@#") == 0),
        "`d_strupr` correctly handles string with numbers and special characters",
        "`d_strupr` failed with numbers and special characters",
        _test_info
    );

    // Test 7: Single character
    d_strcpy_s(str, sizeof(str), "a");
    result = d_strupr(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "A") == 0),
        "`d_strupr` correctly converts single lowercase character",
        "`d_strupr` failed with single character",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        MANIPULATION FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strrev
  Tests the `d_strrev` function which reverses strings in place.
  Tests the following:
  - successful string reversal
  - empty string handling
  - null parameter handling
  - single character strings
  - even and odd length strings
*/
bool
d_tests_sa_strrev
(
    struct d_test_counter* _test_info
)
{
    bool test_result;
    char str[50];
    char* result;

    test_result = true;

    printf("Testing `d_strrev`...\n");

    // Test 1: Normal string reversal
    d_strcpy_s(str, sizeof(str), "Hello");
    result = d_strrev(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "olleH") == 0),
        "`d_strrev` correctly reverses normal string",
        "`d_strrev` failed to reverse normal string",
        _test_info
    );

    // Test 2: Even length string
    d_strcpy_s(str, sizeof(str), "abcd");
    result = d_strrev(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "dcba") == 0),
        "`d_strrev` correctly reverses even length string",
        "`d_strrev` failed to reverse even length string",
        _test_info
    );

    // Test 3: Odd length string
    d_strcpy_s(str, sizeof(str), "abcde");
    result = d_strrev(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "edcba") == 0),
        "`d_strrev` correctly reverses odd length string",
        "`d_strrev` failed to reverse odd length string",
        _test_info
    );

    // Test 4: Single character string
    d_strcpy_s(str, sizeof(str), "A");
    result = d_strrev(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "A") == 0),
        "`d_strrev` correctly handles single character string",
        "`d_strrev` failed with single character string",
        _test_info
    );

    // Test 5: Empty string
    d_strcpy_s(str, sizeof(str), "");
    result = d_strrev(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "") == 0),
        "`d_strrev` correctly handles empty string",
        "`d_strrev` failed to handle empty string",
        _test_info
    );

    // Test 6: NULL parameter
    result = d_strrev(NULL);

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strrev` correctly handles NULL parameter",
        "`d_strrev` failed to handle NULL parameter",
        _test_info
    );

    // Test 7: Palindrome string
    d_strcpy_s(str, sizeof(str), "racecar");
    result = d_strrev(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "racecar") == 0),
        "`d_strrev` correctly reverses palindrome",
        "`d_strrev` failed to reverse palindrome",
        _test_info
    );

    // Test 8: String with spaces and special characters
    d_strcpy_s(str, sizeof(str), "a b!c");
    result = d_strrev(str);

    test_result &= d_assert_standalone(
        (result == str) && (strcmp(str, "c!b a") == 0),
        "`d_strrev` correctly handles string with spaces and special characters",
        "`d_strrev` failed with spaces and special characters",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        CHARACTER SEARCH FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strchrnul
  Tests the `d_strchrnul` function which finds characters or returns string end.
  Tests the following:
  - successful character finding
  - character not found (returns end)
  - null parameter handling
  - null character searching
  - multiple occurrences
*/
bool
d_tests_sa_strchrnul
(
    struct d_test_counter* _test_info
)
{
    bool  test_result;
    char* result;
    const char test_str[] = "Hello World";

    test_result = true;

    printf("Testing `d_strchrnul`...\n");

    // Test 1: Character found
    result = d_strchrnul(test_str, 'o');

    test_result &= d_assert_standalone(
        (result != NULL) && (*result == 'o') && (result == &test_str[4]),
        "`d_strchrnul` correctly finds existing character",
        "`d_strchrnul` failed to find existing character",
        _test_info
    );

    // Test 2: Character not found (returns pointer to end)
    result = d_strchrnul(test_str, 'x');

    test_result &= d_assert_standalone(
        (result != NULL) && (*result == '\0') && (result == &test_str[strlen(test_str)]),
        "`d_strchrnul` correctly returns pointer to end when character not found",
        "`d_strchrnul` failed to return end pointer when character not found",
        _test_info
    );

    // Test 3: NULL string parameter
    result = d_strchrnul(NULL, 'o');

    test_result &= d_assert_standalone(
        result == NULL,
        "`d_strchrnul` correctly handles NULL string parameter",
        "`d_strchrnul` failed to handle NULL string parameter",
        _test_info
    );

    // Test 4: Searching for null terminator
    result = d_strchrnul(test_str, '\0');

    test_result &= d_assert_standalone(
        (result != NULL) && (*result == '\0') && (result == &test_str[strlen(test_str)]),
        "`d_strchrnul` correctly finds null terminator",
        "`d_strchrnul` failed to find null terminator",
        _test_info
    );

    // Test 5: Character at beginning
    result = d_strchrnul(test_str, 'H');

    test_result &= d_assert_standalone(
        (result != NULL) && (*result == 'H') && (result == &test_str[0]),
        "`d_strchrnul` correctly finds character at beginning",
        "`d_strchrnul` failed to find character at beginning",
        _test_info
    );

    // Test 6: Character at end (before null terminator)
    result = d_strchrnul(test_str, 'd');

    test_result &= d_assert_standalone(
        (result != NULL) && (*result == 'd') && (result == &test_str[10]),
        "`d_strchrnul` correctly finds character at end",
        "`d_strchrnul` failed to find character at end",
        _test_info
    );

    // Test 7: Empty string
    result = d_strchrnul("", 'a');

    test_result &= d_assert_standalone(
        (result != NULL) && (*result == '\0'),
        "`d_strchrnul` correctly handles empty string",
        "`d_strchrnul` failed to handle empty string",
        _test_info
    );

    // Test 8: Space character
    result = d_strchrnul(test_str, ' ');

    test_result &= d_assert_standalone(
        (result != NULL) && (*result == ' ') && (result == &test_str[5]),
        "`d_strchrnul` correctly finds space character",
        "`d_strchrnul` failed to find space character",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        ERROR HANDLING FUNCTION TESTS
===============================================================================
*/

/*
d_tests_sa_strerror_r
  Tests the `d_strerror_r` function which provides thread-safe error strings.
  Tests the following:
  - successful error message generation
  - buffer size validation
  - null parameter handling
  - known and unknown error numbers
  - buffer overflow protection
*/
bool
d_tests_sa_strerror_r
(
    struct d_test_counter* _test_info
)
{
    bool test_result;
    char buffer[50];
    int  result;

    test_result = true;

    printf("Testing `d_strerror_r`...\n");

    // Test 1: Valid error number with sufficient buffer
    result = d_strerror_r(EINVAL, buffer, sizeof(buffer));

    test_result &= d_assert_standalone(
        (result == 0) && (strlen(buffer) > 0),
        "`d_strerror_r` successfully generates error message for valid error",
        "`d_strerror_r` failed to generate error message for valid error",
        _test_info
    );

    // Test 2: Unknown error number
    result = d_strerror_r(9999, buffer, sizeof(buffer));

    test_result &= d_assert_standalone(
        (result == 0) && (strlen(buffer) > 0),
        "`d_strerror_r` handles unknown error number",
        "`d_strerror_r` failed to handle unknown error number",
        _test_info
    );

    // Test 3: NULL buffer parameter
    result = d_strerror_r(EINVAL, NULL, sizeof(buffer));

    test_result &= d_assert_standalone(
        result == EINVAL,
        "`d_strerror_r` correctly handles NULL buffer parameter",
        "`d_strerror_r` failed to handle NULL buffer parameter",
        _test_info
    );

    // Test 4: Zero buffer size
    result = d_strerror_r(EINVAL, buffer, 0);

    test_result &= d_assert_standalone(
        result == EINVAL,
        "`d_strerror_r` correctly handles zero buffer size",
        "`d_strerror_r` failed to handle zero buffer size",
        _test_info
    );

    // Test 5: Buffer too small for message
    result = d_strerror_r(EINVAL, buffer, 1);

    test_result &= d_assert_standalone(
        result == ERANGE,
        "`d_strerror_r` correctly handles buffer too small",
        "`d_strerror_r` failed to handle buffer too small",
        _test_info
    );

    // Test 6: Error number zero (success)
    result = d_strerror_r(0, buffer, sizeof(buffer));

    test_result &= d_assert_standalone(
        (result == 0) && (strlen(buffer) > 0),
        "`d_strerror_r` correctly handles error number zero",
        "`d_strerror_r` failed to handle error number zero",
        _test_info
    );

    // Test 7: ERANGE error number
    result = d_strerror_r(ERANGE, buffer, sizeof(buffer));

    test_result &= d_assert_standalone(
        (result == 0) && (strlen(buffer) > 0),
        "`d_strerror_r` correctly handles ERANGE error",
        "`d_strerror_r` failed to handle ERANGE error",
        _test_info
    );

    _test_info->tests_total++;

    if (test_result)
    {
        _test_info->tests_passed++;
    }

    return test_result;
}

/*
===============================================================================
                        COMPREHENSIVE TEST SUITE RUNNERS
===============================================================================
*/

/*
d_tests_sa_dstring_all
  Runs all basic dstring function tests.
  Tests the following:
  - all safe string copy functions
  - all string duplication functions
  - all case-insensitive comparison functions
  - tokenization function
  - length calculation function
  - search function
  - case conversion functions
  - manipulation function
  - character search function
  - error handling function
*/
bool
d_tests_sa_dstring_all
(
    struct d_test_counter* _test_info
)
{
    bool overall_result;

    overall_result = true;

    printf("\n=== Running comprehensive dstring module tests ===\n\n");

    // safe string copy function tests
    overall_result &= d_tests_sa_strcpy_s(_test_info);
    overall_result &= d_tests_sa_strncpy_s(_test_info);
    overall_result &= d_tests_sa_strcat_s(_test_info);
    overall_result &= d_tests_sa_strncat_s(_test_info);

    // string duplication function tests
    overall_result &= d_tests_sa_strdup(_test_info);
    overall_result &= d_tests_sa_strndup(_test_info);

    // case-insensitive comparison function tests
    overall_result &= d_tests_sa_strcasecmp(_test_info);
    overall_result &= d_tests_sa_strncasecmp(_test_info);

    // tokenization function tests
    overall_result &= d_tests_sa_strtok_r(_test_info);

    // length calculation function tests
    overall_result &= d_tests_sa_strnlen(_test_info);

    // search function tests
    overall_result &= d_tests_sa_strcasestr(_test_info);

    // case conversion function tests
    overall_result &= d_tests_sa_strlwr(_test_info);
    overall_result &= d_tests_sa_strupr(_test_info);

    // manipulation function tests
    overall_result &= d_tests_sa_strrev(_test_info);

    // character search function tests
    overall_result &= d_tests_sa_strchrnul(_test_info);

    // error handling function tests
    overall_result &= d_tests_sa_strerror_r(_test_info);

    printf("\n=== dstring module tests complete ===\n");

    return overall_result;
}

/*
d_tests_sa_dstring_all_extended
  Runs all dstring function tests with extended test cases and stress testing.
  Tests the following:
  - all basic dstring tests
  - additional edge case testing
  - stress tests with large strings
  - cross-platform compatibility verification
  - performance boundary testing
*/
bool
d_tests_sa_dstring_all_extended
(
    struct d_test_counter* _test_info
)
{
    bool overall_result;

    overall_result = true;

    printf("\n=== Running extended dstring module tests ===\n\n");

    // run all basic tests first
    overall_result &= d_tests_sa_dstring_all(_test_info);

    printf("\n--- Extended testing not yet implemented ---\n");
    printf("Future extended tests will include:\n");
    printf("- Large string stress testing\n");
    printf("- Cross-platform character encoding tests\n");
    printf("- Performance boundary testing\n");
    printf("- Memory allocation failure simulation\n");
    printf("- Unicode and multi-byte character handling\n");

    printf("\n=== Extended dstring module tests complete ===\n");

    return overall_result;
}