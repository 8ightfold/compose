#ifndef LAMBDA_COMPOSITION_LAMBDA_HPP
#define LAMBDA_COMPOSITION_LAMBDA_HPP

#include <compose/detail/xtraits.hpp>
#include <compose/detail/tuple.hpp>

namespace compose {
    template <typename LHS>
    struct compose_base : lambda_base<LHS> {
    private:
        template <typename RHS, typename...TT, typename...UU>
        static constexpr auto deduce_return(const type_constant<tuple<UU...>>, type_sequence<TT...>)
            requires std::conjunction_v<std::is_convertible<TT, UU>...>
        {
            return [](TT&&...tt) {
                using tuple_type = decltype(RHS{}(std::forward<TT>(tt)...));
                tuple_type tup = RHS{}(std::forward<TT>(tt)...);
                return [&] <std::size_t...NN> (std::index_sequence<NN...>) {
                    return LHS{}( std::forward<UU>(tup[I<NN>])... );
                } (std::make_index_sequence<sizeof...(UU)>());
            };
        }



    public:
        using lambda_base<LHS>::operator();

        constexpr auto operator+(semilambda auto&& rhs) {
            using RHS = std::remove_cvref_t<decltype(rhs)>;
            using rhs_fn = decltype(&RHS::operator());
            using lhs_args = typename pointer_traits<decltype(&LHS::operator())>::args;

            return [] <typename T2, typename...TTs> (const type_constant<T2>, type_sequence<TTs...> ts) {
                using ret_t = std::remove_cvref_t<T2>;
                if constexpr(is_tuple_v<ret_t>) {
                    using new_t = decltype(deduce_return<RHS>(Tp<T2>, ts));
                    return compose_base<new_t>{};
                }
                else {
                    static_assert(sizeof...(TTs) == 1, "Invalid argument count.");
                    using new_t = decltype(
                        [](TTs&&...tt) {
                            return [&] <std::size_t...NN> (std::index_sequence<NN...>) {
                                auto ret_tup = RHS(std::forward<TTs>(tt)...);
                                return LHS{}( ret_tup[I<NN>]... );
                            } (seq_t<sizeof...(TTs)>{});
                        }
                    );
                    return compose_base<new_t>{};
                }
            } (Tp< typename pointer_traits<rhs_fn>::ret >, lhs_args{});
        }
    };

    template <typename T> compose_base(T&& t) -> compose_base<T>;
}

#endif //LAMBDA_COMPOSITION_LAMBDA_HPP
