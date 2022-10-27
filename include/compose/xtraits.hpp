#ifndef LAMBDA_COMPOSITION_XTRAITS_HPP
#define LAMBDA_COMPOSITION_XTRAITS_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

template <std::size_t N>
using Integral = std::integral_constant<std::size_t, N>;

template <std::size_t N>
using Seq = decltype(std::make_index_sequence<N>());

template <std::size_t N>
constexpr auto I = Integral<N>{};

template <typename T>
struct Type {
    using type = T;
};

template <typename...TT>
struct Types {};

template <typename U>
constexpr auto T = Type<U>{};

template <typename> struct Pointer;

template <typename R, typename...Args>
struct Pointer<R(*)(Args...)> {
    using ret = R;
    using base = void;
    using args = Types<Args...>;
};

template <typename R, typename O, typename...Args>
struct Pointer<R(O::*)(Args...)> {
    using ret = R;
    using base = O;
    using args = Types<Args...>;
};

template <typename R, typename O, typename...Args>
struct Pointer<R(O::*)(Args...) const> {
    using ret = R;
    using base = O;
    using args = Types<Args...>;
};

#endif //LAMBDA_COMPOSITION_XTRAITS_HPP
