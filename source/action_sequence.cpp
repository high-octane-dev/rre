#include "action_sequence.hpp"

ActionSequence::ActionSequence() : GameObject(true), action_sequence_steps(10)
{
}

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

void ActionSequence::ReadActionScript(char*, ParameterBlock*)
{
}
