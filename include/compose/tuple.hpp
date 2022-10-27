#ifndef LAMBDA_COMPOSITION_TUPLE_HPP
#define LAMBDA_COMPOSITION_TUPLE_HPP

#include <type_traits>
#include "xtraits.hpp"

namespace detail {
    template <std::size_t N, typename T>
    struct Tuple_leaf {
        static constexpr std::size_t value = N;
        using type = T;
        T data;

        constexpr T& get_self(Integral<N>&) {
            return data;
        }

        constexpr std::size_t self_index(Type<T>&) const {
            return N;
        }
    };

    template <typename /* sequence */, typename...> struct Tuple_elements;

    template <std::size_t...NN, typename...TT>
    struct Tuple_elements<std::index_sequence<NN...>, TT...> : Tuple_leaf<NN, TT>... {
    using Tuple_leaf<NN, TT>::get_self...;
    using Tuple_leaf<NN, TT>::self_index...;

    template <std::size_t N>
    constexpr decltype(auto) get_member(Integral<N>& i) {
        return this->get_self(i);
    }

    template <typename T>
    constexpr std::size_t get_index(Type<T>& t) const {
        return this->self_index(t);
    }
};

template <typename...TT>
Tuple_elements(TT&&...tt) -> Tuple_elements<Seq<sizeof...(TT)>, decltype(static_cast<TT>(tt))...>;
}

template <typename...TT>
struct Tuple {
    using self_type = detail::Tuple_elements< Seq<sizeof...(TT)>, TT... >;
    self_type data;

    static constexpr std::size_t count = sizeof...(TT);

    template <std::size_t N>
    constexpr decltype(auto) get(Integral<N> i) {
        static_assert(N < count, "Index out of range!");
        return data.get_member(i);
    }

    template <typename T>
    constexpr std::size_t get(Type<T> t) const {
        return data.get_index(t);
    }

    template <std::size_t N>
    constexpr decltype(auto) operator[](Integral<N> i) {
        static_assert(N < count, "Index out of range!");
        return data.get_member(i);
    }

    template <typename T>
    constexpr std::size_t operator[](Type<T> t) const {
        return data.get_index(t);
    }
};

template <typename...TT>
Tuple(TT&&...tt) -> Tuple<decltype(static_cast<TT>(tt))...>;

template <typename> struct is_tuple : std::false_type {};

template <typename...TT>
struct is_tuple<Tuple<TT...>> : std::true_type {};

template <typename T>
constexpr bool is_tuple_v = is_tuple<T>::value;

template <typename...TT>
constexpr auto tie(TT&&...tt) {
    return Tuple<TT...>{ tt... };
}

#endif //LAMBDA_COMPOSITION_TUPLE_HPP
