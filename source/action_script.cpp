#include "action_script.hpp"
#include "action_manager.hpp"
#include "util/rsstring_util.hpp"

// OFFSET: 0x0056eee0, STATUS: WIP
ActionScript::ActionScript() : GameObject(true) {
}

// OFFSET: INLINE, STATUS: COMPLETE
ActionScript::~ActionScript() {
    if (unk4 != nullptr) {
        lpASManager->string_block_allocator->FreeString(unk4);
        unk4 = nullptr;
    }
}

// OFFSET: 0x005cdd00, STATUS: WIP
int ActionScript::Tick(float deltaSeconds) {
    return GameObject::Tick(deltaSeconds);
}

// OFFSET: 0x0053aa50, STATUS: COMPLETE
int ActionScript::IsPlaying() {
    return unk3 != 3;
}

// OFFSET: 0x005e78d0, STATUS: COMPLETE
void ActionScript::ReadActionScript(char* script_name, char* file_name) {
    ParameterBlock block{};
    if (block.OpenFile(file_name, 0, -1, nullptr, -1) != 0) {
        ReadActionScript(script_name, &block);
    }
}

// OFFSET: 0x005cdf10, STATUS: COMPLETE
void ActionScript::ReadActionScript(char* script_name, ParameterBlock* file) {
    char temp[64];

    RSStringUtil::Ssnprintf(temp, sizeof(temp), "%sPreAction", script_name);
    ReadNonBodySequence(file, temp);
    RSStringUtil::Ssnprintf(temp, sizeof(temp), "%sPostAction", script_name);
    ReadNonBodySequence(file, temp);
}

// OFFSET: 0x005cdd80, STATUS: WIP
void ActionScript::ReadNonBodySequence(ParameterBlock* param_1, char* param_2) {
    char temp[32];

    RSStringUtil::Ssnprintf(temp, sizeof(temp), "%s1", param_2);

    if (param_1->ReadParameterBlock(temp) != 0) {
    }
}

// OFFSET: 0x005a0d90, STATUS: WIP
void ActionScript::Reset() {
    unk5 = unk5 | 1;
}

// OFFSET: 0x005cdd70, STATUS: WIP
void ActionScript::Run() {
    unk5 = unk5 & 0xfe;
    unk3 = -1;
}
