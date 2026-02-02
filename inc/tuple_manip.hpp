/******************************************************************************
* djinterp [meta]                                              tuple_manip.hpp
*
* djinterp tuple manipulation header:
*   This header provides advanced tuple manipulation metafunctions for
* compile-time tuple operations. It includes:
*   - tuple joining and concatenation (tuple_join, tuple_concat)
*   - element access (tuple_type_at, tuple_type_at_value)
*   - type counting and removal (tuple_count_type, tuple_count_and_remove)
*   - tuple splitting (tuple_split, tuple_subsequence)
*   - type transformation (tuple_apply_all, tuple_consolidate_types)
*   - type selection utilities (type_case, type_selector)
*
*   PORTABILITY:
*   This header uses env.h for C++ version detection and cpp_features.h for
* fine-grained feature detection. All metafunctions use portable trait access
* patterns (::value instead of _v suffixes) for compatibility with C++11 and
* later.
* 
* path:      \inc\meta\tuple_manip.hpp
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2024.05.07
******************************************************************************/

#ifndef DJINTERP_TUPLE_MANIP_
#define DJINTERP_TUPLE_MANIP_ 1

#include <tuple>
#include <array>
#include <type_traits>
#include ".\djinterp.h"
#include "..\cpp_features.h"
#include ".\tuple_util.hpp"


NS_DJINTERP

    // forward declaractions
    template<typename    _Type, 
             typename... _Types> 
    struct tuple_count_and_remove;

    template<typename    _Type, 
             typename... _Types> 
    struct tuple_first_type;

    NS_INTERNAL
        template<typename _Tuple,
                 typename... _Types>
        struct tuple_join_helper;

        template<typename... _Types>
        struct tuple_join_helper<std::tuple<>, _Types...>;
        template<typename... _Head,
                 typename... _Tail,
                 typename... _Types>
        struct tuple_join_helper<std::tuple<std::tuple<_Head...>, _Tail...>, _Types...>;

        template<typename    _Head,
                 typename... _Tail,
                 typename... _Types>
        struct tuple_join_helper<std::tuple<_Head, _Tail...>, _Types...>;
    NS_END  // internal

    template<typename... _Tuples>
    struct tuple_join;


    // =========================================================================
    // II.  TUPLE TRANSFORMATION
    // =========================================================================

    // tuple_apply_all (internal helper)
    NS_INTERNAL

        template<template<typename...> typename _UnaryTrait,
                  typename    _Tuple,
                  typename... _Types>
        struct tuple_apply_all_helper;

        template<template<typename...> typename _UnaryTrait,
                  typename... _Types>
        struct tuple_apply_all_helper<_UnaryTrait, std::tuple<>, _Types...>
        {
            using type = std::tuple<_Types...>;
        };

        template<template<typename...> typename _UnaryTrait,
                  typename    _Head,
                  typename... _Tails,
                  typename... _Types>
        struct tuple_apply_all_helper<_UnaryTrait, std::tuple<_Head, _Tails...>, _Types...>
        {
            using type = typename tuple_apply_all_helper<
                _UnaryTrait,
                std::tuple<_Tails...>,
                _Types...,
                _UnaryTrait<_Head>
            >::type;
        };

    NS_END  // internal

    // tuple_apply_all
    //   type trait: applies a unary type trait to all elements of a tuple.
    template<template<typename...> typename _UnaryTrait,
              typename... _Types>
    struct tuple_apply_all
    {
        using type = typename internal::tuple_apply_all_helper<
            _UnaryTrait,
            typename to_tuple<_Types...>::type
        >::type;
    };

    // tuple_apply_all_t
    //   alias template: shorthand for `tuple_apply_all<...>::type`.
    template<template<typename...> typename _UnaryTrait,
              typename... _Types>
    using tuple_apply_all_t = typename tuple_apply_all<_UnaryTrait, _Types...>::type;


    // =========================================================================
    // III. ELEMENT ACCESS
    // =========================================================================

    // tuple_type_at (internal helper)
    NS_INTERNAL

        template<std::size_t _Index,
                  typename    _Tuple>
        struct tuple_type_at_helper;

        template<typename    _Head,
                  typename... _Tail>
        struct tuple_type_at_helper<0, std::tuple<_Head, _Tail...>>
        {
            using type = _Head;

            static constexpr auto value(const std::tuple<_Head, _Tail...>& _t)
            {
                return std::get<0>(_t);
            }
        };

        template<std::size_t _Index,
                  typename    _Head,
                  typename... _Tail>
        struct tuple_type_at_helper<_Index, std::tuple<_Head, _Tail...>>
        {
            using type = typename tuple_type_at_helper<_Index - 1, std::tuple<_Tail...>>::type;
        };

    NS_END  // internal

    // tuple_type_at
    //   type trait: retrieves the type at a specific index in a tuple.
    template<std::size_t _Index,
              typename... _Types>
    struct tuple_type_at
    {
    private:
        using tuple_type = to_tuple_t<_Types...>;
        static_assert((_Index < std::tuple_size<tuple_type>::value),
                      "Non-type parameter `_Index` cannot be greater than or "
                      "equal to the tuple size of type parameter `_Tuple`.");

    public:
        using type = typename internal::tuple_type_at_helper<_Index, tuple_type>::type;
    };

    // tuple_type_at_t
    //   alias template: shorthand for `tuple_type_at<...>::type`.
    template<std::size_t _Index,
              typename... _Types>
    using tuple_type_at_t = typename tuple_type_at<_Index, _Types...>::type;

    // tuple_type_at_value
    //   function: retrieves the value at a specific index in a tuple instance.
    template<std::size_t _Index,
              typename    _Tuple>
    constexpr auto tuple_type_at_value(const _Tuple& _t)
    {
        return internal::tuple_type_at_helper<_Index, _Tuple>::value(_t);
    }

    // tuple_concat
    //   function: concatenates multiple tuples at compile-time.
    template<typename... _Tuples>
    static constexpr auto tuple_concat(_Tuples&&... _tuples)
    {
        return typename internal::tuple_join_helper<_Tuples...>::type{};
    }


    // =========================================================================
    // IV.  TYPE COUNTING AND FILTERING
    // =========================================================================

    // tuple_consolidate_types (internal helper)
    NS_INTERNAL

        template<typename _Tuple,
                  typename... _Result>
        struct tuple_consolidate_types_helper;

        template<typename... _Types>
        struct tuple_consolidate_types_helper<std::tuple<>, _Types...>
        {
            using type = std::tuple<_Types...>;
        };

        template<typename    _Head,
                  typename... _Tails,
                  typename... _Types>
        struct tuple_consolidate_types_helper<std::tuple<_Head, _Tails...>, _Types...>
        {
            using removed = tuple_count_and_remove<_Head, _Tails...>;
            using type = typename tuple_consolidate_types_helper<
                typename removed::type,
                _Types...,
                std::conditional_t<
                    (removed::value > 0),
                    std::array<_Head, removed::value + 1>,
                    _Head
                >
            >::type;
        };

    NS_END  // internal

    // tuple_consolidate_types
    //   type trait: consolidates repeated types in a tuple into arrays.
    template<typename... _Types>
    struct tuple_consolidate_types
    {
        using type = typename internal::tuple_consolidate_types_helper<
            typename to_tuple<_Types...>::type
        >::type;
    };

    // tuple_consolidate_types_t
    //   alias template: shorthand for `tuple_consolidate_types<...>::type`.
    template<typename... _Types>
    using tuple_consolidate_types_t = typename tuple_consolidate_types<_Types...>::type;

    // tuple_count_and_remove (internal helper)
    NS_INTERNAL

        template<typename    _Type,
                  typename    _Tuple,
                  std::size_t _Count,
                  typename    _Filtered>
        struct tuple_count_and_remove_helper;

        template<typename    _Type,
                  std::size_t _Count,
                  typename... _Filtered>
        struct tuple_count_and_remove_helper<_Type, std::tuple<>, _Count, std::tuple<_Filtered...>>
        {
            static constexpr std::size_t value = _Count;
            using type = std::tuple<_Filtered...>;
        };

        template<typename    _Type,
                  typename    _Head,
                  typename... _Tail,
                  std::size_t _Count,
                  typename... _Filtered>
        struct tuple_count_and_remove_helper<_Type, std::tuple<_Head, _Tail...>, _Count, std::tuple<_Filtered...>>
        {
            using recursive_type = std::conditional_t<
                std::is_same<_Type, _Head>::value,
                tuple_count_and_remove_helper<_Type, std::tuple<_Tail...>, _Count + 1, std::tuple<_Filtered...>>,
                tuple_count_and_remove_helper<_Type, std::tuple<_Tail...>, _Count, std::tuple<_Filtered..., _Head>>
            >;

            using type = typename recursive_type::type;
            static constexpr std::size_t value = recursive_type::value;
        };

    NS_END  // internal

    // tuple_count_and_remove
    //   type trait: counts occurrences of a type and removes them from a tuple.
    template<typename    _Type,
              typename... _Types>
    struct tuple_count_and_remove
    {
    protected:
        using _type = typename internal::tuple_count_and_remove_helper<
            _Type,
            typename to_tuple<_Types...>::type,
            0,
            std::tuple<>
        >;

    public:
        using type = typename _type::type;
        static constexpr std::size_t value = _type::value;
    };

    // tuple_count_and_remove_t
    //   alias template: shorthand for `tuple_count_and_remove<...>::type`.
    template<typename    _Type,
              typename... _Types>
    using tuple_count_and_remove_t = typename tuple_count_and_remove<_Type, _Types...>::type;

    // tuple_count_and_remove_v
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename    _Type,
              typename... _Types>
    constexpr std::size_t tuple_count_and_remove_v = tuple_count_and_remove<_Type, _Types...>::value;
#endif

    // tuple_count_type (internal helper)
    NS_INTERNAL

        template<typename    _Type,
                  typename    _Tuple,
                  std::size_t _Count>
        struct tuple_count_type_helper;

        // case: empty
        template<typename    _Type,
                  std::size_t _Count>
        struct tuple_count_type_helper<_Type, std::tuple<>, _Count>
        {
            static constexpr std::size_t value = 0;
        };

        // case: last element (or tuple of size 1)
        template<typename    _Type,
                  typename    _Head,
                  std::size_t _Count>
        struct tuple_count_type_helper<_Type, std::tuple<_Head>, _Count>
        {
            static constexpr std::size_t value = std::conditional_t<
                std::is_same<_Type, _Head>::value,
                std::integral_constant<std::size_t, _Count + 1>,
                std::integral_constant<std::size_t, _Count>
            >::value;
        };

        // case: recursive
        template<typename    _Type,
                  typename    _Head,
                  typename... _Tail,
                  std::size_t _Count>
        struct tuple_count_type_helper<_Type, std::tuple<_Head, _Tail...>, _Count>
        {
            static constexpr std::size_t value = tuple_count_type_helper<
                _Type,
                std::tuple<_Tail...>,
                std::conditional_t<
                    std::is_same<_Type, _Head>::value,
                    std::integral_constant<std::size_t, _Count + 1>,
                    std::integral_constant<std::size_t, _Count>
                >::value
            >::value;
        };

    NS_END  // internal

    // tuple_count_type
    //   type trait: counts the number of occurrences of a type in a tuple.
    template<typename    _Type,
              typename... _Types>
    struct tuple_count_type
    {
        static constexpr std::size_t value = internal::tuple_count_type_helper<
            _Type,
            typename to_tuple<_Types...>::type,
            0
        >::value;
    };

    // tuple_count_type_v
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename    _Type,
              typename... _Types>
    constexpr std::size_t tuple_count_type_v = tuple_count_type<_Type, _Types...>::value;
#endif


    // =========================================================================
    // V.   TUPLE SPLITTING
    // =========================================================================

    // tuple_split (internal helper)
    NS_INTERNAL

        template<std::size_t _Index,
                  typename    _Before,
                  typename    _After,
                  typename = void>
        struct tuple_split_helper;

        // index = 0
        template<typename... _Before,
                  typename... _After>
        struct tuple_split_helper<0, std::tuple<_Before...>, std::tuple<_After...>>
        {
            using before = std::tuple<_Before...>;
            using after  = std::tuple<_After...>;
        };

        // recursive case
        template<std::size_t _Index,
                  typename... _Before,
                  typename    _Type,
                  typename... _After>
        struct tuple_split_helper<_Index, std::tuple<_Before...>, std::tuple<_Type, _After...>,
                                  std::enable_if_t<(_Index > 0)>>
        {
            using type = tuple_split_helper<_Index - 1, std::tuple<_Before..., _Type>, std::tuple<_After...>>;

            using before = typename type::before;
            using after  = typename type::after;
        };

    NS_END  // internal

    // tuple_split
    //   type trait: splits a tuple at a specified index.
    // Given an index I and a tuple of type T with N elements:
    // 1. An index between 0 and N-1 results in `before` of size I and `after`
    //    of size (N-I).
    // 2. An index of 0 results in an empty `before` and `after` equal to T.
    // 3. An index of N results in `before` equal to T and an empty `after`.
    template<std::size_t _Index,
              typename... _Types>
    struct tuple_split
    {
    private:
        using tuple_type = to_tuple_t<_Types...>;
        static_assert((_Index <= std::tuple_size<tuple_type>::value),
                      "`_Index` must be less than or equal to the `std::tuple` size.");

    public:
        using type   = internal::tuple_split_helper<_Index, std::tuple<>, tuple_type>;
        using before = typename type::before;
        using after  = typename type::after;
    };

    // tuple_split_t
    //   alias template: shorthand for `tuple_split<...>::type`.
    template<std::size_t _Index,
              typename    _Tuple>
    using tuple_split_t = typename tuple_split<_Index, _Tuple>::type;

    // tuple_subsequence (internal helper)
    NS_INTERNAL

        template<std::size_t _Start,
                  std::size_t _End,
                  typename    _Tuple,
                  typename... _Types>
        struct tuple_subsequence_helper;

        // Base case: Start index reaches End index
        template<std::size_t _End,
                  typename    _Tuple,
                  typename... _Types>
        struct tuple_subsequence_helper<_End, _End, _Tuple, _Types...>
        {
            using type = std::tuple<_Types...>;
        };

        // Recursive case
        template<std::size_t _Start,
                  std::size_t _End,
                  typename    _Tuple,
                  typename    _First,
                  typename... _Rest>
        struct tuple_subsequence_helper<_Start, _End, _Tuple, _First, _Rest...>
        {
            using type = typename tuple_subsequence_helper<_Start + 1, _End, _Tuple, _Rest...>::type;
        };

    NS_END  // internal

    // tuple_subsequence
    //   type trait: extracts a subsequence from a tuple between two indices.
    template<std::size_t _Start,
              std::size_t _End,
              typename    _Tuple>
    struct tuple_subsequence;

    template<std::size_t _Start,
              std::size_t _End,
              typename... _Types>
    struct tuple_subsequence<_Start, _End, std::tuple<_Types...>>
    {
        static_assert(_Start <= _End,
                      "Start index must be less than or equal to End index.");
        static_assert(_End <= sizeof...(_Types),
                      "End index must be less than or equal to tuple size.");

        using type = typename internal::tuple_subsequence_helper<
            _Start,
            _End,
            std::tuple<_Types...>,
            _Types...
        >::type;
    };

    // tuple_subsequence_t
    //   alias template: shorthand for `tuple_subsequence<...>::type`.
    template<std::size_t _Start,
              std::size_t _End,
              typename    _Tuple>
    using tuple_subsequence_t = typename tuple_subsequence<_Start, _End, _Tuple>::type;


    // =========================================================================
    // VI.  TUPLE UTILITIES
    // =========================================================================

    // tuple_to_pack (internal helper)
    NS_INTERNAL

        template<typename       _Tuple,
                  typename       _Fn,
                  std::size_t... _I>
        void tuple_to_pack_helper(_Tuple&& _tuple,
                                  _Fn&&    _func,
                                  std::index_sequence<_I...>)
        {
            _func(std::get<_I>(_tuple)...);
        }

    NS_END  // internal

    // tuple_to_pack
    //   function: expands a tuple into a function call with the tuple elements
    // as arguments.
    template<typename _Tuple,
              typename _Fn>
    void tuple_to_pack(_Tuple&& _tuple,
                       _Fn&&    _func)
    {
        constexpr std::size_t N = std::tuple_size<typename std::decay<_Tuple>::type>::value;
        internal::tuple_to_pack_helper(
            std::forward<_Tuple>(_tuple),
            std::forward<_Fn>(_func),
            std::make_index_sequence<N>{}
        );
    }


    // =========================================================================
    // VII. TYPE SELECTION
    // =========================================================================

    // type_case
    //   type trait: represents a condition-type pair for use with type_selector.
    template<bool     _Condition,
              typename _Type>
    struct type_case
    {
        static constexpr bool condition = _Condition;
        using type = _Type;
    };

    // type_selector
    //   type trait: selects a type based on the first matching condition in a
    // sequence of type_case instances.
    template<typename... _TypeCases>
    struct type_selector
    {
        using type = void;  // Default when no type_cases match
        static constexpr bool matched = false;
    };

    // Base case: no type_cases left
    template<>
    struct type_selector<>
    {
        using type = void;
        static constexpr bool matched = false;
    };

    // Recursive case: process type_cases sequentially
    template<bool        _Condition,
              typename    _Type,
              typename... _RestTypeCases>
    struct type_selector<type_case<_Condition, _Type>, _RestTypeCases...>
    {
    private:
        using next_selector = type_selector<_RestTypeCases...>;

    public:
        using type = std::conditional_t<_Condition, _Type, typename next_selector::type>;
        static constexpr bool matched = (_Condition || next_selector::matched);
    };

    // type_select_t
    //   alias template: shorthand for `type_selector<...>::type`.
    template<typename... _TypeCases>
    using type_select_t = typename type_selector<_TypeCases...>::type;

    // type_matched_v
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename... _TypeCases>
    constexpr bool type_matched_v = type_selector<_TypeCases...>::matched;
#endif


    // =========================================================================
    // VIII. TUPLE HOMOGENEITY
    // =========================================================================

    // is_tuple_homogeneous
    //   type trait: evaluates whether all types in a tuple are the same.
    template<typename _Tuple>
    struct is_tuple_homogeneous : std::false_type
    {};

    template<typename _Type>
    struct is_tuple_homogeneous<std::tuple<_Type>> : std::true_type
    {};

    template<typename    _Type,
              typename    _Type2,
              typename... _Types>
    struct is_tuple_homogeneous<std::tuple<_Type, _Type2, _Types...>>
        : std::integral_constant<bool,
            std::is_same<_Type, _Type2>::value &&
            is_tuple_homogeneous<std::tuple<_Type2, _Types...>>::value>
    {};

    // is_tuple_homogeneous_v
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename _Tuple>
    constexpr bool is_tuple_homogeneous_v = is_tuple_homogeneous<_Tuple>::value;
#endif

    // is_homogeneous
    //   function: runtime helper to check tuple homogeneity.
    template<typename... _Types>
    static inline constexpr bool is_homogeneous(std::tuple<_Types...> const&)
    {
        return is_tuple_homogeneous<std::tuple<_Types...>>::value;
    }

NS_END  // djinterp


#endif  // DJINTERP_TUPLE_MANIP_
