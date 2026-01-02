/******************************************************************************
* djinterp [text]                                                text_lineup.h
*
*   Text lineup and formatting utilities for generating aligned, wrapped text
* output. Provides tools for formatting token sequences into columns with
* configurable alignment, prefixes, suffixes, and line wrapping.
* 
* Contents:
*   - d_index:        signed index type supporting negative (reverse) indexing
*   - d_text_buffer:  dynamically growing text buffer with append/prepend
*   - d_string_array: dynamic array of strings with series generation
*   - d_text_lineup:  core formatting function for aligned token output
*   - d_code_c_define_macro: C preprocessor macro definition generator
*
* 
* path:      \inc\text\text_lineup.h
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2025.12.17
******************************************************************************/

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>



// D_SUCCESS
//   alias: corresponds to a SUCCESSFUL operation; evaluates to D_TRUE.
#define D_SUCCESS true

// D_FAILURE
//   alias: corresponds to a FAILED operation; evaluates to D_FALSE.
#define D_FAILURE false

typedef ssize_t d_index;

size_t d_index_convert_fast(d_index _index, size_t _count);
bool   d_index_convert_safe(d_index _index, size_t _count, size_t* _destination);
bool   d_index_is_valid(d_index _index, size_t _count);

/*
d_index_convert_fast
  Converts a `d_index` to its positive equivalent.

Parameter(s):
  _index: the index to convert.
  _count: the count of elements in the array.

Return:
  The positive equivalent of the index.
*/
size_t 
d_index_convert_fast
(
    d_index _index, 
    size_t  _count
) 
{
    return (_index >= 0) ?
        (size_t)_index :
        (size_t)(_count - ((ssize_t)(-_index)));
}

/*
d_index_convert_safe
  Converts a `d_index` to its positive equivalent safely.

Parameter(s):
  _index:       the index to convert.
  _count:       the count of elements in the array.
  _destination: pointer to store the converted index.

Return:
  D_SUCCESS if the conversion was successful, D_FAILURE otherwise.
*/
bool
d_index_convert_safe
(
    d_index _index, 
    size_t  _count,
    size_t* _destination
) 
{
    size_t positive_index;
    size_t abs_value;

    if (_destination == NULL)
    {
        return D_FAILURE;
    }
    
    if (_index >= 0)
    {
        positive_index = (size_t)_index;
    }
    else
    {
        abs_value = (size_t)(-_index);

        if (abs_value > _count)
        {
            return D_FAILURE;
        }
        
        positive_index = _count - abs_value;
    }
    
    if (positive_index >= _count)
    {
        return D_FAILURE;
    }
    
    *(_destination) = positive_index;

    return D_SUCCESS;
}

/*
d_index_is_valid
  Evaluates a `d_index` for validity.

Parameter(s):
  _index: the index to evaluate.
  _count: the count of elements in the array.

Return:
  true if the index is valid, false otherwise.
*/
bool
d_index_is_valid
(
    d_index _index, 
    size_t  _count
)
{
    return ( (_count > 0) &&
             ( ( (_index >= 0) && ((size_t)_index < _count) )  ||
               ( (_index < 0)  && ((size_t)(-_index) <= _count) ) ) );
}


// D_DEFAULT_TEXT_BUFFER_SIZE
//   constant: default size for text buffer allocation.
#define D_DEFAULT_TEXT_BUFFER_SIZE 256

/* d_text_buffer
   struct: holds a dynamically growing text buffer with position tracking. */
struct d_text_buffer
{
    size_t pos;
    char*  buffer;
    size_t capacity;
};


struct d_text_buffer* d_text_buffer_new(size_t _capacity);
bool   d_text_buffer_ensure_capacity(struct d_text_buffer* _text_buffer, size_t _capacity);
size_t d_text_buffer_append(struct d_text_buffer* _text_buffer, const char* _text);
size_t d_text_buffer_append_s(struct d_text_buffer* _text_buffer, const char* _text, size_t _text_length);
size_t d_text_buffer_prepend(struct d_text_buffer* _text_buffer, const char* _text);
size_t d_text_buffer_write_over(struct d_text_buffer* _text_buffer, d_index _start, d_index _end);
void   d_text_buffer_free(struct d_text_buffer* _text_buffer);

/*
d_text_buffer_new
  Creates a new text buffer with the specified capacity.

Parameter(s):
  _capacity: the initial capacity of the buffer. If 0, uses default size.

Return:
  A pointer to the new text buffer, or NULL on failure.
*/
struct d_text_buffer* 
d_text_buffer_new
(
    size_t _capacity
)
{
    struct d_text_buffer* new_text_buffer;

    new_text_buffer = malloc(sizeof(struct d_text_buffer));

    if (!new_text_buffer)
    {
        return NULL;
    }

    new_text_buffer->capacity = (_capacity > 0)
                                    ? _capacity 
                                    : D_DEFAULT_TEXT_BUFFER_SIZE;
    new_text_buffer->buffer = calloc(new_text_buffer->capacity, sizeof(char));

    if (!new_text_buffer->buffer)
    {
        free(new_text_buffer);

        return NULL;
    }

    new_text_buffer->pos = 0;

    return new_text_buffer;
}

/*
d_text_buffer_ensure_capacity
  Ensures the buffer has at least the specified capacity.

Parameter(s):
  _text_buffer: the text buffer to resize.
  _capacity:    the minimum required capacity.

Return:
  D_SUCCESS if capacity is sufficient or was increased, D_FAILURE otherwise.
*/
bool
d_text_buffer_ensure_capacity
(
    struct d_text_buffer* _text_buffer,
    size_t                _capacity
)
{
    char*  new_buffer;
    size_t new_capacity;

    if (!_text_buffer)
    {
        return D_FAILURE;
    }

    if (_text_buffer->capacity >= _capacity)
    {
        return D_SUCCESS;
    }

    new_capacity = _text_buffer->capacity * 2;

    while (new_capacity < _capacity)
    {
        new_capacity *= 2;
    }

    new_buffer = realloc(_text_buffer->buffer, new_capacity);

    if (!new_buffer)
    {
        return D_FAILURE;
    }

    _text_buffer->buffer   = new_buffer;
    _text_buffer->capacity = new_capacity;

    return D_SUCCESS;
}

/*
d_text_buffer_append
  Appends text to the end of the buffer.

Parameter(s):
  _text_buffer: the text buffer to append to.
  _text:        the null-terminated string to append.

Return:
  The length of text appended, or (size_t)-1 on failure.
*/
size_t 
d_text_buffer_append
(
    struct d_text_buffer* _text_buffer, 
    const char*           _text
)
{
    size_t text_length;
    size_t space_required;
    size_t new_capacity;
    char*  new_buffer;

    if ( (!_text_buffer) || 
         (!_text) )
    {
        return (size_t)-1;
    }

    text_length    = strlen(_text);
    space_required = _text_buffer->pos + text_length + 1;

    if (space_required > _text_buffer->capacity)
    {
        new_capacity = _text_buffer->capacity * 2;

        while (new_capacity < space_required)
        {
            new_capacity *= 2;
        }

        new_buffer = realloc(_text_buffer->buffer, new_capacity);

        if (!new_buffer)
        {
            return (size_t)-1;
        }

        _text_buffer->buffer   = new_buffer;
        _text_buffer->capacity = new_capacity;
    }

    memcpy(_text_buffer->buffer + _text_buffer->pos, _text, text_length + 1);
    _text_buffer->pos += text_length;

    return text_length;
}

/*
d_text_buffer_append_s
  Appends text of a specified length to the buffer.

Parameter(s):
  _text_buffer: the text buffer to append to.
  _text:        the string to append.
  _text_length: the number of characters to append.

Return:
  true on success, false on failure.
*/
size_t
d_text_buffer_append_s
(
    struct d_text_buffer* _text_buffer, 
    const char*           _text,
    size_t                _text_length
)
{
    size_t space_required;
    size_t new_capacity;
    char*  new_buffer;

    if ( (!_text_buffer) || 
         (!_text) )
    {
        return false;
    }

    space_required = _text_buffer->pos + _text_length + 1;

    if (space_required > _text_buffer->capacity)
    {
        new_capacity = _text_buffer->capacity * 2;

        while (new_capacity < space_required)
        {
            new_capacity *= 2;
        }

        new_buffer = realloc(_text_buffer->buffer, new_capacity);

        if (!new_buffer)
        {
            return false;
        }

        _text_buffer->buffer   = new_buffer;
        _text_buffer->capacity = new_capacity;
    }

    memcpy(_text_buffer->buffer + _text_buffer->pos, _text, _text_length + 1);
    _text_buffer->pos += _text_length;

    return true;
}

/*
d_text_buffer_prepend
  Prepends text to the beginning of the buffer.

Parameter(s):
  _text_buffer: the text buffer to prepend to.
  _text:        the null-terminated string to prepend.

Return:
  true on success, false on failure.
*/
size_t 
d_text_buffer_prepend
(
    struct d_text_buffer* _text_buffer, 
    const char*           _text
)
{
    size_t text_len;
    size_t space_required;
    size_t new_capacity;
    char*  new_buffer;
    
    if ( (!_text_buffer) || 
         (!_text) )
    {
        return false;
    }

    text_len       = strlen(_text);
    space_required = (_text_buffer->pos + text_len + 1);

    if (space_required > _text_buffer->capacity)
    {
        new_capacity = _text_buffer->capacity * 2;

        while (new_capacity < space_required)
        {
            new_capacity *= 2;
        }

        new_buffer = realloc(_text_buffer->buffer, new_capacity);

        if (!new_buffer)
        {
            return false;
        }

        _text_buffer->buffer   = new_buffer;
        _text_buffer->capacity = new_capacity;
    }

    memmove(_text_buffer->buffer + text_len,
            _text_buffer->buffer,
            _text_buffer->pos + 1);
    memcpy(_text_buffer->buffer, _text, text_len);
    _text_buffer->pos += text_len;

    return true;
}

/*
d_text_buffer_write_over
  Removes characters from the buffer between two indices.

Parameter(s):
  _text_buffer: the text buffer to modify.
  _start:       the starting index (supports negative indexing).
  _end:         the ending index (supports negative indexing).

Return:
  The number of characters removed, or (size_t)-1 on failure.
*/
size_t
d_text_buffer_write_over
(
    struct d_text_buffer* _text_buffer,
    d_index               _start,
    d_index               _end
)
{
    size_t start_pos;
    size_t end_pos;
    size_t chars_removed;

    if (!_text_buffer)
    {
        return (size_t)-1;
    }

    if (_text_buffer->pos == 0)
    {
        return (_start == 0 && _end == 0) ? 0 : (size_t)-1;
    }

    if (!d_index_convert_safe(_start, _text_buffer->pos, &start_pos))
    {
        return (size_t)-1;
    }

    if (_end >= 0 && (size_t)_end == _text_buffer->pos)
    {
        end_pos = _text_buffer->pos;
    }
    else if (!d_index_convert_safe(_end, _text_buffer->pos, &end_pos))
    {
        return (size_t)-1;
    }

    if (start_pos > end_pos)
    {
        return (size_t)-1;
    }

    chars_removed = end_pos - start_pos;

    if (chars_removed == 0)
    {
        return 0;
    }

    memmove(_text_buffer->buffer + start_pos,
            _text_buffer->buffer + end_pos,
            _text_buffer->pos - end_pos + 1);

    _text_buffer->pos -= chars_removed;

    return chars_removed;
}

/*
d_text_buffer_free
  Frees a text buffer and its contents.

Parameter(s):
  _text_buffer: the text buffer to free.
*/
void 
d_text_buffer_free
(
    struct d_text_buffer* _text_buffer
)
{
    if (!_text_buffer)
    {
        return;
    }

    free(_text_buffer->buffer);
    free(_text_buffer);

    return;
}


/* d_string_array
   struct: holds an array of strings with count + capacity. */
struct d_string_array
{
    size_t count;
    size_t capacity;
    char** elements;
};

/*
d_internal_strdup
  Internal helper to duplicate a string.

Parameter(s):
  _s: the string to duplicate.

Return:
  A newly allocated copy of the string, or NULL on failure.
*/
static char*
d_internal_strdup
(
    const char* _s
)
{
    size_t len;
    char*  out;

    if (!_s)
    {
        return NULL;
    }

    len = strlen(_s);
    out = (char*)malloc(len + 1);

    if (!out)
    {
        return NULL;
    }

    memcpy(out, _s, len + 1);

    return out;
}

/*
d_string_array_new
  Creates a new string array with the specified capacity.

Parameter(s):
  _capacity: the initial capacity of the array.

Return:
  A pointer to the new string array, or NULL on failure.
*/
struct d_string_array*
d_string_array_new
(
    size_t _capacity
)
{
    struct d_string_array* array;

    array = (struct d_string_array*)malloc(sizeof(struct d_string_array));

    if (!array)
    {
        return NULL;
    }

    array->count    = 0;
    array->capacity = _capacity;
    array->elements = NULL;

    if (_capacity > 0)
    {
        array->elements = (char**)calloc(_capacity, sizeof(char*));

        if (!array->elements)
        {
            free(array);

            return NULL;
        }
    }

    return array;
}

/*
d_string_array_free
  Frees a string array and all its elements.

Parameter(s):
  _array: the string array to free.
*/
void
d_string_array_free
(
    struct d_string_array* _array
)
{
    size_t i;

    if (!_array)
    {
        return;
    }

    if (_array->elements)
    {
        for (i = 0; i < _array->count; i++)
        {
            free(_array->elements[i]);
        }

        free(_array->elements);
    }

    free(_array);

    return;
}

/*
d_string_array_reserve
  Ensures the array has at least the specified capacity.

Parameter(s):
  _array:        the string array to resize.
  _min_capacity: the minimum required capacity.

Return:
  true if capacity is sufficient or was increased, false otherwise.
*/
bool
d_string_array_reserve
(
    struct d_string_array* _array,
    size_t                 _min_capacity
)
{
    char** new_elements;
    size_t new_capacity;

    if (!_array)
    {
        return false;
    }

    if (_min_capacity <= _array->capacity)
    {
        return true;
    }

    new_capacity = _array->capacity ? _array->capacity : 8;

    while (new_capacity < _min_capacity)
    {
        if (new_capacity > (SIZE_MAX / 2))
        {
            new_capacity = _min_capacity;
            break;
        }

        new_capacity *= 2;
    }

    if (new_capacity > (SIZE_MAX / sizeof(char*)))
    {
        return false;
    }

    new_elements = (char**)realloc(_array->elements,
                                   new_capacity * sizeof(char*));

    if (!new_elements)
    {
        return false;
    }

    if (new_capacity > _array->capacity)
    {
        memset(new_elements + _array->capacity,
               0,
               (new_capacity - _array->capacity) * sizeof(char*));
    }

    _array->elements = new_elements;
    _array->capacity = new_capacity;

    return true;
}

/*
d_string_array_append
  Appends a string to the array.

Parameter(s):
  _array: the string array to append to.
  _str:   the string to append (will be copied).

Return:
  true on success, false on failure.
*/
bool
d_string_array_append
(
    struct d_string_array* _array,
    const char*            _str
)
{
    char* copy;

    if ( (!_array) || 
         (!_str) )
    {
        return false;
    }

    if (!d_string_array_reserve(_array, _array->count + 1))
    {
        return false;
    }

    copy = d_internal_strdup(_str);

    if (!copy)
    {
        return false;
    }

    _array->elements[_array->count++] = copy;

    return true;
}

/*
d_internal_digits_u
  Counts the number of digits in an unsigned integer.

Parameter(s):
  _v: the value to count digits for.

Return:
  The number of digits in the value.
*/
static size_t
d_internal_digits_u
(
    unsigned int _v
)
{
    size_t d;

    d = 1;

    while (_v >= 10u)
    {
        _v /= 10u;
        d++;
    }

    return d;
}

/*
d_string_array_generate_series_capacity
  Generates a series of strings with optional prefix/suffix and padding.

Parameter(s):
  _start:               the starting value of the series.
  _end:                 the ending value of the series.
  _step:                the step between values.
  _prefix:              optional prefix for each string.
  _suffix:              optional suffix for each string.
  _zero_padding:        minimum width for numbers (zero-padded).
  _additional_capacity: extra capacity to reserve beyond the series.

Return:
  A pointer to the new string array, or NULL on failure.
*/
struct d_string_array*
d_string_array_generate_series_capacity
(
    int         _start,
    int         _end,
    int         _step,
    const char* _prefix,
    const char* _suffix,
    size_t      _zero_padding,
    size_t      _additional_capacity
)
{
    struct d_string_array* array;
    size_t                 token_count;
    size_t                 prefix_length;
    size_t                 suffix_length;
    size_t                 max_number_length;
    size_t                 buffer_size;
    size_t                 i;
    int                    current_value;
    long long              diff;
    long long              step_ll;
    unsigned int           max_abs;
    unsigned int           a;
    unsigned int           b;
    char                   number_buffer[64];
    int                    width;

    if (_step == 0)
    {
        return NULL;
    }

    if ( ( (_step > 0) && (_start > _end) ) ||
         ( (_step < 0) && (_start < _end) ) )
    {
        return NULL;
    }

    diff    = (long long)_end - (long long)_start;
    step_ll = (long long)_step;

    if (_step > 0)
    {
        token_count = (size_t)(diff / step_ll) + 1u;
    }
    else
    {
        token_count = (size_t)((-diff) / (-step_ll)) + 1u;
    }

    if (token_count == 0)
    {
        return NULL;
    }

    if (token_count > (SIZE_MAX - _additional_capacity))
    {
        return NULL;
    }

    array = d_string_array_new(token_count + _additional_capacity);

    if (!array)
    {
        return NULL;
    }

    prefix_length = _prefix ? strlen(_prefix) : 0;
    suffix_length = _suffix ? strlen(_suffix) : 0;

    a       = (unsigned int)((_start < 0) ? -(long long)_start : _start);
    b       = (unsigned int)((_end   < 0) ? -(long long)_end   : _end);
    max_abs = (a > b) ? a : b;

    max_number_length = d_internal_digits_u(max_abs);

    if (_zero_padding > max_number_length)
    {
        max_number_length = _zero_padding;
    }

    if ( (_start < 0) || (_end < 0) )
    {
        max_number_length += 1;
    }

    buffer_size   = prefix_length + max_number_length + suffix_length + 1;
    current_value = _start;
    array->count  = 0;

    for (i = 0; i < token_count; i++)
    {
        array->elements[i] = (char*)malloc(buffer_size);

        if (!array->elements[i])
        {
            array->count = i;
            d_string_array_free(array);

            return NULL;
        }

        array->elements[i][0] = '\0';

        if (_prefix)
        {
            strcpy(array->elements[i], _prefix);
        }

        if ((_zero_padding > 0) && (current_value >= 0))
        {
            width = (_zero_padding > (size_t)INT_MAX) 
                        ? INT_MAX 
                        : (int)_zero_padding;
            snprintf(number_buffer,
                     sizeof(number_buffer),
                     "%0*d",
                     width,
                     current_value);
        }
        else
        {
            snprintf(number_buffer,
                     sizeof(number_buffer),
                     "%d",
                     current_value);
        }

        strcat(array->elements[i], number_buffer);

        if (_suffix)
        {
            strcat(array->elements[i], _suffix);
        }

        array->count = i + 1;
        current_value += _step;

        if ( (_step > 0 && current_value > _end) ||
             (_step < 0 && current_value < _end) )
        {
            break;
        }
    }

    return array;
}

/*
d_string_array_generate_series
  Generates a series of strings with optional prefix/suffix and padding.

Parameter(s):
  _start:        the starting value of the series.
  _end:          the ending value of the series.
  _step:         the step between values.
  _prefix:       optional prefix for each string.
  _suffix:       optional suffix for each string.
  _zero_padding: minimum width for numbers (zero-padded).

Return:
  A pointer to the new string array, or NULL on failure.
*/
struct d_string_array*
d_string_array_generate_series
(
    int         _start,
    int         _end,
    int         _step,
    const char* _prefix,
    const char* _suffix,
    size_t      _zero_padding
)
{
    return d_string_array_generate_series_capacity(_start,
                                                   _end,
                                                   _step,
                                                   _prefix,
                                                   _suffix,
                                                   _zero_padding,
                                                   0);
}


/*
d_code_c_define_macro
  Generates a C preprocessor macro definition string.

Parameter(s):
  _name:    the macro name.
  _params:  optional parameter list (without parentheses).
  _body:    the macro body.
  _comment: optional comment to precede the macro.

Return:
  A newly allocated string containing the macro definition, or NULL on failure.
*/
char*
d_code_c_define_macro
(
    const char* _name,
    const char* _params,
    const char* _body,
    const char* _comment
)
{
    struct d_text_buffer* buffer;
    char*                 result;
    const char*           body_ptr;
    bool                  need_continuation;
    
    if ( (!_name) || 
         (!_body) )
    {
        return NULL;
    }
    
    buffer = d_text_buffer_new(256);
    
    if (!buffer)
    {
        return NULL;
    }
    
    if (_comment)
    {
        d_text_buffer_append(buffer, "/* ");
        d_text_buffer_append(buffer, _comment);
        d_text_buffer_append(buffer, " */\n");
    }
    
    d_text_buffer_append(buffer, "#define ");
    d_text_buffer_append(buffer, _name);
    
    if (_params)
    {
        d_text_buffer_append(buffer, "(");
        d_text_buffer_append(buffer, _params);
        d_text_buffer_append(buffer, ")");
    }
    
    need_continuation = (strchr(_body, '\n') != NULL);
    
    if (need_continuation)
    {
        d_text_buffer_append(buffer, " \\\n    ");
        
        body_ptr = _body;

        while (*body_ptr)
        {
            if (*body_ptr == '\n')
            {
                if (*(body_ptr + 1) != '\0')
                {
                    d_text_buffer_append(buffer, " \\\n    ");
                }
                else
                {
                    d_text_buffer_append(buffer, "\n");
                }

                body_ptr++;
            }
            else
            {
                d_text_buffer_append_s(buffer, body_ptr, 1);
                body_ptr++;
            }
        }
    }
    else
    {
        d_text_buffer_append(buffer, " ");
        d_text_buffer_append(buffer, _body);
    }
    
    d_text_buffer_append(buffer, "\n");
    
    result = strdup(buffer->buffer);
    d_text_buffer_free(buffer);
    
    return result;
}


// D_BIT_MASK_UINT32
//   macro: determines if a value has a bit enabled.
#define D_BIT_MASK_UINT32(value, flag)  \
    ( (uint32_t)(value & flag) )


/* DBehaviorFlag
   enum: flags controlling text lineup behavior. */
enum DBehaviorFlag
{
    D_BEHAVIOR_FLAG_UNKNOWN                    = 0x0000,
    D_BEHAVIOR_FLAG_LINE_PREFIX_ALL_LINES      = 0x0001,
    D_BEHAVIOR_FLAG_LINE_SUFFIX_ALL_LINES      = 0x0002,
    D_BEHAVIOR_FLAG_ALLOW_OVERSIZE_PREFIX      = 0x0004,
    D_BEHAVIOR_FLAG_ALLOW_OVERSIZE_SUFFIX      = 0x0008,
    D_BEHAVIOR_FLAG_ALIGN_SINGLE_ROW           = 0x0010,
    D_BEHAVIOR_FLAG_LINE_SUFFIX_ON_SINGLE_LINE = 0x0020,
    D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX   = 0x0040,
    D_BEHAVIOR_FLAG_RIGHT_ALIGN_LINE_SUFFIX    = 0x0080,
    D_BEHAVIOR_FLAG_TOKEN_ALIGN_LEFT           = 0x0100,
    D_BEHAVIOR_FLAG_TOKEN_ALIGN_CENTER         = 0x0200,
    D_BEHAVIOR_FLAG_PAD_AFTER_SEPARATOR        = 0x0400,
     D_BEHAVIOR_FLAG_ALIGN_FIRST_ROW_TO_SLOTS  = 0x0800
};

// D_TEXT_TEMPLATE_PREFIX_ALL_LINES
//   macro: checks if line prefix should be applied to all lines.
#define D_TEXT_TEMPLATE_PREFIX_ALL_LINES(value)         \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_LINE_PREFIX_ALL_LINES) )

// D_TEXT_TEMPLATE_SUFFIX_ALL_LINES
//   macro: checks if line suffix should be applied to all lines.
#define D_TEXT_TEMPLATE_SUFFIX_ALL_LINES(value)         \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_LINE_SUFFIX_ALL_LINES) )

// D_TEXT_TEMPLATE_ALLOWS_OVERSIZE_PREFIX
//   macro: checks if oversize prefix is allowed.
#define D_TEXT_TEMPLATE_ALLOWS_OVERSIZE_PREFIX(value)   \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_ALLOW_OVERSIZE_PREFIX) )

// D_TEXT_TEMPLATE_ALLOWS_OVERSIZE_SUFFIX
//   macro: checks if oversize suffix is allowed.
#define D_TEXT_TEMPLATE_ALLOWS_OVERSIZE_SUFFIX(value)   \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_ALLOW_OVERSIZE_SUFFIX) )

// D_TEXT_LINE_UP_ALIGNS_SINGLE_ROW
//   macro: checks if single row alignment is enabled.
#define D_TEXT_LINE_UP_ALIGNS_SINGLE_ROW(value)         \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_ALIGN_SINGLE_ROW) )

// D_TEXT_LINE_UP_SUFFIX_ON_SINGLE_LINE
//   macro: checks if suffix should appear on single line output.
#define D_TEXT_LINE_UP_SUFFIX_ON_SINGLE_LINE(value)     \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_LINE_SUFFIX_ON_SINGLE_LINE) )

// D_TEXT_LINE_UP_LINE_SUFFIX_AFTER_SUFFIX
//   macro: checks if line suffix comes after the main suffix.
#define D_TEXT_LINE_UP_LINE_SUFFIX_AFTER_SUFFIX(value)  \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX) )

// D_TEXT_LINE_UP_RIGHT_ALIGN_LINE_SUFFIX
//   macro: checks if line suffix should be right-aligned.
#define D_TEXT_LINE_UP_RIGHT_ALIGN_LINE_SUFFIX(value)   \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_RIGHT_ALIGN_LINE_SUFFIX) )

// D_TEXT_LINE_UP_TOKEN_ALIGN_LEFT
//   macro: checks if tokens should be left-aligned.
#define D_TEXT_LINE_UP_TOKEN_ALIGN_LEFT(value)          \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_TOKEN_ALIGN_LEFT) )

// D_TEXT_LINE_UP_TOKEN_ALIGN_CENTER
//   macro: checks if tokens should be center-aligned.
#define D_TEXT_LINE_UP_TOKEN_ALIGN_CENTER(value)        \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_TOKEN_ALIGN_CENTER) )

// D_TEXT_LINE_UP_PAD_AFTER_SEPARATOR
//   macro: checks if padding should come after the separator.
#define D_TEXT_LINE_UP_PAD_AFTER_SEPARATOR(value)       \
    ( D_BIT_MASK_UINT32(value, D_BEHAVIOR_FLAG_PAD_AFTER_SEPARATOR) )


/* d_text_template
   struct: template configuration for text lineup operations. */
struct d_text_template
{
    struct d_string_array* tokens;
    char*                  separator;
    bool                   separator_before;
    bool                   separator_after;
    char*                  line_prefix;
    char*                  line_suffix;
    size_t                 min_token_width;
    uint32_t               flags;
};


/*
d_text_lineup
  Formats tokens into aligned lines with configurable prefixes and suffixes.

Parameter(s):
  _template:       the template containing tokens and formatting options.
  _text_buffer:    the buffer to write output to.
  _line_width:     the maximum line width (0 for no wrapping).
  _prefix:         optional prefix for the first line.
  _suffix:         optional suffix for the last line.
  _indent_prefix:  if true, line_prefix precedes prefix on first line.
  _outdent_suffix: if true, line_suffix follows suffix on last line.

Return:
  true on success, false on failure.
*/
bool 
d_text_lineup
(
    struct d_text_template* _template,
    struct d_text_buffer*   _text_buffer,
    size_t                  _line_width,
    const char*             _prefix,
    const char*             _suffix,
    bool                    _indent_prefix,
    bool                    _outdent_suffix
)
{
    size_t  start_pos;
    size_t  line_start_pos;
    size_t  line_prefix_length;
    size_t  line_suffix_length;
    size_t  prefix_length;
    size_t  suffix_length;
    size_t  separator_length;
    size_t  max_token_length;
    size_t  token_index;
    size_t  token_slot_size;
    size_t  content_width;
    size_t  first_line_content_width;
    size_t  current_line_pos;
    size_t  padding_needed;
    size_t  first_row_padding;
    size_t  num_tokens;
    size_t  max_padding;
    size_t  token_space;
    size_t  line_content_width;
    size_t  current_line_length;
    size_t  padding_to_apply;
    size_t  pad_before;
    size_t  pad_after;
    size_t* token_lengths;
    bool    are_tokens_same_length;
    bool    right_align_suffix;
    bool    line_suffix_after_suffix;
    bool    token_align_left;
    bool    token_align_center;
    bool    pad_after_separator;
    bool    is_last_token;
    bool    write_separator;
    char*   padding_buffer;

    #define APPEND_CHECK(buf, str)                                            \
        do {                                                                  \
            if (d_text_buffer_append((buf), (str)) == (size_t)-1)             \
            {                                                                 \
                goto undo_with_cleanup;                                       \
            }                                                                 \
        } while(0)
    
    #define APPEND_S_CHECK(buf, str, len)                                     \
        do {                                                                  \
            if (!d_text_buffer_append_s((buf), (str), (len)))                 \
            {                                                                 \
                goto undo_with_cleanup;                                       \
            }                                                                 \
        } while(0)

    if ( (!_template)                ||
         (!_template->tokens)        ||
         (!_template->tokens->count) ||
         (!_text_buffer) )
    {
        return false;
    }

    num_tokens             = _template->tokens->count;
    right_align_suffix     = D_TEXT_LINE_UP_RIGHT_ALIGN_LINE_SUFFIX(_template->flags) != 0;
    line_suffix_after_suffix = D_TEXT_LINE_UP_LINE_SUFFIX_AFTER_SUFFIX(_template->flags) != 0;
    token_align_left       = D_TEXT_LINE_UP_TOKEN_ALIGN_LEFT(_template->flags) != 0;
    token_align_center     = D_TEXT_LINE_UP_TOKEN_ALIGN_CENTER(_template->flags) != 0;
    pad_after_separator    = D_TEXT_LINE_UP_PAD_AFTER_SEPARATOR(_template->flags) != 0;
    start_pos              = _text_buffer->pos;
    prefix_length          = _prefix ? strlen(_prefix) : 0;
    line_prefix_length     = _template->line_prefix 
                                 ? strlen(_template->line_prefix) 
                                 : 0;
    suffix_length          = _suffix ? strlen(_suffix) : 0;
    line_suffix_length     = _template->line_suffix 
                                 ? strlen(_template->line_suffix)
                                 : 0;
    separator_length       = _template->separator 
                                 ? strlen(_template->separator)
                                 : 0;
    padding_buffer         = NULL;
    token_lengths          = NULL;

    // handle zero line width (no wrapping)
    if (_line_width == 0)
    {
        if (_prefix)
        {
            APPEND_CHECK(_text_buffer, _prefix);
        }

        if (_template->separator_before && _template->separator)
        {
            APPEND_CHECK(_text_buffer, _template->separator);
        }

        for (token_index = 0; token_index < num_tokens; token_index++)
        {
            APPEND_CHECK(_text_buffer, _template->tokens->elements[token_index]);
            
            if (token_index < num_tokens - 1)
            {
                if (_template->separator)
                {
                    APPEND_CHECK(_text_buffer, _template->separator);
                }
            }
            else if (_template->separator_after && _template->separator)
            {
                APPEND_CHECK(_text_buffer, _template->separator);
            }
        }

        if (_suffix)
        {
            APPEND_CHECK(_text_buffer, _suffix);
        }

        return true;
    }

    // validate prefix and suffix sizes
    if ( (prefix_length > _line_width) && 
         (!D_TEXT_TEMPLATE_ALLOWS_OVERSIZE_PREFIX(_template->flags)) )
    {
        return false;
    }

    if ( (suffix_length > _line_width) && 
         (!D_TEXT_TEMPLATE_ALLOWS_OVERSIZE_SUFFIX(_template->flags)) )
    {
        return false;
    }

    // allocate token lengths array
    token_lengths = malloc(sizeof(size_t) * num_tokens);

    if (!token_lengths)
    {
        return false;
    }

    // calculate token lengths and find maximum
    max_token_length       = strlen(_template->tokens->elements[0]);
    token_lengths[0]       = max_token_length;
    are_tokens_same_length = true;

    for (token_index = 1; token_index < num_tokens; token_index++)
    {
        token_lengths[token_index] = strlen(_template->tokens->elements[token_index]);
        
        if (token_lengths[token_index] != max_token_length)
        {
            are_tokens_same_length = false;
            
            if (token_lengths[token_index] > max_token_length)
            {
                max_token_length = token_lengths[token_index];
            }
        }
    }

    if (_template->min_token_width > max_token_length)
    {
        max_token_length       = _template->min_token_width;
        are_tokens_same_length = false;
    }

    // calculate slot size and padding
    token_slot_size = max_token_length + separator_length;

    if (token_slot_size == 0)
    {
        first_row_padding = 0;
    }
    else if (_indent_prefix)
    {
        // first line already includes line_prefix, so align to slot boundary
        first_row_padding =
            (token_slot_size - (prefix_length % token_slot_size)) % token_slot_size;
    }
    else
    {
        // first line does NOT include line_prefix, but subsequent lines DO.
        // Align first token to the same modular column as subsequent lines.
        size_t want = line_prefix_length % token_slot_size;
        size_t have = prefix_length      % token_slot_size;

        first_row_padding =
            (want + token_slot_size - have) % token_slot_size;
    }


    // allocate padding buffer
    max_padding = max_token_length;

    if (first_row_padding > max_padding)
    {
        max_padding = first_row_padding;
    }

    if (prefix_length > max_padding)
    {
        max_padding = prefix_length;
    }

    if (_line_width > max_padding)
    {
        max_padding = _line_width;
    }
    
    padding_buffer = malloc(max_padding + 1);

    if (!padding_buffer)
    {
        if (token_lengths)
        {
            free(token_lengths);
        }

        return false;
    }

    memset(padding_buffer, ' ', max_padding);
    padding_buffer[max_padding] = '\0';

    // calculate content widths
    content_width = _line_width;
    
    if (line_prefix_length < content_width)
    {
        content_width -= line_prefix_length;
    }
    else
    {
        content_width = 0;
    }
        
    if (line_suffix_length < content_width)
    {
        content_width -= line_suffix_length;
    }
    else
    {
        content_width = 0;
    }

    first_line_content_width = _line_width;
    
    if (_indent_prefix && line_prefix_length < first_line_content_width)
    {
        first_line_content_width -= line_prefix_length;
    }
    
    if (prefix_length < first_line_content_width)
    {
        first_line_content_width -= prefix_length;
    }
    else
    {
        first_line_content_width = 0;
    }
    
    if (first_row_padding < first_line_content_width)
    {
        first_line_content_width -= first_row_padding;
    }
    else
    {
        first_line_content_width = 0;
    }
        
    if (line_suffix_length < first_line_content_width)
    {
        first_line_content_width -= line_suffix_length;
    }
    else
    {
        first_line_content_width = 0;
    }

    // === BEGIN OUTPUT ===
    
    token_index    = 0;
    line_start_pos = _text_buffer->pos;
    
    // --- FIRST LINE ---
    
    if (_indent_prefix && _template->line_prefix)
    {
        APPEND_CHECK(_text_buffer, _template->line_prefix);
    }
    
    if (_prefix)
    {
        APPEND_CHECK(_text_buffer, _prefix);
    }
    
    // apply first row padding for alignment
    if (first_row_padding > 0)
    {
        APPEND_S_CHECK(_text_buffer, padding_buffer, first_row_padding);
    }
    
    if (_template->separator_before && _template->separator)
    {
        APPEND_CHECK(_text_buffer, _template->separator);

        if (first_line_content_width >= separator_length)
        {
            first_line_content_width -= separator_length;
        }
        else
        {
            first_line_content_width = 0;
        }
    }
    
    current_line_pos = 0;
    
    // write tokens for first line
    while (token_index < num_tokens)
    {
        token_space   = max_token_length;
        is_last_token = (token_index == num_tokens - 1);
        
        if (!is_last_token || _template->separator_after)
        {
            token_space += separator_length;
        }
        
        // check if token fits on current line
        if (current_line_pos > 0 && 
            current_line_pos + token_space > first_line_content_width)
        {
            break;
        }
        
        // determine if separator needed
        write_separator = false;

        if (_template->separator)
        {
            if (!is_last_token || _template->separator_after)
            {
                write_separator = true;
            }
        }
        
        // apply token padding based on alignment mode
        if (!are_tokens_same_length)
        {
            padding_needed = max_token_length - token_lengths[token_index];

            if (padding_needed > 0)
            {
                if (token_align_left)
                {
                    if (pad_after_separator && write_separator)
                    {
                        APPEND_CHECK(_text_buffer,
                                     _template->tokens->elements[token_index]);
                        APPEND_CHECK(_text_buffer, _template->separator);
                        APPEND_S_CHECK(_text_buffer,
                                       padding_buffer,
                                       padding_needed);
                        write_separator = false;
                    }
                    else
                    {
                        APPEND_CHECK(_text_buffer,
                                     _template->tokens->elements[token_index]);
                        APPEND_S_CHECK(_text_buffer,
                                       padding_buffer,
                                       padding_needed);
                    }
                }
                else if (token_align_center)
                {
                    pad_before = padding_needed / 2;
                    pad_after  = padding_needed - pad_before;
                    
                    if (pad_after_separator && write_separator)
                    {
                        if (pad_before > 0)
                        {
                            APPEND_S_CHECK(_text_buffer,
                                           padding_buffer,
                                           pad_before);
                        }

                        APPEND_CHECK(_text_buffer,
                                     _template->tokens->elements[token_index]);
                        APPEND_CHECK(_text_buffer, _template->separator);

                        if (pad_after > 0)
                        {
                            APPEND_S_CHECK(_text_buffer,
                                           padding_buffer,
                                           pad_after);
                        }

                        write_separator = false;
                    }
                    else
                    {
                        if (pad_before > 0)
                        {
                            APPEND_S_CHECK(_text_buffer,
                                           padding_buffer,
                                           pad_before);
                        }

                        APPEND_CHECK(_text_buffer,
                                     _template->tokens->elements[token_index]);

                        if (pad_after > 0)
                        {
                            APPEND_S_CHECK(_text_buffer,
                                           padding_buffer,
                                           pad_after);
                        }
                    }
                }
                else
                {
                    // right-align (default)
                    APPEND_S_CHECK(_text_buffer, padding_buffer, padding_needed);
                    APPEND_CHECK(_text_buffer,
                                 _template->tokens->elements[token_index]);
                }
            }
            else
            {
                APPEND_CHECK(_text_buffer,
                             _template->tokens->elements[token_index]);
            }
        }
        else
        {
            APPEND_CHECK(_text_buffer,
                         _template->tokens->elements[token_index]);
        }
        
        current_line_pos += max_token_length;
        
        // write separator if not already written
        if (write_separator)
        {
            APPEND_CHECK(_text_buffer, _template->separator);
            current_line_pos += separator_length;
        }
        else if (_template->separator && 
                 (!is_last_token || _template->separator_after))
        {
            current_line_pos += separator_length;
        }
        
        token_index++;
    }
    
    // check if all tokens fit on first line
    if (token_index >= num_tokens)
    {
        if (_suffix)
        {
            APPEND_CHECK(_text_buffer, _suffix);
        }
        
        if (line_suffix_after_suffix && 
            _template->line_suffix && 
            line_suffix_length > 0)
        {
            if (right_align_suffix)
            {
                current_line_length = _text_buffer->pos - line_start_pos;
                
                if (current_line_length + line_suffix_length < _line_width)
                {
                    padding_needed = _line_width 
                                     - current_line_length 
                                     - line_suffix_length;
                    APPEND_S_CHECK(_text_buffer, padding_buffer, padding_needed);
                }
            }
            
            APPEND_CHECK(_text_buffer, _template->line_suffix);
        }
        
        goto success;
    }
    
    // write line_suffix on non-last lines
    if (_template->line_suffix && line_suffix_length > 0)
    {
        if (_outdent_suffix && right_align_suffix)
        {
            current_line_length = _text_buffer->pos - line_start_pos;
            
            if (current_line_length + line_suffix_length < _line_width)
            {
                padding_needed = _line_width 
                                 - current_line_length 
                                 - line_suffix_length;
                APPEND_S_CHECK(_text_buffer, padding_buffer, padding_needed);
            }
        }
        
        APPEND_CHECK(_text_buffer, _template->line_suffix);
    }
    
    APPEND_CHECK(_text_buffer, "\n");
    
    // --- SUBSEQUENT LINES ---
    
    while (token_index < num_tokens)
    {
        line_content_width = content_width;
        line_start_pos     = _text_buffer->pos;
        
        if (_template->line_prefix)
        {
            APPEND_CHECK(_text_buffer, _template->line_prefix);
        }
        
        current_line_pos = 0;
        
        // write tokens for this line
        while (token_index < num_tokens)
        {
            token_space   = max_token_length;
            is_last_token = (token_index == num_tokens - 1);
            
            if (!is_last_token || _template->separator_after)
            {
                token_space += separator_length;
            }
            
            // check if token fits
            if (current_line_pos > 0 && 
                current_line_pos + token_space > line_content_width)
            {
                break;
            }
            
            // determine if separator needed
            write_separator = false;

            if (_template->separator)
            {
                if (!is_last_token || _template->separator_after)
                {
                    write_separator = true;
                }
            }
            
            // apply token padding based on alignment mode
            if (!are_tokens_same_length)
            {
                padding_needed = max_token_length - token_lengths[token_index];

                if (padding_needed > 0)
                {
                    if (token_align_left)
                    {
                        if (pad_after_separator && write_separator)
                        {
                            APPEND_CHECK(_text_buffer,
                                         _template->tokens->elements[token_index]);
                            APPEND_CHECK(_text_buffer, _template->separator);
                            APPEND_S_CHECK(_text_buffer,
                                           padding_buffer,
                                           padding_needed);
                            write_separator = false;
                        }
                        else
                        {
                            APPEND_CHECK(_text_buffer,
                                         _template->tokens->elements[token_index]);
                            APPEND_S_CHECK(_text_buffer,
                                           padding_buffer,
                                           padding_needed);
                        }
                    }
                    else if (token_align_center)
                    {
                        pad_before = padding_needed / 2;
                        pad_after  = padding_needed - pad_before;
                        
                        if (pad_after_separator && write_separator)
                        {
                            if (pad_before > 0)
                            {
                                APPEND_S_CHECK(_text_buffer,
                                               padding_buffer,
                                               pad_before);
                            }

                            APPEND_CHECK(_text_buffer,
                                         _template->tokens->elements[token_index]);
                            APPEND_CHECK(_text_buffer, _template->separator);

                            if (pad_after > 0)
                            {
                                APPEND_S_CHECK(_text_buffer,
                                               padding_buffer,
                                               pad_after);
                            }

                            write_separator = false;
                        }
                        else
                        {
                            if (pad_before > 0)
                            {
                                APPEND_S_CHECK(_text_buffer,
                                               padding_buffer,
                                               pad_before);
                            }

                            APPEND_CHECK(_text_buffer,
                                         _template->tokens->elements[token_index]);

                            if (pad_after > 0)
                            {
                                APPEND_S_CHECK(_text_buffer,
                                               padding_buffer,
                                               pad_after);
                            }
                        }
                    }
                    else
                    {
                        // right-align (default)
                        APPEND_S_CHECK(_text_buffer,
                                       padding_buffer,
                                       padding_needed);
                        APPEND_CHECK(_text_buffer,
                                     _template->tokens->elements[token_index]);
                    }
                }
                else
                {
                    APPEND_CHECK(_text_buffer,
                                 _template->tokens->elements[token_index]);
                }
            }
            else
            {
                APPEND_CHECK(_text_buffer,
                             _template->tokens->elements[token_index]);
            }
            
            current_line_pos += max_token_length;
            
            // write separator if not already written
            if (write_separator)
            {
                APPEND_CHECK(_text_buffer, _template->separator);
                current_line_pos += separator_length;
            }
            else if (_template->separator && 
                     (!is_last_token || _template->separator_after))
            {
                current_line_pos += separator_length;
            }
            
            token_index++;
        }
        
        // check if this was the last line
        if (token_index >= num_tokens)
        {
            if (_suffix)
            {
                APPEND_CHECK(_text_buffer, _suffix);
            }
            
            if (line_suffix_after_suffix && 
                _template->line_suffix && 
                line_suffix_length > 0)
            {
                if (right_align_suffix)
                {
                    current_line_length = _text_buffer->pos - line_start_pos;
                    
                    if (current_line_length + line_suffix_length < _line_width)
                    {
                        padding_needed = _line_width 
                                         - current_line_length 
                                         - line_suffix_length;
                        APPEND_S_CHECK(_text_buffer,
                                       padding_buffer,
                                       padding_needed);
                    }
                }
                
                APPEND_CHECK(_text_buffer, _template->line_suffix);
            }

            break;
        }
        
        // write line_suffix on non-last lines
        if (_template->line_suffix && line_suffix_length > 0)
        {
            if (_outdent_suffix && right_align_suffix)
            {
                current_line_length = _text_buffer->pos - line_start_pos;
                
                if (current_line_length + line_suffix_length < _line_width)
                {
                    padding_needed = _line_width 
                                     - current_line_length 
                                     - line_suffix_length;
                    APPEND_S_CHECK(_text_buffer, padding_buffer, padding_needed);
                }
            }
            
            APPEND_CHECK(_text_buffer, _template->line_suffix);
        }
        
        APPEND_CHECK(_text_buffer, "\n");
    }

success:
    if (padding_buffer)
    {
        free(padding_buffer);
    }

    if (token_lengths)
    {
        free(token_lengths);
    }
    
    #undef APPEND_CHECK
    #undef APPEND_S_CHECK
    
    return true;

undo_with_cleanup:
    if (padding_buffer)
    {
        free(padding_buffer);
    }

    if (token_lengths)
    {
        free(token_lengths);
    }

    _text_buffer->pos = start_pos;
    _text_buffer->buffer[start_pos] = '\0';
    
    #undef APPEND_CHECK
    #undef APPEND_S_CHECK
    
    return false;
}


int
main
(
    int   argc,
    char* argv[]
)
{
    struct d_string_array* tokens;
    struct d_text_template template;
    struct d_text_buffer*  output;
    char*                  result;
    size_t                 i;
    size_t                 j;
    size_t                 n;
    char                   name[64];
    char                   params[512];
    char                   body[1024];
    char                   param[16];
    char                   call[32];
    char*                  macro_def;
    
    (void)argc;
    (void)argv;
    
    printf("=== Text Lineup and Code Generation Demo ===\n\n");
    
    // Demo 1: File listing
    printf("Demo 1: File listing with alignment\n");
    printf("------------------------------------\n");
    
    tokens = d_string_array_generate_series(1, 20, 1, "file_", ".txt", 3);
    
    if (tokens)
    {
        output = d_text_buffer_new(1024);
        
        template.tokens           = tokens;
        template.separator        = " | ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "  ";
        template.line_suffix      = "";
        template.min_token_width  = 0;
        template.flags            = 0;
        
        if (d_text_lineup(&template, output, 80, "Files: ", " [END]", true, false))
        {
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");
    
    // Demo 2: Countdown
    printf("Demo 2: Countdown timer display\n");
    printf("--------------------------------\n");
    
    tokens = d_string_array_generate_series(10, 0, -1, "T-", NULL, 2);
    
    if (tokens)
    {
        output = d_text_buffer_new(512);
        
        template.tokens           = tokens;
        template.separator        = " -> ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = ";";
        template.min_token_width  = 0;
        template.flags            = 0;
        
        if (d_text_lineup(&template,
                          output,
                          50,
                          "Countdown: ",
                          " LIFTOFF!",
                          false,
                          false))
        {
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");
    
    // Demo 3: Simple macro
    printf("Demo 3: Single macro definition\n");
    printf("--------------------------------\n");
    
    result = d_code_c_define_macro("D_MAX",
                                   "a, b",
                                   "((a) > (b) ? (a) : (b))",
                                   "Returns the maximum of two values");

    if (result)
    {
        printf("%s", result);
        free(result);
    }
    
    printf("\n");
    
    // Demo 4: FOR_EACH macros
    printf("Demo 4: FOR_EACH macro series (first 5)\n");
    printf("----------------------------------------\n");
    
    output = d_text_buffer_new(4096);
    
    for (i = 1; i <= 5; i++)
    {
        snprintf(name, sizeof(name), "D_INTERNAL_FOR_EACH_%zu", i);
        
        strcpy(params, "_fn");

        for (j = 1; j <= i; j++)
        {
            snprintf(param, sizeof(param), ", _%zu", j);
            strcat(params, param);
        }
        
        body[0] = '\0';

        for (j = 1; j <= i; j++)
        {
            if (j > 1)
            {
                strcat(body, ", ");
            }

            snprintf(call, sizeof(call), "_fn(_%zu)", j);
            strcat(body, call);
        }
        
        macro_def = d_code_c_define_macro(name, params, body, NULL);
        
        if (macro_def)
        {
            d_text_buffer_append(output, macro_def);
            free(macro_def);
        }
    }
    
    printf("%s", output->buffer);
    d_text_buffer_free(output);
    
    printf("\n");
    
    // Demo 5: FOR_EACH_PAIR macros
    printf("Demo 5: FOR_EACH_PAIR macro series (first 3 even numbers)\n");
    printf("----------------------------------------------------------\n");
    
    output = d_text_buffer_new(4096);
    
    for (i = 1; i <= 3; i++)
    {
        n = i * 2;
        
        snprintf(name, sizeof(name), "D_INTERNAL_FOR_EACH_PAIR_%zu", n);
        
        strcpy(params, "_fn");

        for (j = 1; j <= n; j++)
        {
            snprintf(param, sizeof(param), ", _%zu", j);
            strcat(params, param);
        }
        
        body[0] = '\0';

        for (j = 1; j <= n; j += 2)
        {
            if (j > 1)
            {
                strcat(body, ", ");
            }

            snprintf(call, sizeof(call), "_fn(_%zu, _%zu)", j, j + 1);
            strcat(body, call);
        }
        
        macro_def = d_code_c_define_macro(name, params, body, NULL);
        
        if (macro_def)
        {
            d_text_buffer_append(output, macro_def);
            free(macro_def);
        }
    }
    
    printf("%s", output->buffer);
    d_text_buffer_free(output);
    
    printf("\n");
    
    // Demo 6: Memory map
    printf("Demo 6: Memory map display\n");
    printf("---------------------------\n");
    
    tokens = d_string_array_generate_series(0, 240, 16, "0x", NULL, 4);
    
    if (tokens)
    {
        output = d_text_buffer_new(1024);
        
        template.tokens           = tokens;
        template.separator        = " ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "  ";
        template.line_suffix      = "";
        template.min_token_width  = 0;
        template.flags            = 0;
        
        if (d_text_lineup(&template,
                          output,
                          70,
                          "Memory: [",
                          "]",
                          false,
                          false))
        {
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");
    
    // Demo 7: indent_prefix comparison
    printf("Demo 7: indent_prefix = true vs false\n");
    printf("--------------------------------------\n");
    
    tokens = d_string_array_generate_series(1, 20, 1, "val", "", 2);
    
    if (tokens)
    {
        output = d_text_buffer_new(1024);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "----";
        template.line_suffix      = "";
        template.min_token_width  = 0;
        template.flags            = 0;
        
        d_text_buffer_append(output, "indent_prefix = TRUE:\n");
        d_text_buffer_append(output, "(first line: line_prefix + prefix)\n");

        if (d_text_lineup(&template, output, 50, "List: ", "", true, false))
        {
            d_text_buffer_append(output, "\n\n");
        }
        
        d_text_buffer_append(output, "indent_prefix = FALSE:\n");
        d_text_buffer_append(output,
                             "(first line: just prefix, subsequent: "
                             "line_prefix + alignment)\n");

        if (d_text_lineup(&template, output, 50, "List: ", "", false, false))
        {
            d_text_buffer_append(output, "\n");
        }
        
        printf("%s", output->buffer);
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");
    
    // Demo 8: Token alignment modes
    printf("Demo 8: Token alignment modes (left/center/right)\n");
    printf("-------------------------------------------------\n");
    
    tokens = d_string_array_generate_series(1, 12, 1, "item", "", 0);
    
    if (tokens)
    {
        output = d_text_buffer_new(2048);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "               ";
        template.line_suffix      = "";
        template.min_token_width  = 0;
        
        // right-align (default)
        template.flags = 0;
        d_text_buffer_append(output, "Right-align (default):\n");

        if (d_text_lineup(&template,
                          output,
                          60,
                          "Extended List: ",
                          "",
                          false,
                          false))
        {
            d_text_buffer_append(output, "\n\n");
        }
        
        // left-align (padding before separator)
        template.flags = D_BEHAVIOR_FLAG_TOKEN_ALIGN_LEFT;
        d_text_buffer_append(output,
                             "Left-align (pad before separator): \"item1 , \"\n");

        if (d_text_lineup(&template,
                          output,
                          60,
                          "Extended List: ",
                          "",
                          false,
                          false))
        {
            d_text_buffer_append(output, "\n\n");
        }
        
        // left-align (padding after separator)
        template.flags = D_BEHAVIOR_FLAG_TOKEN_ALIGN_LEFT | 
                         D_BEHAVIOR_FLAG_PAD_AFTER_SEPARATOR;
        d_text_buffer_append(output,
                             "Left-align (pad after separator): \"item1,  \"\n");

        if (d_text_lineup(&template,
                          output,
                          60,
                          "Extended List: ",
                          "",
                          false,
                          false))
        {
            d_text_buffer_append(output, "\n\n");
        }
        
        // center-align
        template.flags = D_BEHAVIOR_FLAG_TOKEN_ALIGN_CENTER;
        d_text_buffer_append(output, "Center-align:\n");

        if (d_text_lineup(&template,
                          output,
                          60,
                          "Extended List: ",
                          "",
                          false,
                          false))
        {
            d_text_buffer_append(output, "\n");
        }
        
        printf("%s", output->buffer);
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }

    printf("\n");

    // Demo 9: VARG GET_ARG_99 macro - NO right-alignment
    printf("Demo 9: VARG GET_ARG_99 macro (no right-align)\n");
    printf("-----------------------------------------------\n");
    
    tokens = d_string_array_generate_series_capacity(1, 99, 1, "_", NULL, 0, 1);
    d_string_array_append(tokens, "...");
    
    if (tokens)
    {
        output = d_text_buffer_new(4096);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 0;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          80,
                          "#define D_VARG_GET_ARG_99(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    _99");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");

    // Demo 10: Same macro WITH right-alignment
    printf("Demo 10: VARG GET_ARG_99 macro (WITH right-align)\n");
    printf("-------------------------------------------------\n");
    
    tokens = d_string_array_generate_series_capacity(1, 99, 1, "_", NULL, 0, 1);
    d_string_array_append(tokens, "...");
    
    if (tokens)
    {
        output = d_text_buffer_new(4096);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 0;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX | 
                                    D_BEHAVIOR_FLAG_RIGHT_ALIGN_LINE_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          80,
                          "#define D_VARG_GET_ARG_99(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    _99");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");

    // Demo 11: VARG GET_ARG_100 macro - NO right-alignment
    printf("Demo 11: VARG GET_ARG_100 macro (no right-align)\n");
    printf("-----------------------------------------------\n");
    
    tokens = d_string_array_generate_series_capacity(1, 100, 1, "_", NULL, 0, 1);
    d_string_array_append(tokens, "...");
    
    if (tokens)
    {
        output = d_text_buffer_new(4096);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 0;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          80,
                          "#define D_VARG_GET_ARG_100(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    _100");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");

    // Demo 12: Same macro WITH right-alignment
    printf("Demo 12: VARG GET_ARG_100 macro (WITH right-align)\n");
    printf("-------------------------------------------------\n");
    
    tokens = d_string_array_generate_series_capacity(1, 100, 1, "_", NULL, 0, 1);
    d_string_array_append(tokens, "...");
    
    if (tokens)
    {
        output = d_text_buffer_new(4096);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 0;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX | 
                                    D_BEHAVIOR_FLAG_RIGHT_ALIGN_LINE_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          80,
                          "#define D_VARG_GET_ARG_100(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    _100");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");

    // Demo 13: min_token_width
    printf("Demo 13: min_token_width for macro series alignment\n");
    printf("----------------------------------------------------\n");
    printf("Without min_token_width:\n");
    
    tokens = d_string_array_generate_series(1, 5, 1, "_", NULL, 0);

    if (tokens)
    {
        output = d_text_buffer_new(256);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 0;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          60,
                          "#define MACRO_5(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    body5");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    tokens = d_string_array_generate_series(1, 12, 1, "_", NULL, 0);

    if (tokens)
    {
        output = d_text_buffer_new(256);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 0;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          60,
                          "#define MACRO_12(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    body12");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\nWith min_token_width = 3:\n");
    
    tokens = d_string_array_generate_series(1, 5, 1, "_", NULL, 0);

    if (tokens)
    {
        output = d_text_buffer_new(256);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 3;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          60,
                          "#define MACRO_5(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    body5");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    tokens = d_string_array_generate_series(1, 12, 1, "_", NULL, 0);

    if (tokens)
    {
        output = d_text_buffer_new(256);
        
        template.tokens           = tokens;
        template.separator        = ", ";
        template.separator_before = false;
        template.separator_after  = false;
        template.line_prefix      = "    ";
        template.line_suffix      = " \\";
        template.min_token_width  = 3;
        template.flags            = D_BEHAVIOR_FLAG_LINE_SUFFIX_AFTER_SUFFIX;
        
        if (d_text_lineup(&template,
                          output,
                          60,
                          "#define MACRO_12(",
                          ")",
                          false,
                          true))
        {
            d_text_buffer_append(output, "\n    body12");
            printf("%s\n", output->buffer);
        }
        
        d_text_buffer_free(output);
        d_string_array_free(tokens);
    }
    
    printf("\n");
    
    printf("\n=== Demo Complete ===\n");
    
    return 0;
}