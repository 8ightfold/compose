#ifndef COMPOSE_BASE_HPP
#define COMPOSE_BASE_HPP

#include <compose/detail/xtraits.hpp>
#include <compose/detail/compose_base.hpp>

namespace compose {
    struct start {
        constexpr auto operator%(semilambda auto&& rhs) const noexcept {
            using lambda_type = std::remove_cvref_t<decltype(rhs)>;
            return compose_base<lambda_type>{};
        }
    };

    inline constexpr start S {};
}

#endif //COMPOSE_BASE_HPP
