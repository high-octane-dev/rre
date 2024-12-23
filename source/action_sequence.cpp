#include "action_sequence.hpp"

ActionSequence::ActionSequence() : GameObject(true), action_sequence_steps(10)
{
}

// OFFSET: 0x005bb590
GameObject* ActionSequence::Create()
{
    unk6 = unk6 & 0xfe;
    unk1 = 0;
    return this;
}

// OFFSET: 0x005bb5b0
int ActionSequence::Tick(float deltaSeconds)
{
    return GameObject::Tick(deltaSeconds);
}

// OFFSET: 0x005cda50
void ActionSequence::ReadActionScript(char*, ParameterBlock*)
{
}
