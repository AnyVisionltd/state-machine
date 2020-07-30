#pragma once

namespace fsm {

template <typename Action>
struct ByDefault {
    template <typename Event>
    Action handle(const Event&) const {
        return Action{};
    }
};

}  // End of namespace fsm
