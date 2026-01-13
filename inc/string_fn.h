/******************************************************************************
* djinterp [core]                                                  string_fn.h
*
* Cross-platform variants of certain `string.h` functions.
* 
*
* path:      \inc\string_fn.h                                  
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.10.26
******************************************************************************/

#ifndef DJINTERP_STRING_FN_
#define DJINTERP_STRING_FN_ 1

#include <ctype.h>
#include <stddef.h> 		// for size_t
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include ".\djinterp.h"		// 
#include ".\dmemory.h"		// 


// safe string copy (C11 `strcpy_s` equivalent)
int    d_strcpy_s(char* restrict _destination, size_t _dest_size, const char* restrict _src);
int    d_strncpy_s(char* restrict _destination, size_t _dest_size, const char* restrict _src, size_t _count);
int    d_strcat_s(char* restrict _destination, size_t _dest_size, const char* restrict _src);
int    d_strncat_s(char* restrict _destination, size_t _dest_size, const char* restrict _src, size_t _count);

// string duplication (POSIX `strdup` equivalent)
char*  d_strdup(const char* _str);
char*  d_strndup(const char* _str, size_t _n);

// case-insensitive comparison (POSIX `strcasecmp` equivalent)
int    d_strcasecmp(const char* _str2, const char* _s2);
int    d_strncasecmp(const char* _str2, const char* _s2, size_t _n);

// thread-safe tokenization (POSIX `strtok_r` equivalent)
char*  d_strtok_r(char* restrict _str, const char* restrict _delim, char** restrict _saveptr);

// string length with limit (C11 `strnlen` equivalent)
size_t d_strnlen(const char* _str, size_t _maxlen);

// case-insensitive substring search (GNU `strcasestr` equivalent)
char*  d_strcasestr(const char* _haystack, const char* _needle);

// string case conversion (Windows `_strlwr`/`_strupr` equivalent)
char*  d_strlwr(char* _str);
char*  d_strupr(char* _str);

// string reversal (Windows `_strrev` equivalent)
char*  d_strrev(char* _str);

// character search that returns end pointer (GNU `strchrnul` equivalent)
char*  d_strchrnul(const char* _str, int _c);

// thread-safe error string (POSIX `strerror_r` equivalent)
int    d_strerror_r(int _errnum, char* _buf, size_t _buflen);


#endif	// DJINTERP_STRING_FN_