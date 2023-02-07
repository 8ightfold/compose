#ifndef LAMBDA_COMPOSITION_TUPLE_HPP
#define LAMBDA_COMPOSITION_TUPLE_HPP

#include <type_traits>
#include <compose/detail/xtraits.hpp>

namespace compose {
    namespace detail {
        template <std::size_t N, typename T>
        struct tuple_leaf {
            static constexpr std::size_t value = N;
            using type = T;
            T data;

            constexpr T& at_index(index_constant<N>&) {
                return data;
            }

            constexpr std::size_t at_type(type_constant<T>&) const {
                return N;
            }
        };

        template <typename /* sequence */, typename...> struct tuple_elements;

        template <std::size_t...NN, typename...TT>
        struct tuple_elements<std::index_sequence<NN...>, TT...> : tuple_leaf<NN, TT>... {
            using tuple_leaf<NN, TT>::at_index...;
            using tuple_leaf<NN, TT>::at_type...;

            template <std::size_t N>
            constexpr decltype(auto) get_member(index_constant<N>& i) {
                return this->at_index(i);
            }

            template <typename T>
            constexpr std::size_t get_index(type_constant<T>& t) const {
                return this->at_type(t);
            }
        };

        template <typename...TT>
        tuple_elements(TT&&...tt) -> tuple_elements<seq_t<sizeof...(TT)>, decltype(static_cast<TT>(tt))...>;
    }

    template <typename...TT>
    struct tuple {
        using self_type = detail::tuple_elements< seq_t<sizeof...(TT)>, TT... >;
        self_type data;

        static constexpr std::size_t count = sizeof...(TT);

        template <std::size_t N>
        constexpr decltype(auto) get(index_constant<N> i) requires (N < count) {
            static_assert(N < count, "Index out of range!");
            return data.get_member(i);
        }

        template <typename T>
        constexpr std::size_t get(type_constant<T> t) const {
            return data.get_index(t);
        }

        template <std::size_t N>
        constexpr decltype(auto) operator[](index_constant<N> i) requires (N < count) {
            static_assert(N < count, "Index out of range!");
            return data.get_member(i);
        }

        template <typename T>
        constexpr std::size_t operator[](type_constant<T> t) const {
            return data.get_index(t);
        }
    };

    template <typename...TT>
    tuple(TT&&...tt) -> tuple<decltype(static_cast<TT>(tt))...>;


    template <typename> struct is_tuple : std::false_type {};

    template <typename...TT>
    struct is_tuple<tuple<TT...>> : std::true_type {};

    template <typename T>
    inline constexpr bool is_tuple_v = is_tuple<T>::value;

    template <typename...TT>
    constexpr auto tie(TT&&...tt) {
        return tuple<TT...>{ std::forward<TT>(tt)... };
    }
}

#endif //LAMBDA_COMPOSITION_TUPLE_HPP
