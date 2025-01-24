#include "action_sequence.hpp"

// OFFSET: 0x0056edc0, STATUS: WIP
ActionSequence::ActionSequence() : GameObject(true), action_sequence_steps(10) {
}

// OFFSET: 0x005bb590, STATUS: WIP
GameObject* ActionSequence::Create() {
    unk6 = unk6 & 0xfe;
    unk1 = 0;
    return this;
}

// OFFSET: 0x005bb5b0, STATUS: WIP
int ActionSequence::Tick(float deltaSeconds) {
    return GameObject::Tick(deltaSeconds);
}

// OFFSET: 0x005cda50, STATUS: TODO
void ActionSequence::ReadActionScript(char*, ParameterBlock*) {
}
