#pragma once

#include <tuple>
#include <variant>

#include "tools/TupleTools.hpp"

namespace fsm {

template <class... States>
class StateMachine {
public:
    StateMachine() : currentState(&std::get<0>(states)) {
    }

    virtual ~StateMachine() noexcept {
    }

    StateMachine(States... states_in) : states(std::move(states_in)...) {
        currentState = &std::get<0>(states);
    }

    StateMachine(const StateMachine& other) : states(other.states) {
        currentState = runtime_get(states, other.currentState.index());
    }

    StateMachine(StateMachine&& other) : states(std::move(other.states)) {
        currentState = runtime_get(states, other.currentState.index());
    }

    StateMachine& operator=(const StateMachine& other) {
        if (this != &other) {
            states = other.states;
            currentState = runtime_get(states, other.currentState.index());
        }
        return *this;
    }

    StateMachine& operator=(StateMachine&& other) {
        if (this != &other) {
            states = std::move(other.states);
            currentState = runtime_get(states, other.currentState.index());
        }
        return *this;
    }

    template <typename State>
    State& transitionTo() {
        State& state = std::get<State>(states);
        currentState = &state;
        return state;
    }

    template <typename Event>
    void handle(const Event& event) {
        handleBy(event, *this);
    }

    template <typename Event, typename Machine>
    void handleBy(const Event& event, Machine& machine) {
        auto passEventToState = [&machine, &event](auto statePtr) {
            auto action = statePtr->handle(event);
            action.execute(machine, *statePtr, event);
        };
        std::visit(passEventToState, currentState);
    }

protected:
    std::tuple<States...> states;
    std::variant<States*...> currentState;
};

}  // End of namespace fsm
