/*******************************************************************************
* djinterp [core]                                               type_traits.hpp
*
* This is a test file for `type_traits.hpp` unit tests.
* For the file itself, go to `\inc\type_traits.hpp`.
*
* author(s): Samuel 'teer' Neal-Blim
* link:   TBA
* file:   \test\type_traits_tests.hpp                          date: 2024.05.03
*******************************************************************************/
#include <iostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include "..\inc\type_traits.hpp"


using namespace djinterp;

// test_evaluate_types_for_trait
//   Tests the `evaluate_types_for_trait` trait, which evaluates all types in a
// parameter pack or `std::tuple` for a unary type trait.
// 
void test_evaluate_types_for_trait()
{
    // test 01: regular case (single-argument tuple type)
    using tuple01 = std::tuple<int>;
    using test01  = evaluate_types_for_trait<tuple01, std::is_arithmetic, std::conjunction>;
    static_assert(test01::value,
        "test assertion failed: a `std::tuple` consisting of a single `int` type should return true for trait `std::is_arithmetic`.");

    // test 02: regular case (single-argument tuple type)
    using tuple02 = std::tuple<std::string, std::string, std::string>;
    using test02 = evaluate_types_for_trait<tuple02, std::is_copy_assignable, std::disjunction>;
    static_assert(test02::value,
        "test assertion failed: a `std::tuple` consisting of one (1) or more `void` types should return true for trait `std::is_void`.");

    // test 03: regular case (single-argument tuple type)
    using tuple03 = std::tuple<int, long, float, long long&, short, char, void, std::string**>;
    using test03 = evaluate_types_for_trait<tuple03, std::is_void, std::disjunction>;
    static_assert(test03::value,
        "test assertion failed: a `std::tuple` consisting of one (1) or more `void` types should return true for trait `std::is_void`.");
}

// test_is_single_arg
//   Tests the `is_single_arg` trait, which determines if a parameter pack 
// consists of a single argument.
void test_is_single_arg()
{
    // empty case
    static_assert(is_single_arg<>::value == false,
        "test assertion failed: `is_single_arg` should not inherit `std::true_type` when containing 0 arguments.");

    // empty case (but with `is_single_arg_v` helper type)
    static_assert(is_single_arg_v<> == false,
        "test assertion failed: `is_single_arg_v` should return `false` when containing 0 arguments.");

    // regular case
    static_assert(is_single_arg<std::string>::value == true,
        "test assertion failed: `is_single_arg` should inherit `std::true_type` when given a single argument.");

    // regular case (but with `is_single_arg_v` helper type)
    static_assert(is_single_arg_v<std::string*> == true,
        "test assertion failed: `is_single_arg_v` should return `true` when given a single argument.");

    // parameter pack (false) case
    static_assert(is_single_arg<int, float, void, long>::value == false,
        "test assertion failed: `is_single_arg` should inherit `std::false_type` when given multiple arguments.");

    // parameter pack (false) case (but with `is_single_arg_v` helper type)
    static_assert(is_single_arg_v<int, float, void, long> == false,
        "test assertion failed: `is_single_arg_v` should return `false` when given multiple arguments.");
}

// test_is_single_type_arg
//   Tests the `is_single_type_arg` trait, which determines if a parameter pack 
// consists of a single argument that is also a specific type.
//   See also: `is_single_arg`.
void test_is_single_type_arg()
{
    // empty case
    static_assert(is_single_type_arg<int>::value == false,
        "test assertion failed: `is_single_type_arg` should not inherit `std::true_type` when containing 0 parameter pack arguments.");

    // empty case (but with `is_single_type_arg_v` helper type)
    static_assert(is_single_type_arg_v<int> == false,
        "test assertion failed: `is_single_type_arg_v` should return `false` when containing 0 parameter pack arguments.");

    // regular case
    static_assert(is_single_type_arg<std::string**, std::string**>::value == true,
        "test assertion failed: `is_single_type_arg` should inherit `std::true_type` when given a single parameter pack argument.");

    // regular case (but with `is_single_type_arg_v` helper type)
    static_assert(is_single_type_arg_v<std::string**, std::string**> == true,
        "test assertion failed: `is_single_type_arg_v` should return `true` when given a single parameter pack argument.");

    // 
    static_assert(is_single_type_arg<int, int, void, long>::value == false,
        "test assertion failed: `is_single_type_arg` should inherit `std::false_type` when given multiple parameter pack arguments.");

    // regular case (but with `is_single_type_arg_v` helper type)
    static_assert(is_single_type_arg_v<int, float, void, long> == false,
        "test assertion failed: `is_single_type_arg_v` should return `false` when given multiple parameter pack arguments.");
}

// test_is_single_tuple_arg
//   Tests the `is_single_type_arg` trait, which determines if a parameter pack 
// consists of a single argument that is also a specific type.
//   See also: `is_single_tuple_arg`.
void test_is_single_tuple_arg()
{
    // empty case
    static_assert(is_single_tuple_arg<>::value == false,
        "test assertion failed: `is_single_tuple_arg` should not inherit `std::true_type` when containing 0 arguments.");
    static_assert(is_single_tuple_arg_v<> == false,
        "test assertion failed: `is_single_tuple_arg_v` should return `false` when containing 0 arguments.");

    // regular case
    using tuple01 = std::tuple<int, float, long>;
    static_assert(is_single_tuple_arg<tuple01>::value == true,
        "test assertion failed: `is_single_tuple_arg` should inherit `std::true_type` when given a single argument.");

    // regular case (but with `is_single_type_arg_v` helper type)
    static_assert(is_single_tuple_arg_v<tuple01> == true,
        "test assertion failed: `is_single_tuple_arg_v` should return `true` when given a single argument.");

    // 
    using tuple02 = std::tuple<char>;
    static_assert(is_single_tuple_arg<tuple02, int, long, char>::value == false,
        "test assertion failed: `is_single_tuple_arg` should inherit `std::false_type` when given multiple parameter pack arguments.");

    // regular case (but with `is_single_type_arg_v` helper type)
    static_assert(is_single_tuple_arg_v<tuple02, int, long, char> == false,
        "test assertion failed: `is_single_tuple_arg_v` should return `false` when given multiple parameter pack arguments.");
}

// test_is_nonvoid_trait
//   Tests the `is_nonvoid` trait
void test_is_nonvoid_trait()
{
    // 
    static_assert(!is_nonvoid_v<void>,
        "test assertion failed: `is_nonvoid_v` cannot return true for void-type parameter.");

    static_assert(is_nonvoid_v<short*>,
        "test assertion failed: `is_nonvoid_v` cannot return true for void-type parameter.");

    static_assert(is_nonvoid_v<std::string&>,
        "test assertion failed: `is_nonvoid_v` cannot return true for void-type parameter.");
}

// test_are_all_nonvoid_trait
// 
void test_are_all_nonvoid_trait()
{
    // test 01: single, non-void parameter
    using tuple01 = std::tuple<int>;
    static_assert(are_all_nonvoid<tuple01>::value,
        "test assertion failed: `are_all_nonvoid_v` must return true for a single non-void type.");

    
}

// test_is_tuple
//   Tests functionality of the `is_tuple` type trait, which determines if a 
// given type is `std::tuple`.
//   And its `constexpr bool` counterpart `is_tuple_v`.
void test_is_tuple()
{    
    // test #1: regular case (single-argument `std::tuple` type)
    using test_type1 = std::tuple<int>;
    static_assert(is_tuple<test_type1>::value,
        "test assertion failed: `is_tuple` is supposed to inherit `std::true_type` for `std::tuple<int>`.");
    static_assert(is_tuple_v<test_type1> == is_tuple<test_type1>::value,
        "test assertion failed: `is_tuple_v` is supposed to return the same value as `is_tuple<...>::value` for the given input.");

    // test #2: empty case (zero (0) argument `std::tuple` type)
    using test_type2 = std::tuple<>;
    static_assert(is_tuple<test_type2>::value,
        "test assertion failed: `is_tuple` is supposed to inherit `std::true_type` for `std::tuple<>`.");
    static_assert(is_tuple_v<test_type2> == is_tuple<test_type2>::value,
        "test assertion failed: `is_tuple_v` is supposed to return the same value as `is_tuple<...>::value` for the given input.");

    // test #3:
    using test_type3 = int;
    static_assert(is_tuple<test_type3>::value == false,
        "test assertion failed: `is_tuple` is supposed to inherit `std::false_type` for non-tuple type `int`.");
    static_assert(is_tuple_v<test_type3> == is_tuple<test_type3>::value,
        "test assertion failed: `is_tuple_v` is supposed to return the same value as `is_tuple<...>::value` for the given input.");

    // test #4:
    using test_type4 = std::string;
    static_assert(!is_tuple<test_type4>::value,
        "test assertion failed: `is_tuple` is supposed to inherit `std::false_type` for non-tuple `std::string`.");
    static_assert(is_tuple_v<test_type4> == is_tuple<test_type4>::value,
        "test assertion failed: `is_tuple_v` is supposed to return the same value as `is_tuple<...>::value` for the given input.");

    // test #5:
    using test_type5 = std::tuple<int, float, double>;
    static_assert(is_tuple<test_type5>::value,
        "test assertion failed: `is_tuple` is supposed to inherit `std::true_type` for `std::tuple<int, float, double>*`.");
    static_assert(is_tuple_v<test_type5> == is_tuple<test_type5>::value,
        "test assertion failed: `is_tuple_v` is supposed to return the same value as `is_tuple<...>::value` for the given input.");

    // test #6:
    using test_type6 = std::tuple<int, float, double>*;
    static_assert(!is_tuple<test_type6>::value,
        "test assertion failed: `is_tuple` is supposed to inherit `std::false_type` for `std::tuple<int, float, double>*`.");
    static_assert(is_tuple_v<test_type6> == is_tuple<test_type6>::value,
        "test assertion failed: `is_tuple_v` is supposed to return the same value as `is_tuple<...>::value` for the given input.");
}

// test_is_tuple_homogeneous
//   Tests the compile-time `is_tuple_homogeneous` type trait, which determines 
// if a given type is a `std::tuple` containing all identical types.
//   And its `constexpr bool` counterpart `test_is_tuple_homogeneous_v`.
void test_is_tuple_homogeneous()
{
    // test #01: regular case (single-argument `std::tuple` type)
    using test_type01 = std::tuple<int>;
    static_assert(is_tuple_homogeneous<test_type01>::value,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::true_type` for single-argument tuple type `std::tuple<int>`.");
    static_assert(is_tuple_homogeneous_v<test_type01> == is_tuple_homogeneous<test_type01>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");

    // test #02: regular case (multiple-argument `std::tuple` type)
    using test_type02 = std::tuple<float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>;
    static_assert(is_tuple_homogeneous<test_type02>::value,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::true_type` for tuple-type `std::tuple<float...>`.");
    static_assert(is_tuple_homogeneous_v<test_type02> == is_tuple_homogeneous<test_type02>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");

    // test #03: empty case (zero-argument `std::tuple` type)
    using test_type03 = std::tuple<>;
    static_assert(is_tuple_homogeneous<test_type03>::value,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::true_type` for empty tuple-type `std::tuple<>`.");
    static_assert(is_tuple_homogeneous_v<test_type03> == is_tuple_homogeneous<test_type03>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");

    // test #04: negative case (2-argument `std::tuple` type)
    using test_type04 = std::tuple<float, char>;
    static_assert(is_tuple_homogeneous<test_type04>::value == false,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::false_type` for tuple-type `std::tuple<float, char>`.");
    static_assert(is_tuple_homogeneous_v<test_type04> == is_tuple_homogeneous<test_type04>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");

    // test #05: negative case (first arugment differs)
    using test_type05 = std::tuple<char, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float>;
    static_assert(is_tuple_homogeneous<test_type05>::value == false,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::false_type` for tuple-type `std::tuple<char, float...>`.");
    static_assert(is_tuple_homogeneous_v<test_type05> == is_tuple_homogeneous<test_type05>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");

    // test #06: negative case (last arugment differs)
    using test_type06 = std::tuple<float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, char>;
    static_assert(is_tuple_homogeneous<test_type06>::value == false,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::false_type` for tuple-type `std::tuple<...float, char>`.");
    static_assert(is_tuple_homogeneous_v<test_type06> == is_tuple_homogeneous<test_type06>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");

    // test #07: negative case (first AND last arugment differs)
    using test_type07 = std::tuple<char, float, float, float, float, float, float, float, float, float, float, float, float, float, float, char>;
    static_assert(is_tuple_homogeneous<test_type07>::value == false,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::false_type` for tuple-type `std::tuple<char, ...float..., char>`.");
    static_assert(is_tuple_homogeneous_v<test_type07> == is_tuple_homogeneous<test_type07>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");

    // test #08: negative case (middle arugment differs)
    using test_type08 = std::tuple<float, float, float, float, float, float, float, float, char, float, float, float, float, float, float, float>;
    static_assert(is_tuple_homogeneous<test_type08>::value == false,
        "test assertion failed: `is_tuple_homogeneous` is supposed to inherit `std::false_type` for tuple-type `std::tuple<..., float, char, float...>`.");
    static_assert(is_tuple_homogeneous_v<test_type08> == is_tuple_homogeneous<test_type08>::value,
        "test assertion failed: `is_tuple_homogeneous_v` is supposed to return the same value as `is_tuple_homogeneous<...>::value` for the given input.");
}


int main(int argc, char** argv)
{
    // test: evaluate_all_for_trait
    test_evaluate_types_for_trait();

    // test: is_single_arg, is_single_arg_v
    test_is_single_arg();

    // test: is_nonvoid, is_nonvoid_v, are_all_nonvoid, are_all_nonvoid_v
    test_is_nonvoid_trait();
    test_are_all_nonvoid_trait();

    // test: is_tuple, is_tuple_v
    test_is_tuple();

    // test: is_tuple_homogeneous, is_tuple_homogeneous_v
    test_is_tuple_homogeneous();
}