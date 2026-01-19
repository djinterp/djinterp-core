/******************************************************************************
* djinterp [core]                                              type_traits.hpp
*
*
* 
* path:      \inc\meta\tuple_util.hpp
* link:      TBA
* author(s): Samuel 'teer' Neal-Blim                          date: 2024.03.09
******************************************************************************/

#ifndef DJINTERP_TYPE_TRAITS_
#define	DJINTERP_TYPE_TRAITS_ 1

#include <memory>
#include <type_traits>
#include "..\djinterp.h"
#include ".\tuple_util.hpp"


// D_TYPE_TRAIT_DETECT_METHOD
//   Detects: TYPE::METHOD(TYPE::value_type)
#define D_TYPE_TRAIT_DETECT_METHOD(METHOD_NAME)     \
    decltype(std::declval<_Type&>().METHOD(std::declval<typename _Type::value_type>()))

// D_TYPE_TRAIT_DETECT_METHOD_ARGS
//   Detects: TYPE::METHOD(...args)
#define D_TYPE_TRAIT_DETECT_METHOD_ARGS(METHOD_NAME, ...)       \
    decltype(std::declval<_Type&>().METHOD_NAME(std::declval<__VA_ARGS__>()))

// D_TYPE_TRAIT_TRUE_FALSE
//   Creates a trait with true/false value based on detection expression
#define D_TYPE_TRAIT_TRUE_FALSE(TRAIT_NAME, DETECTION_EXPR)                 \
template<typename _Type,                                                    \
          typename = void>                                                  \
struct TRAIT_NAME : std::false_type                                         \
{};                                                                         \
                                                                            \
template<typename _Type>                                                    \
struct TRAIT_NAME<_Type, std::void_t<DETECTION_EXPR>> : std::true_type      \
{};

// D_TYPE_TRAIT_VALUE_BOOL
//   Adds an inline constexpr bool alias for the given trait
#define D_TYPE_TRAIT_VALUE_BOOL(TRAIT_NAME)                                 \
    template<typename _Type>                                           		\
    inline constexpr bool TRAIT_NAME##_v = TRAIT_NAME<_Type>::value;

// D_TYPE_TRAIT_TRUE
//   Combines true/false detection with an alias
#define D_TYPE_TRAIT_TRUE(TRAIT_NAME, DETECTION_EXPR)                       \
    D_TYPE_TRAIT_TRUE_FALSE(TRAIT_NAME, DETECTION_EXPR)                     \
    D_TYPE_TRAIT_VALUE_BOOL(TRAIT_NAME)

// D_TYPE_TRAIT_NEW_VOID_T
//   Like TRUE_FALSE but inherits from a user-specified trait
#define D_TYPE_TRAIT_NEW_VOID_T(TRAIT_NAME, DETECTION_EXPR, BASE_TRAIT)     \
template<typename _Type,                                                    \
          typename = void>                                                  \
struct TRAIT_NAME : std::false_type                                         \
{};                                                                         \
                                                                            \
template<typename _Type>                                                    \
struct TRAIT_NAME<_Type, std::void_t<DETECTION_EXPR>> : BASE_TRAIT<_Type>   \
{};

// D_TYPE_TRAIT_NEW
//   Full "master" trait with inheritance and alias
#define D_TYPE_TRAIT_NEW(TRAIT_NAME, DETECTION_EXPR, BASE_TRAIT)            \
    D_TYPE_TRAIT_NEW_VOID_T(TRAIT_NAME, DETECTION_EXPR, BASE_TRAIT)         \
    D_TYPE_TRAIT_VALUE_BOOL(TRAIT_NAME)

// D_TYPE_TRAIT_METHOD_CHECK
//   Trait detects if T has METHOD(T::value_type)
#define D_TYPE_TRAIT_METHOD_CHECK(TRAIT_NAME, METHOD_NAME)                  \
    D_TYPE_TRAIT_TRUE(TRAIT_NAME, D_TYPE_TRAIT_DETECT_METHOD(T, METHOD_NAME))

// D_TYPE_TRAIT_METHOD_CHECK_ARGS
//   Trait detects if T has METHOD(arg types...)
#define D_TYPE_TRAIT_METHOD_CHECK_ARGS(TRAIT_NAME, METHOD_NAME, ...)        \
    D_TYPE_TRAIT_TRUE(TRAIT_NAME,                                           \
                      D_TYPE_TRAIT_DETECT_METHOD_ARGS(METHOD_NAME, __VA_ARGS__))

// D_TYPE_TRAIT_HAS_METHOD_TYPED
//   macro: Trait detects if T::METHOD(args...) exists and returns RETURN_TYPE
#define D_TYPE_TRAIT_HAS_METHOD_TYPED(TRAIT_NAME, METHOD_NAME, RETURN_TYPE, ...)  \
    template<typename _Type,                                                      \
              typename = void>                                                    \
    struct TRAIT_NAME : std::false_type         
    {};                                         
                                                
                                                
    template<typename _Type>                   
    struct TRAIT_NAME<_Type, std::void_t<decltype(std::declval<_Type&>().METHOD_NAME(std::declval<__VA_ARGS__>()))>>  \
        : std::is_same<decltype(std::declval<_Type&>().METHOD_NAME(std::declval<__VA_ARGS__>())), RETURN_TYPE>        \
    {};											                                                                      \
                                                                                                                      \
    D_TYPE_TRAIT_VALUE_BOOL(TRAIT_NAME)

// For methods without arguments
#define HAS_METHOD_OF_TYPE(method_name, return_type)                        \
    std::enable_if_t<std::is_same_v<                                        \
        decltype(std::declval<_Type>().method_name()),                      \
        return_type>>

// For methods with arguments
#define HAS_METHOD_OF_TYPE_ARGS(method_name, return_type, ...)              \
    std::enable_if_t<std::is_same_v<                                        \
        decltype(std::declval<_Type>().method_name(__VA_ARGS__)),           \
        return_type>>

#define HAS_METHOD_OF_TYPE_V(T, method_name, return_type)                   \
    std::is_same_v<decltype(std::declval<T>().method_name()), return_type>

// For methods with arguments  
#define HAS_METHOD_OF_TYPE_ARGS_V(T, method_name, return_type, ...)         \
    std::is_same_v<decltype(std::declval<T>().method_name(__VA_ARGS__)), return_type>

NS_DJINTERP
	// forward declarations
	template<typename... _Types> struct to_tuple;
	template<typename _Type> struct to_tuple<_Type>;

	template<typename _Type> struct is_tuple;
	template<typename... _Types> struct is_tuple<std::tuple<_Types...>>;
	template<typename _Tuple, template<typename> typename _UnaryTrait, template<typename...> typename _Evaluator> struct evaluate_types_for_trait;
	template<typename... _Types, template<typename> typename _UnaryTrait, template<typename...> typename _Evaluator> struct evaluate_types_for_trait<std::tuple<_Types...>, _UnaryTrait, _Evaluator>;
	template<typename _Type> struct to_type;

	
	// I. Language feature traits
	// ---------------------------------------------------------------------------
	// (to be used as a supplement for those found in std header <type_traits>)

	// exclusive_disjunction
	//  Forms the exclusive logical disjunction of the type traits `B...`, effectively performing a logical XOR on the sequence of traits. 
	template<typename... _Bs>
	struct exclusive_disjunction : std::false_type 
	{};

	template<typename _B1>
	struct exclusive_disjunction<_B1> : _B1
	{};

	template<typename _B1, 
		     typename _B2>
	struct exclusive_disjunction<_B1, _B2> : 
		std::integral_constant<bool, _B1::value != _B2::value>
	{};

	template<typename    _B1,
		     typename    _B2, 
		     typename... _Bs>
	struct exclusive_disjunction<_B1, _B2, _Bs...> : 
		std::integral_constant<bool, (_B1::value != _B2::value) && exclusive_disjunction<_Bs...>::value>
	{};

	template<typename... _Bs>
	inline constexpr bool exclusive_disjunction_v = exclusive_disjunction<_Bs...>::value;

	// are_all_nonvoid
	// 
	template<typename... _Types>
	struct are_all_nonvoid : std::negation<evaluate_types_for_trait<typename to_tuple<_Types...>::type, std::is_void, std::disjunction>>
	{};

	template<typename _Type>
	struct are_all_nonvoid<_Type> : evaluate_types_for_trait<typename to_tuple<_Type>::type, std::is_void, std::disjunction>
	{}; 

	template<typename... _Types>
	inline constexpr bool are_all_nonvoid_v = are_all_nonvoid<_Types...>::value;

	// evaluate_all_for_trait
	NS_INTERNAL
		template<typename                       _Tuple, 
				 template<typename> typename    _UnaryTrait,
				 template<typename...> typename _Evaluator = std::conjunction>
		struct evaluate_all_for_trait_helper;

		template<typename                       _Type,
			     template<typename> typename    _UnaryTrait,
				 template<typename...> typename _Evaluator>
		struct evaluate_all_for_trait_helper<std::tuple<_Type>, _UnaryTrait, _Evaluator> : std::true_type
		{};

		template<typename                       _Type, 
				 typename...                    _Types, 
				 template<typename> typename    _UnaryTrait,
				 template<typename...> typename _Evaluator>
		struct evaluate_all_for_trait_helper<std::tuple<_Type, Types...>, _UnaryTrait, _Evaluator>
		{
			static constexpr bool value = _Evaluator<_UnaryTrait<_Type>, _UnaryTrait<Types>...>::value;
		};
	NS_END	// internal

	// evaluate_all_for_trait
	//   Evaluates all types in a parameter pack or `std::tuple` for a unary type 
	// trait.
	template<typename                       _Tuple,
			 template<typename> typename    _UnaryTrait,
			 template<typename...> typename _Evaluator = std::conjunction>
	struct evaluate_types_for_trait
	{
		static constexpr bool value = internal::evaluate_all_for_trait_helper<typename to_tuple<_Tuple>::type, _UnaryTrait, _Evaluator>::value;
	};

	template<typename...                    _Types,
			 template<typename> typename    _UnaryTrait,
			 template<typename...> typename _Evaluator>
	struct evaluate_types_for_trait<std::tuple<_Types...>, _UnaryTrait, _Evaluator>
	{
		static constexpr bool value = internal::evaluate_all_for_trait_helper<typename to_tuple<_Types...>::type, _UnaryTrait, _Evaluator>::value;
	};

	template<typename                       _Tuple,
			 template<typename> typename    _UnaryTrait,
			 template<typename...> typename _Evaluator>
	constexpr bool evaluate_types_for_trait_v = evaluate_types_for_trait<typename to_tuple<_Tuple>::type, _UnaryTrait, std::conjunction>::value;

	// follows_rule_of_zero
	//   type trait: returns true if, and only if, class `_Type` follows the 
	// Rule of Zero for class definitions: that all special member functions 
	//   1. copy constructor
	//   2. move constructor
	//   3. copy assignment operator
	//   4. move assignment operator
	//   5. destructor
	// are trivially implemented by the compiler
	template<typename _Type, 
			  typename = void>
	struct follows_rule_of_zero : std::false_type 
	{};

	template<typename _Type>
	struct follows_rule_of_zero<_Type, std::void_t<
		// trivial copy constructor
		std::enable_if_t<std::is_trivially_copy_constructible_v<_Type>>,
		// trivial move constructor  
		std::enable_if_t<std::is_trivially_move_constructible_v<_Type>>,
		// trivial copy assignment
		std::enable_if_t<std::is_trivially_copy_assignable_v<_Type>>,
		// trivial move assignment
		std::enable_if_t<std::is_trivially_move_assignable_v<_Type>>,
		// trivial destructor
		std::enable_if_t<std::is_trivially_destructible_v<_Type>>
		// note: we check for trivial operations since rule of zero means
		// the compiler-generated versions are sufficient
	>> : std::true_type
	{};

	// follows_rule_of_three
    //   type trait: returns true if, and only if, class `_Type` follows the 
	// Rule of Three for class definitions: that the class'
	//   1. copy constructor
	//   2. copy assignment operator
	//   3. destructor
	// are all defined
	template<typename _Type,
			 typename = void>
	struct follows_rule_of_three : std::false_type
	{};

	template<typename _Type>
	struct follows_rule_of_three<_Type, std::void_t<
		// copy constructor exists
		decltype(_Type(std::declval<const _Type&>())),
		// copy assignment exists and returns reference
		std::enable_if_t<std::is_same_v<
			decltype(std::declval<_Type&>() = std::declval<const _Type&>()),
			_Type&>>,
		// destructor check (always present)
		// decltype(std::declval<_Type>().~_Type())
	>> : std::true_type
	{};

	// follows_rule_of_five
	//   type trait: returns true if, and only if, class `_Type` follows the 
	// Rule of Five for class definitions: that the class'
	//   1. copy constructor
	//   2. move constructor
	//   3. copy assignment operator
	//   4. move assignment operator
	//   5. destructor
	// are all defined
	template<typename _Type,
		     typename = void>
	struct follows_rule_of_five : std::false_type
	{};

	template<typename _Type>
	struct follows_rule_of_five<_Type, std::void_t<
		// copy constructor
		decltype(_Type(std::declval<const _Type&>())),
		// move constructor
		decltype(_Type(std::declval<_Type&&>())),
		// copy assignment with correct return type
		std::enable_if_t<std::is_same_v<
			decltype(std::declval<_Type&>() = std::declval<const _Type&>()),
			_Type&>>,
		// move assignment with correct return type
		std::enable_if_t<std::is_same_v<
			decltype(std::declval<_Type&>() = std::declval<_Type&&>()),
			_Type&>>,
		// destructor check (always present)
		// decltype(std::declval<_Type>().~_Type())
	>> : std::true_type
	{};

	// has_max_size
    //   Trait: used to determine if a type has both a `size_type` alias, and a
	// `max_size` constexpr corresponding to that particular type.
	template<typename _Type,
		     typename = void>
	struct has_max_size : std::false_type
	{};

	template<typename _Type>
	struct has_max_size<_Type,
		std::void_t<
			typename _Type::size_type,
			decltype(_Type::max_size),
			std::enable_if_t<std::is_same_v<decltype(_Type::max_size), const typename _Type::size_type>>
		>> : std::true_type
	{};

	template<typename _Type>
	constexpr bool has_max_size_v = has_max_size<_Type>::value;

	// has_nested_template_type
	//   
	template<typename _Type,
		     typename = std::void_t<>>
	struct has_nested_template_type : std::false_type 
	{};

	template<typename _Type>
	struct has_nested_template_type<_Type, std::void_t<typename _Type::template type<int>>> : std::true_type 
	{};

	// has_nested_template_type_v
	// 
	template<typename _Type>
	inline constexpr bool has_nested_template_type_v = has_nested_template_type<_Type>::value;

	// has_variadic_constructor
	//
	template<typename _Type, 
		     typename = void>
	struct has_variadic_constructor : std::false_type
	{};

	template<typename _Type>
	struct has_variadic_constructor<_Type, std::void_t<decltype(_Type(std::declval<_Type>()))>> : std::true_type
	{};

	// has_variadic_constructor_v
	// 
	template<typename _Type>
	inline constexpr bool has_variadic_constructor_v = has_variadic_constructor<_Type>::value;

	// is_allocator
	// 
	template<typename _Type,
	         typename = void>
	struct is_allocator : std::false_type
	{};

	template<typename _Type>
	struct is_allocator<_Type, std::void_t<
		typename std::allocator_traits<_Type>::value_type,
		decltype(std::allocator_traits<_Type>::allocate(std::declval<_Type>(), std::size_t{})),
		decltype(std::allocator_traits<_Type>::deallocate(std::declval<_Type>(), nullptr, std::size_t{}))
	>> : std::true_type
	{};

	// is_allocator_v
	// 
	template<typename _Type>
	inline constexpr bool is_allocator_v = is_allocator<_Type>::value;

	// is_bounded
    //   
	template<typename                    _Type, 
			 template<typename...> class _Trait,
			 typename = void>
	struct is_bounded : std::false_type
	{};

	template<typename                    _Type, 
			 template<typename...> class _Trait>
	struct is_bounded<_Type, _Trait,
		std::void_t<
			std::enable_if_t<_Trait<_Type>::value>,
			std::enable_if_t<has_max_size<_Type>::value>
		>> : std::true_type
	{};

	// Helper variable template
	template<typename _Type, 
			 template<typename...> class _Trait>
	inline constexpr bool is_bounded_v = is_bounded<_Type, _Trait>::value;

	// is_nonvoid
	//   
	template<typename _Type,
		     typename = void>
	struct is_nonvoid : std::false_type
	{};

	template<typename _Type>
	struct is_nonvoid<_Type, std::enable_if_t<!std::is_void_v<_Type>>> : std::true_type
	{};

	// is_nonvoid_v
	// 
	template<typename _Type>
	inline constexpr bool is_nonvoid_v = is_nonvoid<_Type>::value;

	// is_single_arg_pack
	// 
	template<typename... _Types>
	struct is_single_arg : std::false_type
	{};

	template<typename _Type>
	struct is_single_arg<_Type> : std::true_type
	{
		using type = _Type;
	};

	// is_single_arg_v
	// 
	template<typename... _Types>
	inline constexpr bool is_single_arg_v = is_single_arg<_Types...>::value;

	// is_single_tuple_arg
	//   Returns true if, and only if, a parameter pack consists of a single
	// `std::tuple` argument.
	template<typename... _Types>
	struct is_single_tuple_arg : std::false_type
	{};

	template<typename _Type>
	struct is_single_tuple_arg<_Type>
	{
		static constexpr bool value = is_tuple<_Type>::value;
	};

	// is_single_tuple_arg_v
	// 
	template<typename... _Types>
	inline constexpr bool is_single_tuple_arg_v = is_single_tuple_arg<_Types...>::value;

	// is_single_type_arg
	//   Returns true if, and only if, a parameter pack consists of a single
	// argument of type `_Type`.
	template<typename    _Type,
		     typename... _Types>
	struct is_single_type_arg : std::conjunction<is_single_arg<_Types...>, std::is_same<typename is_single_arg<_Types...>::type, _Type>>
	{};

	template<typename... _Types>
	inline constexpr bool is_single_type_arg_v = is_single_type_arg<_Types...>::value;

	// is_sized
	//   type trait: evaluates whether or not the given type has all of the following conditions:
	//   - a `size_type` type alias
	//   - a size() function
	//   - the type of both `size_type` and `size()` is convertible to `std::size_t`
	template<typename,
			 typename = void>
	struct is_sized : std::false_type
	{};

	template<typename _Type>
	struct is_sized<_Type, std::void_t<
		typename _Type::size_type,
		decltype(std::declval<const _Type&>().size())>> 
		: std::conjunction<std::is_convertible<decltype(std::declval<const _Type&>().size()), std::size_t>, 
						   std::is_convertible<typename _Type::size_type, std::size_t>>
	{};

	// is_sized_v
	//   
	template<typename _Type>
	inline constexpr bool is_sized_v = is_sized<_Type>::value;

	// is_template
	//   type trait: evaluates a type for being a template template (i.e. 
	// `_Type` is itself a template with parameters).
	template<typename> 
	struct is_template : std::false_type
	{};

	template<template <typename...> typename _Type>
	struct is_template<_Type<>> : std::true_type 
	{};
	
	template<typename _Type>
	inline constexpr bool is_template_v = is_template<_Type>::value;

	// is_template_parameter_base_of
	//   type trait: evaluates a type type `_Type` contains a `value_type` 
	// member type alias that is itself a base of `_Type`.
	// Useful for dealing with polymorphic and composite-patterned class
	// template.
	template<typename _Type,
			 typename = void>
	struct is_template_parameter_base_of : std::false_type 
	{};

	template<typename _Type>
	struct is_template_parameter_base_of<_Type, std::void_t<
		typename _Type::value_type, 
		std::enable_if_t<std::is_base_of_v<typename _Type::value_type, _Type>>>
	> : std::true_type 
	{};

	template<typename _Type>
	inline constexpr bool is_template_parameter_base_of_v = is_template_parameter_base_of<_Type>::value;

	// is_template_with_args
	//   type trait: 
	template<typename> 
	struct is_template_with_args : std::false_type
	{};

	template<template <typename...> typename _Type,
	         typename...                     _Args>
	struct is_template_with_args<_Type<_Args...>> : std::true_type
	{};

	template<typename _Type>
	inline constexpr bool is_template_with_args_v = is_template_with_args<_Type>::value;

	// is_tuple
	//   type trait: 
	template<typename _Type>
	struct is_tuple : std::false_type
	{};

	template<typename... _Types>
	struct is_tuple<std::tuple<_Types...>> : std::true_type
	{};

	template<typename _Type>
	inline constexpr bool is_tuple_v = is_tuple<_Type>::value;

	// is_tuple_homogeneous
	//   type trait: 
	template<typename _Tuple>
	struct is_tuple_homogeneous : std::false_type
	{};

	template<typename _Type>
	struct is_tuple_homogeneous<std::tuple<_Type>> : std::true_type
	{};

	template<typename    _Type,
             typename... _Types>
	struct is_tuple_homogeneous<std::tuple<_Type, _Types...>> : std::conjunction<std::is_same<_Type, _Types>...>
	{};

	// is_tuple_homogeneous_v
	//   type trait: 
	template<typename... _Types>
	inline constexpr bool is_tuple_homogeneous_v = is_tuple_homogeneous<_Types...>::value;

	// is_homogeneous
	//   type trait: 
	template<typename... Types>
	static inline constexpr bool is_homogeneous(std::tuple<Types...> const&)
	{
		return is_tuple_homogeneous<Types...>::value;
	}

	// is_valid_size_type
	//   Returns if the following is allowable as a 
	template<typename _Type, 
		     typename = void>
	struct is_valid_size_type : std::false_type
	{};

	template<typename _Type>
	struct is_valid_size_type<_Type, std::enable_if<std::conjunction_v<std::is_unsigned<_Type>, std::is_arithmetic<_Type>>>> : std::true_type
	{};

	template<typename _Type>
	inline constexpr bool is_valid_size_type_v = is_valid_size_type<_Type>::value;

	template<std::size_t N>
	using is_zero = std::is_same<std::integral_constant<std::size_t, N>, std::integral_constant<std::size_t, 0>>;

	template<std::size_t N>
	using is_nonzero = std::negation<std::is_same<std::integral_constant<std::size_t, N>, std::integral_constant<std::size_t, 0>>>;

	template<std::size_t N>
	inline constexpr bool is_nonzero_v = std::negation<std::is_same<std::integral_constant<std::size_t, N>, std::integral_constant<std::size_t, 0>>>::value;

	// to_type	
	// 
	template<typename _Type>
	struct to_type
	{
		using type = _Type;
	};

NS_END	// djinterp


#endif	// DJINTERP_TYPE_TRAITS_