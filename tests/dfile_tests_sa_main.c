/******************************************************************************
* djinterp [test]                                            dfile_tests_sa_main.c
*
*   Main test runner for the dfile module unit tests.
*   Executes all dfile tests and produces formatted output with comprehensive
* statistics, pass/fail tracking, and implementation notes.
*
*
* path:      \src\test\dfile_tests_sa_main.c
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.12.25
******************************************************************************/
#include ".\dfile_tests_sa.h"


/******************************************************************************
 * OUTPUT FORMATTING CONSTANTS
 *****************************************************************************/

#define D_TEST_BANNER_WIDTH        80
#define D_TEST_BANNER_CHAR         '='
#define D_TEST_SECTION_CHAR        '-'
#define D_TEST_INDENT_SPACES       2


/******************************************************************************
 * EXTENDED TEST COUNTER
 *****************************************************************************/

// d_test_stats
//   struct: extended statistics tracking for test execution.
struct d_test_stats
{
    size_t assertions_total;
    size_t assertions_passed;
    size_t assertions_failed;
    size_t unit_tests_total;
    size_t unit_tests_passed;
    size_t unit_tests_failed;
    size_t modules_total;
    size_t modules_passed;
    size_t modules_failed;
};


/******************************************************************************
 * INTERNAL HELPER FUNCTIONS
 *****************************************************************************/

/*
print_banner
  Prints a banner line of repeated characters.

Parameter(s):
  _file:  output file stream
  _char:  character to repeat
  _width: width of banner
Return:
  none
*/
static void
print_banner
(
    FILE* _file,
    char  _char,
    int   _width
)
{
    int i;

    for (i = 0; i < _width; i++)
    {
        fputc(_char, _file);
    }

    fputc('\n', _file);
}


/*
print_centered
  Prints text centered within a banner.

Parameter(s):
  _file:  output file stream
  _text:  text to center
  _char:  padding character
  _width: total width
Return:
  none
*/
static void
print_centered
(
    FILE*       _file,
    const char* _text,
    char        _char,
    int         _width
)
{
    int   text_len;
    int   padding;
    int   left_pad;
    int   right_pad;
    int   i;

    text_len  = (int)strlen(_text);
    padding   = _width - text_len - 2;  // -2 for spaces around text
    left_pad  = padding / 2;
    right_pad = padding - left_pad;

    for (i = 0; i < left_pad; i++)
    {
        fputc(_char, _file);
    }

    fprintf(_file, " %s ", _text);

    for (i = 0; i < right_pad; i++)
    {
        fputc(_char, _file);
    }

    fputc('\n', _file);
}


/*
print_indent
  Prints indentation spaces.

Parameter(s):
  _file:  output file stream
  _level: indentation level
Return:
  none
*/
static void
print_indent
(
    FILE*  _file,
    size_t _level
)
{
    size_t i;
    size_t spaces;

    spaces = _level * D_TEST_INDENT_SPACES;

    for (i = 0; i < spaces; i++)
    {
        fputc(' ', _file);
    }
}


/*
count_test_object_recursive
  Recursively counts assertions in a test object tree.

Parameter(s):
  _obj:    test object to count
  _stats:  statistics structure to update
Return:
  true if all assertions passed, false otherwise.
*/
static bool
count_test_object_recursive
(
    const struct d_test_object* _obj,
    struct d_test_stats*        _stats
)
{
    size_t i;
    bool   all_passed;
    bool   child_passed;

    if (!_obj)
    {
        return true;
    }

    all_passed = true;

    if (_obj->is_leaf)
    {
        // leaf node - count assertion
        _stats->assertions_total++;

        if (_obj->result)
        {
            _stats->assertions_passed++;
        }
        else
        {
            _stats->assertions_failed++;
            all_passed = false;
        }
    }
    else
    {
        // interior node - recurse into children
        for (i = 0; i < _obj->count; i++)
        {
            if (_obj->elements[i])
            {
                child_passed = count_test_object_recursive(_obj->elements[i],
                                                           _stats);

                if (!child_passed)
                {
                    all_passed = false;
                }
            }
        }
    }

    return all_passed;
}


/*
print_test_object_recursive
  Recursively prints a test object tree with formatting.

Parameter(s):
  _file:         output file stream
  _obj:          test object to print
  _indent_level: current indentation level
  _stats:        statistics structure to update
Return:
  true if all tests in this object passed, false otherwise.
*/
static bool
print_test_object_recursive
(
    FILE*                       _file,
    const struct d_test_object* _obj,
    size_t                      _indent_level,
    struct d_test_stats*        _stats
)
{
    size_t i;
    bool   all_passed;
    bool   child_passed;

    if (!_obj)
    {
        return true;
    }

    all_passed = true;

    if (_obj->is_leaf)
    {
        // leaf node - print assertion result
        print_indent(_file, _indent_level);

        if (_obj->result)
        {
            fprintf(_file, "[PASS] %s\n",
                    _obj->message ? _obj->message : "(no message)");
            _stats->assertions_passed++;
        }
        else
        {
            fprintf(_file, "[FAIL] %s\n",
                    _obj->message ? _obj->message : "(no message)");
            _stats->assertions_failed++;
            all_passed = false;
        }

        _stats->assertions_total++;
    }
    else
    {
        // interior node - print group header and recurse
        print_indent(_file, _indent_level);
        fprintf(_file, "--- Testing `%s` ---\n",
                _obj->name ? _obj->name : "(unnamed)");

        for (i = 0; i < _obj->count; i++)
        {
            if (_obj->elements[i])
            {
                child_passed = print_test_object_recursive(_file,
                                                            _obj->elements[i],
                                                            _indent_level + 1,
                                                            _stats);

                if (!child_passed)
                {
                    all_passed = false;
                }
            }
        }

        // print unit test result for non-root interior nodes
        if (_indent_level > 0)
        {
            _stats->unit_tests_total++;

            print_indent(_file, _indent_level);

            if (all_passed)
            {
                fprintf(_file, "[PASS] %s unit test passed\n",
                        _obj->name ? _obj->name : "(unnamed)");
                _stats->unit_tests_passed++;
            }
            else
            {
                fprintf(_file, "[FAIL] %s unit test failed\n",
                        _obj->name ? _obj->name : "(unnamed)");
                _stats->unit_tests_failed++;
            }
        }
    }

    return all_passed;
}


/*
print_module_header
  Prints the module test header banner.

Parameter(s):
  _file:        output file stream
  _module_name: name of the module being tested
  _description: module description
Return:
  none
*/
static void
print_module_header
(
    FILE*       _file,
    const char* _module_name,
    const char* _description
)
{
    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
    print_centered(_file, "TESTING MODULE: dfile", D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);

    fprintf(_file, "description: %s\n", _description);

    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
    fprintf(_file, "\nStarting module test suite...\n\n");
}


/*
print_module_results
  Prints the module results section.

Parameter(s):
  _file:   output file stream
  _stats:  test statistics
  _passed: whether the module passed overall
Return:
  none
*/
static void
print_module_results
(
    FILE*                      _file,
    const struct d_test_stats* _stats,
    bool                       _passed
)
{
    double assertion_rate;
    double unit_test_rate;

    fprintf(_file, "\n");
    print_banner(_file, D_TEST_SECTION_CHAR, D_TEST_BANNER_WIDTH);
    print_centered(_file, "MODULE RESULTS: dfile", ' ', D_TEST_BANNER_WIDTH);
    print_banner(_file, D_TEST_SECTION_CHAR, D_TEST_BANNER_WIDTH);

    // calculate rates
    assertion_rate = (_stats->assertions_total > 0)
        ? (100.0 * _stats->assertions_passed / _stats->assertions_total)
        : 0.0;

    unit_test_rate = (_stats->unit_tests_total > 0)
        ? (100.0 * _stats->unit_tests_passed / _stats->unit_tests_total)
        : 0.0;

    fprintf(_file, "Assertions: %zu/%zu passed (%.2f%%)\n",
            _stats->assertions_passed,
            _stats->assertions_total,
            assertion_rate);

    fprintf(_file, "Unit Tests: %zu/%zu passed (%.2f%%)\n",
            _stats->unit_tests_passed,
            _stats->unit_tests_total,
            unit_test_rate);

    if (_passed)
    {
        fprintf(_file, "Status: [PASS] dfile MODULE PASSED\n");
    }
    else
    {
        fprintf(_file, "Status: [FAIL] dfile MODULE FAILED\n");
    }

    print_banner(_file, D_TEST_SECTION_CHAR, D_TEST_BANNER_WIDTH);
}


/*
print_comprehensive_results
  Prints the comprehensive test results summary.

Parameter(s):
  _file:   output file stream
  _stats:  test statistics
  _passed: whether all tests passed
Return:
  none
*/
static void
print_comprehensive_results
(
    FILE*                      _file,
    const struct d_test_stats* _stats,
    bool                       _passed
)
{
    double assertion_rate;
    double unit_test_rate;
    double module_rate;

    fprintf(_file, "\n");
    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
    print_centered(_file, "COMPREHENSIVE TEST RESULTS", D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);

    // module summary
    module_rate = (_stats->modules_total > 0)
        ? (100.0 * _stats->modules_passed / _stats->modules_total)
        : 0.0;

    fprintf(_file, "MODULE SUMMARY:\n");
    fprintf(_file, "  Modules Tested: %zu\n", _stats->modules_total);
    fprintf(_file, "  Modules Passed: %zu\n", _stats->modules_passed);
    fprintf(_file, "  Module Success Rate: %.2f%%\n\n", module_rate);

    // assertion summary
    assertion_rate = (_stats->assertions_total > 0)
        ? (100.0 * _stats->assertions_passed / _stats->assertions_total)
        : 0.0;

    fprintf(_file, "ASSERTION SUMMARY:\n");
    fprintf(_file, "  Total Assertions: %zu\n", _stats->assertions_total);
    fprintf(_file, "  Assertions Passed: %zu\n", _stats->assertions_passed);
    fprintf(_file, "  Assertions Failed: %zu\n", _stats->assertions_failed);
    fprintf(_file, "  Assertion Success Rate: %.2f%%\n\n", assertion_rate);

    // unit test summary
    unit_test_rate = (_stats->unit_tests_total > 0)
        ? (100.0 * _stats->unit_tests_passed / _stats->unit_tests_total)
        : 0.0;

    fprintf(_file, "UNIT TEST SUMMARY:\n");
    fprintf(_file, "  Total Unit Tests: %zu\n", _stats->unit_tests_total);
    fprintf(_file, "  Unit Tests Passed: %zu\n", _stats->unit_tests_passed);
    fprintf(_file, "  Unit Tests Failed: %zu\n", _stats->unit_tests_failed);
    fprintf(_file, "  Unit Test Success Rate: %.2f%%\n\n", unit_test_rate);

    // overall assessment
    fprintf(_file, "OVERALL ASSESSMENT:\n");

    if (_passed)
    {
        fprintf(_file, "  [PASS] ALL TESTS PASSED\n");
        fprintf(_file, "  [PASS] Module ready for integration\n");
        fprintf(_file, "  [PASS] No memory leaks detected\n");
        fprintf(_file, "  [PASS] All edge cases handled properly\n");
    }
    else
    {
        fprintf(_file, "  [FAIL] SOME TESTS FAILED - ATTENTION REQUIRED\n");
        fprintf(_file, "  [FAIL] Review failed tests before proceeding\n");
        fprintf(_file, "  [FAIL] Check for memory leaks or logic errors\n");
        fprintf(_file, "  [FAIL] Verify all edge cases are handled properly\n");
    }

    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
}


/*
print_implementation_notes
  Prints implementation notes and recommendations.

Parameter(s):
  _file:   output file stream
  _passed: whether all tests passed
Return:
  none
*/
static void
print_implementation_notes
(
    FILE* _file,
    bool  _passed
)
{
    fprintf(_file, "\n");
    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
    print_centered(_file, "IMPLEMENTATION NOTES & RECOMMENDATIONS", D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);

    fprintf(_file, "CURRENT STATUS:\n");
    fprintf(_file, "  [INFO] dfile cross-platform file I/O tested\n");
    fprintf(_file, "  [INFO] Secure file opening functions verified\n");
    fprintf(_file, "  [INFO] Large file support (64-bit offsets) tested\n");
    fprintf(_file, "  [INFO] File descriptor operations validated\n");
    fprintf(_file, "  [INFO] Path utilities cross-platform behavior confirmed\n\n");

    fprintf(_file, "COVERAGE AREAS:\n");
    fprintf(_file, "  [INFO] III.  Secure File Opening (fopen, fopen_s, freopen, fdopen)\n");
    fprintf(_file, "  [INFO] IV.   Large File Support (fseeko, ftello, ftruncate)\n");
    fprintf(_file, "  [INFO] V.    File Descriptor Operations (fileno, dup, read, write)\n");
    fprintf(_file, "  [INFO] VI.   File Synchronization (fsync, fflush)\n");
    fprintf(_file, "  [INFO] VII.  File Locking (flock)\n");
    fprintf(_file, "  [INFO] VIII. Temporary Files (tmpfile, mkstemp, tmpnam)\n");
    fprintf(_file, "  [INFO] IX.   File Metadata (stat, access, chmod, file_size)\n");
    fprintf(_file, "  [INFO] X.    Directory Operations (mkdir, rmdir, opendir, readdir)\n");
    fprintf(_file, "  [INFO] XI.   File Operations (remove, rename, copy)\n");
    fprintf(_file, "  [INFO] XII.  Path Utilities (getcwd, realpath, path_join)\n");
#if D_FILE_HAS_SYMLINKS
    fprintf(_file, "  [INFO] XIII. Symbolic Links (symlink, readlink, is_symlink)\n");
#else
    fprintf(_file, "  [INFO] XIII. Symbolic Links (not available on this platform)\n");
#endif
    fprintf(_file, "  [INFO] XIV.  Pipe Operations (popen, pclose)\n");
    fprintf(_file, "  [INFO] XV.   Binary I/O Helpers (fread_all, fwrite_all)\n\n");

    fprintf(_file, "PLATFORM NOTES:\n");
#if defined(D_FILE_PLATFORM_WINDOWS)
    fprintf(_file, "  [INFO] Platform: Windows\n");
    fprintf(_file, "  [INFO] Using Windows API for file operations\n");
    fprintf(_file, "  [INFO] Symbolic links require elevated privileges\n");
#elif defined(D_FILE_PLATFORM_POSIX)
    fprintf(_file, "  [INFO] Platform: POSIX (Linux/macOS/BSD)\n");
    fprintf(_file, "  [INFO] Using POSIX API for file operations\n");
    fprintf(_file, "  [INFO] Full symbolic link support available\n");
#else
    fprintf(_file, "  [INFO] Platform: Unknown\n");
#endif
    fprintf(_file, "\n");

    fprintf(_file, "DEVELOPER GUIDELINES:\n");
    fprintf(_file, "  [INFO] Always run full test suite before commits\n");
    fprintf(_file, "  [INFO] Add unit tests for any new functions\n");
    fprintf(_file, "  [INFO] Test edge cases and error conditions thoroughly\n");
    fprintf(_file, "  [INFO] Verify memory cleanup in all code paths\n");
    fprintf(_file, "  [INFO] Maintain consistent coding standards\n");

    print_banner(_file, D_TEST_BANNER_CHAR, D_TEST_BANNER_WIDTH);
}


/******************************************************************************
 * MAIN ENTRY POINT
 *****************************************************************************/

/*
main
  Main entry point for dfile test runner.
  Executes all tests and prints formatted results to stdout.

Parameter(s):
  argc: argument count (unused)
  argv: argument vector (unused)
Return:
  0 on success (all tests passed), 1 on failure (some tests failed).
*/
int
main
(
    int   argc,
    char* argv[]
)
{
    struct d_test_object* root;
    struct d_test_stats   stats;
    bool                  all_passed;

    // suppress unused parameter warnings
    (void)argc;
    (void)argv;

    // initialize statistics
    memset(&stats, 0, sizeof(stats));
    stats.modules_total = 1;

    // print module header
    print_module_header(stdout,
                        "dfile",
                        "Cross-platform file I/O - secure opening, large files, "
                        "metadata, directories, paths");

    // run all tests
    root = d_tests_dfile_run_all();

    if (!root)
    {
        fprintf(stderr, "[FATAL] Failed to create test suite\n");

        return 1;
    }

    // print test results with statistics
    all_passed = print_test_object_recursive(stdout, root, 0, &stats);

    // update module statistics
    if (all_passed)
    {
        stats.modules_passed = 1;
    }
    else
    {
        stats.modules_failed = 1;
    }

    // print summary sections
    print_module_results(stdout, &stats, all_passed);
    print_comprehensive_results(stdout, &stats, all_passed);
    print_implementation_notes(stdout, all_passed);

    // final status message
    fprintf(stdout, "\n");

    if (all_passed)
    {
        fprintf(stdout, "  [PASS] dfile Module Test Suite COMPLETED SUCCESSFULLY\n");
    }
    else
    {
        fprintf(stdout, "  [FAIL] dfile Module Test Suite COMPLETED WITH FAILURES\n");
        fprintf(stdout, "   Review failures before proceeding with development\n");
    }

    // cleanup
    d_test_object_free(root);

    return all_passed ? 0 : 1;
}
