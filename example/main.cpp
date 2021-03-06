#include <fsm/actions/Nothing.hpp>
#include <fsm/actions/TransitionTo.hpp>
#include <fsm/actions/Maybe.hpp>
#include <fsm/actions/ByDefault.hpp>
#include <fsm/actions/On.hpp>
#include <fsm/actions/Will.hpp>
#include <fsm/StateMachine.hpp>

struct OpenEvent
{
};

struct CloseEvent
{
};

struct LockEvent
{
    uint32_t newKey;    
};

struct UnlockEvent
{
    uint32_t key;    
};

struct ClosedState;
struct OpenState;
class LockedState;

struct ClosedState : public Will<ByDefault<Nothing>,
                                 On<LockEvent, TransitionTo<LockedState>>,
                                 On<OpenEvent, TransitionTo<OpenState>>>
{
};

struct OpenState : public Will<ByDefault<Nothing>,
                               On<CloseEvent, TransitionTo<ClosedState>>>
{
};

class LockedState : public ByDefault<Nothing>
{
public:
    using ByDefault::handle;

    LockedState(uint32_t key)
        : key(key)
    {
    }

    void onEnter(const LockEvent& e)
    {
        key = e.newKey;
    }

    Maybe<TransitionTo<ClosedState>> handle(const UnlockEvent& e) const
    {
        if (e.key == key) {
            return TransitionTo<ClosedState>{};
        }
        return Nothing{};
    }

private:
    uint32_t key;
};

using Door = StateMachine<ClosedState, OpenState, LockedState>;

int main()
{
    Door door{ClosedState{}, OpenState{}, LockedState{0x11}};

    door.handle(LockEvent{1234});
    door.handle(UnlockEvent{2});
    door.handle(UnlockEvent{1234});

    return 0;
}
