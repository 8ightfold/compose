#ifndef LAMBDA_COMPOSITION_XTRAITS_HPP
#define LAMBDA_COMPOSITION_XTRAITS_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

namespace compose {
    template <std::size_t N>
    using index_constant = std::integral_constant<std::size_t, N>;

    template <std::size_t N>
    using seq_t = decltype(std::make_index_sequence<N>());

    template <std::size_t N>
    inline constexpr auto I = index_constant<N>{};


    template <typename T>
    struct type_constant {
        using type = T;
    };

    template <typename...TT>
    struct type_sequence {};

    template <typename U>
    inline constexpr auto Tp = type_constant<U>{};


    template <typename> struct pointer_traits;

    template <typename R, typename...Args>
    struct pointer_traits<R(*)(Args...)> {
        using ret = R;
        using base = void;
        using args = type_sequence<Args...>;
    };

    template <typename R, typename O, typename...Args>
    struct pointer_traits<R(O::*)(Args...)> {
        using ret = R;
        using base = O;
        using args = type_sequence<Args...>;
    };

    template <typename R, typename O, typename...Args>
    struct pointer_traits<R(O::*)(Args...) const> {
        using ret = R;
        using base = O;
        using args = type_sequence<Args...>;
    };

    template <typename T>
    struct lambda_base : public T {
        using T::operator();
    };

    template <typename T>
    lambda_base(T&&) -> lambda_base<std::remove_cvref_t<T>>;

    namespace detail {
        template <typename T>
        concept semilambda = requires { &T::operator(); };
    }

    template <typename T>
    concept semilambda = detail::semilambda<std::remove_cvref_t<T>>;
}

using compose::I;
using compose::Tp;

#endif //LAMBDA_COMPOSITION_XTRAITS_HPP
