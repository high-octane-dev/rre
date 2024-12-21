#include "action_sequence.hpp"

GameObject* ActionSequence::Create()
{
    unk6 = unk6 & 0xfe;
    unk1 = 0;
    return this;
}

int ActionSequence::Tick(float deltaSeconds)
{
    return GameObject::Tick(deltaSeconds);
}
