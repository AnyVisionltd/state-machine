#pragma once

namespace fsm {

template <typename Event, typename Action>
struct On {
    Action handle(const Event&) const {
        return {};
    }
};

}  // End of namespace fsm
