#pragma once

#include "Nothing.hpp"
#include "OneOf.hpp"

namespace fsm {

template <typename Action>
struct Maybe : public OneOf<Action, Nothing> {
    using OneOf<Action, Nothing>::OneOf;
};

}  // End of namespace fsm
