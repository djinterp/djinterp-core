/******************************************************************************
* djinterp [meta]                                               tuple_util.hpp
*
* djinterp tuple utility header:
*   This header provides tuple-related type trait utilities and metafunctions
* for compile-time tuple manipulation. It includes:
*   - type extraction (first_arg)
*   - tuple detection (is_tuple, is_single_tuple_arg)
*   - tuple construction (to_tuple, make_tuple_of, repeat)
*   - type modifiers (wrap_all, to_lvalue_reference, to_pointer, etc.)
*
*   PORTABILITY:
*   This header uses env.h for C++ version detection and cpp_features.h for
* fine-grained feature detection. All metafunctions are designed to work with
* C++11 and later, using portable trait access patterns.
* 
* path:      \inc\meta\tuple_util.hpp
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2024.04.25
******************************************************************************/

#ifndef DJINTERP_TUPLE_UTILITY_
#define DJINTERP_TUPLE_UTILITY_ 1

#include <tuple>
#include <type_traits>
#include "..\djinterp.h"
#include "..\cpp_features.h"


NS_DJINTERP

    // =========================================================================
    // I.   FORWARD DECLARATIONS
    // =========================================================================

    template<typename... _Types>
    struct first_arg;

    template<typename _Type>
    struct first_arg<_Type>;

    template<typename _Type,
             typename... _Types>
    struct first_arg<_Type, _Types...>;

    template<typename... _Types>
    struct is_single_arg;

    template<typename _Type>
    struct is_single_arg<_Type>;

    template<typename _Type>
    struct is_tuple;

    template<typename... _Types>
    struct is_tuple<std::tuple<_Types...>>;

    template<typename... _Types>
    struct is_single_tuple_arg;

    template<typename _Type>
    struct is_single_tuple_arg<_Type>;

    template<typename... _Types>
    struct to_tuple;

    template<typename _Type>
    struct to_tuple<_Type>;

    template<template<typename> typename... _Modifiers>
    struct wrap_all;

    template<template<typename> typename    _Modifier,
             template<typename> typename... _Modifiers>
    struct wrap_all<_Modifier, _Modifiers...>;

    template<>
    struct wrap_all<>;


    // =========================================================================
    // II.  PARAMETER PACK UTILITIES
    // =========================================================================

    // first_arg
    //   type trait: given a parameter pack, determines the first parameter.
    // Member alias `type` is the type of the first argument in the pack.
    template<typename... _Types>
    struct first_arg;

    template<typename _Type>
    struct first_arg<_Type>
    {
        using type = _Type;
    };

    template<typename    _Type,
              typename... _Types>
    struct first_arg<_Type, _Types...>
    {
        using type = _Type;
    };

    // first_arg_t
    //   alias template: shorthand for `first_arg<_Types...>::type`.
    template<typename... _Types>
    using first_arg_t = typename first_arg<_Types...>::type;


    // =========================================================================
    // III. TUPLE DETECTION TRAITS
    // =========================================================================

    // is_tuple
    //   type trait: evaluates to `std::true_type` if `_Type` is a `std::tuple`,
    // otherwise `std::false_type`.
    template<typename _Type>
    struct is_tuple : std::false_type
    {};

    template<typename... _Types>
    struct is_tuple<std::tuple<_Types...>> : std::true_type
    {};

    // is_tuple_v
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename _Type>
    constexpr bool is_tuple_v = is_tuple<_Type>::value;
#endif

    // is_single_tuple_arg
    //   type trait: evaluates to `std::true_type` if the parameter pack
    // consists of exactly one argument that is itself a `std::tuple`.
    template<typename... _Types>
    struct is_single_tuple_arg : std::conditional<
        (sizeof...(_Types) == 1),
        is_tuple<typename std::tuple_element<0, std::tuple<_Types...>>::type>,
        std::false_type
    >::type
    {};

    // is_single_tuple_arg_v
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename... _Types>
    constexpr bool is_single_tuple_arg_v = is_single_tuple_arg<_Types...>::value;
#endif


    // =========================================================================
    // IV.  NUMERIC UTILITIES
    // =========================================================================

    // abs_value
    //   type trait: computes the absolute value of a compile-time integral
    // constant.
    template<typename _Type,
              _Type    _N>
    struct abs_value
    {
        static_assert(std::is_integral<_Type>::value,
                      "Type parameter `_Type` must be an integral type.");

        static constexpr _Type value = (_N < 0) ? -(_N) : _N;
    };

    // abs_value_v
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename _Type,
              _Type    _N>
    constexpr _Type abs_value_v = abs_value<_Type, _N>::value;
#endif

    // abs_value_to_size_t
    //   
#if D_ENV_CPP_FEATURE_LANG_VARIABLE_TEMPLATES
    template<typename _Type,
              _Type    _N>
    constexpr std::size_t abs_value_to_size_t =
        std::integral_constant<std::size_t, abs_value<_Type, _N>::value>::value;
#endif


    // =========================================================================
    // V.   TUPLE CONSTRUCTION
    // =========================================================================

    // to_tuple
    //   type trait: converts a parameter pack to a `std::tuple`.
    // If the parameter pack consists of a single `std::tuple` type, `to_tuple`
    // resolves to that tuple type directly. Otherwise, all arguments are
    // wrapped in a new `std::tuple`.
    template<typename... _Types>
    struct to_tuple
    {
        using type = typename std::conditional<
            is_single_tuple_arg<_Types...>::value,
            typename first_arg<_Types...>::type,
            std::tuple<_Types...>
        >::type;
    };

    template<typename _Type>
    struct to_tuple<_Type>
    {
        using type = typename std::conditional<
            is_tuple<_Type>::value,
            _Type,
            std::tuple<_Type>
        >::type;
    };

    // to_tuple_t
    //   alias template: shorthand for `to_tuple<_Types...>::type`.
    template<typename... _Types>
    using to_tuple_t = typename to_tuple<_Types...>::type;

    // make_tuple_of
    //   type trait: creates a `std::tuple` containing `_Count` copies of
    // `_Type`.
    template<typename    _Type,
              std::size_t _Count>
    struct make_tuple_of;

    template<typename _Type>
    struct make_tuple_of<_Type, 0>
    {
        using type = std::tuple<>;
    };

    template<typename _Type>
    struct make_tuple_of<_Type, 1>
    {
        using type = to_tuple_t<_Type>;
    };

    template<typename    _Type,
              std::size_t _Count>
    struct make_tuple_of
    {
    private:
        template<std::size_t... _Indices>
        static auto make_impl(std::index_sequence<_Indices...>)
        {
            // Use comma operator to expand _Indices but always produce _Type
            return to_tuple_t<decltype((_Indices, std::declval<_Type>()))...>{};
        }

    public:
        using type = decltype(make_impl(std::make_index_sequence<_Count>{}));
    };

    // make_tuple_of_t
    //   alias template: shorthand for `make_tuple_of<_Type, _Count>::type`.
    template<typename    _Type,
              std::size_t _Count>
    using make_tuple_of_t = typename make_tuple_of<_Type, _Count>::type;

    // repeat (internal helper)
    NS_INTERNAL

        template<typename      _Type,
                  std::size_t   _N,
                  typename... _Types>
        struct repeat_type_helper
        {
            using type = typename repeat_type_helper<_Type, _N - 1, _Type, _Types...>::type;
        };

        template<typename    _Type,
                  typename... _Types>
        struct repeat_type_helper<_Type, 0, _Types...>
        {
            using type = std::tuple<_Types...>;
        };

    NS_END  // internal

    // repeat
    //   type trait: creates a `std::tuple` by repeating `_Type` exactly
    // `_NumTimes` times.
    template<typename    _Type,
              std::size_t _NumTimes>
    struct repeat
    {
        using type = typename std::conditional<
            (_NumTimes > 0),
            typename internal::repeat_type_helper<_Type, _NumTimes>::type,
            std::tuple<>
        >::type;
    };

    // repeat_t
    //   alias template: shorthand for `repeat<_Type, _NumTimes>::type`.
    template<typename    _Type,
              std::size_t _NumTimes>
    using repeat_t = typename repeat<_Type, _NumTimes>::type;


    // =========================================================================
    // VI.  TYPE MODIFIERS
    // =========================================================================

    // wrap_all
    //   type trait: applies a series of type transformations left-to-right,
    // where right is the innermost and left is the outermost.
    // Example: wrap_all<X, Y, Z>::template type<int> == X<Y<Z<int>>>
    template<template<typename> typename... _Modifiers>
    struct wrap_all
    {
        template<typename _Type>
        using type = _Type;
    };

    template<template<typename> typename    _Modifier,
              template<typename> typename... _Modifiers>
    struct wrap_all<_Modifier, _Modifiers...>
    {
        template<typename _Type>
        using type = typename _Modifier<
            typename wrap_all<_Modifiers...>::template type<_Type>
        >::type;
    };

    template<>
    struct wrap_all<>
    {
        template<typename _Type>
        using type = _Type;
    };

    // wrap_all_t
    //   alias template: shorthand for applying wrap_all.
    template<typename                        _Type,
              template<typename> typename... _Modifiers>
    using wrap_all_t = typename wrap_all<_Modifiers...>::template type<_Type>;

    // to_lvalue_reference
    //   type modifier: converts a type to an lvalue reference, removing any
    // existing reference first.
    struct to_lvalue_reference
    {
        template<typename _Type>
        using type = typename wrap_all<
            std::add_lvalue_reference,
            std::remove_reference
        >::template type<_Type>;
    };

    // to_rvalue_reference
    //   type modifier: converts a type to an rvalue reference, removing any
    // existing reference first.
    struct to_rvalue_reference
    {
        template<typename _Type>
        using type = typename wrap_all<
            std::add_rvalue_reference,
            std::remove_reference
        >::template type<_Type>;
    };

    // to_pointer
    //   type modifier: converts a type to a pointer, removing any existing
    // pointer first.
    struct to_pointer
    {
        template<typename _Type>
        using type = typename wrap_all<
            std::add_pointer,
            std::remove_pointer
        >::template type<_Type>;
    };

    // to_type
    //   type trait: identity wrapper that simply exposes a `type` member alias.
    // Useful for metaprogramming contexts where a type wrapper is expected.
    template<typename _Type>
    struct to_type
    {
        using type = _Type;
    };

    // to_type_t
    //   alias template: shorthand for `to_type<_Type>::type`.
    template<typename _Type>
    using to_type_t = typename to_type<_Type>::type;

    // tuple_all_elements_same_as
    template<typename _Tuple, 
             typename _Type>
    struct tuple_all_elements_same_as;

    template<typename   _Type, 
            typename... _Es>
    struct tuple_all_elements_same_as<std::tuple<_Es...>, _T>
        : std::bool_constant<(std::is_same_v<clean_t<_Es>, _T> && ...)>
    {};

    // all_inner_tuple_elements_one_type
    template<typename _Outer>
    struct all_inner_tuple_elements_one_type : std::false_type
    {};

    template<>
    struct all_inner_tuple_elements_one_type<std::tuple<>> : std::true_type
    {};

    // case: first inner is empty => true iff all inners are empty tuples
    template<typename... _Ts>
    struct all_inner_tuple_elements_one_type<std::tuple<std::tuple<>, _Ts...>>
        : std::bool_constant<(std::is_same_v<normalize_tuple_t<std::tuple<>>, normalize_tuple_t<_Ts>> && ...)>
    {};

    // case: first inner non-empty => all inners non-empty and all elements match first element type
    template<typename    _E0
             typename... _Erest, 
             typename... _Inners>
    struct all_inner_tuple_elements_one_type<std::tuple<std::tuple<_E0, _Erest...>, _Inners...>>
    {
    private:
        using first_elem_t = clean_t<_E0>;

        template<typename _Inner>
        static constexpr bool inner_ok =
            ( is_tuple<clean_t<_Inner>>::value                   &&
              (std::tuple_size_v<normalize_tuple_t<_Inner>> > 0) &&
              tuple_all_elements_same_as<normalize_tuple_t<_Inner>, first_elem_t>::value )

    public:
        // check first inner also (it is known tuple<...> here)
        static constexpr bool value = tuple_all_elements_same_as<std::tuple<_E0, _Erest...>,
                                                                 first_elem_t>::value && 
                                                                 (inner_ok<_Inners> && ...);
    };

    template<typename _Outer>
    inline constexpr bool all_inner_tuple_elements_one_type_v = all_inner_tuple_elements_one_type<_Outer>::value;

NS_END  // djinterp


#endif  // DJINTERP_TUPLE_UTILITY_
