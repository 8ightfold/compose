#ifndef LAMBDA_COMPOSITION_LAMBDA_HPP
#define LAMBDA_COMPOSITION_LAMBDA_HPP

#include "tuple.hpp"

template <typename V>
struct Fn : public V {
    template <typename L>
    constexpr auto operator+(const Fn<L>& that) {
        using this_fn = decltype(&V::operator());
        using that_fn = decltype(&L::operator());
        using this_args = typename Pointer<this_fn>::args;
        using that_ret = typename Pointer<that_fn>::ret;

        return [] <typename T, typename...TTs>
                (Types<TTs...>, Type<T>) {
            using ret_t = std::remove_cvref_t<T>;
            if constexpr(is_tuple_v<ret_t>) {
                using new_t = decltype(
                [] <typename...UU> (Type<Tuple<UU...>>) {
                    constexpr bool is_valid = std::conjunction_v<std::is_convertible<TTs, UU>...>;
                    static_assert(is_valid, "Types are not convertible.");

                    return [](TTs&&...tt) {
                        decltype(L{}(std::forward<TTs>(tt)...)) tup = L{}(std::forward<TTs>(tt)...);
                        return [&] <std::size_t...NN> (std::index_sequence<NN...>) {
                            return V{}(std::forward<UU>(tup[I<NN>])...);
                        } (std::make_index_sequence<sizeof...(UU)>());
                    };
                } (Type<T>{})
                );
                return Fn<new_t>{};
            }
            else {
                static_assert(sizeof...(TTs) == 1, "Invalid argument count.");
                using new_t = decltype(
                    [](TTs&&...tt) { return V(L(std::forward<TTs>(tt)...)); }
                );
                return Fn<new_t>{};
            }
        } (this_args{}, Type<that_ret>{});
    }
};

template <typename T> Fn(T&& t) -> Fn<T>;

#endif //LAMBDA_COMPOSITION_LAMBDA_HPP
