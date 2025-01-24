#include "action_script.hpp"
#include <format>

// OFFSET: 0x0056eee0, STATUS: WIP
ActionScript::ActionScript() : GameObject(true) {
}

// OFFSET: 0x005cdd00, STATUS: WIP
int ActionScript::Tick(float deltaSeconds) {
    return GameObject::Tick(deltaSeconds);
}

// OFFSET: 0x0053aa50, STATUS: WIP
int ActionScript::IsPlaying() {
    return unk2 != 3;
}

// OFFSET: 0x005e78d0, STATUS: WIP
void ActionScript::ReadActionScript(char* param_1, char* param_2) {
    ParameterBlock local_85c;

    if (local_85c.OpenFile(param_2, 0, -1, nullptr, -1) == 0)
    {
        local_85c.FreeKeyAndHeaderMemory();
    }
    else
    {
        ReadActionScript(param_1, &local_85c);
        local_85c.FreeKeyAndHeaderMemory();
    }
}

// OFFSET: 0x005cdf10, STATUS: COMPLETE
void ActionScript::ReadActionScript(char* param_1, ParameterBlock* file) {
    char temp[64];

    snprintf(temp, sizeof(temp), "%sPreAction", param_1);
    ReadNonBodySequence(file, temp);
    snprintf(temp, sizeof(temp), "%sPostAction", param_1);
    ReadNonBodySequence(file, temp);
}

// OFFSET: 0x005cdd80, STATUS: WIP
void ActionScript::ReadNonBodySequence(ParameterBlock* param_1, char* param_2) {
    char temp[32];

    snprintf(temp, sizeof(temp), "%s1", param_2);

    if (param_1->ReadParameterBlock(temp) != 0)
    {
    }
}

// OFFSET: 0x005a0d90, STATUS: WIP
void ActionScript::Reset() {
    unk4 = unk4 | 1;
}

// OFFSET: 0x005cdd70, STATUS: WIP
void ActionScript::Run() {
    unk4 = unk4 & 0xfe;
    unk2 = -1;
}
