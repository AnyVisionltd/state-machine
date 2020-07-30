#pragma once

namespace fsm {

struct Nothing {
    template <typename Machine, typename State, typename Event>
    void execute(Machine&, State&, const Event&) {
    }
};

}  // End of namespace fsm
