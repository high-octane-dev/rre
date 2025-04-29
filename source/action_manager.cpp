#include "action_manager.hpp"
#include "game.hpp"

ActionManager* lpASManager = nullptr;

// OFFSET: 0x005ce040, STATUS: WIP
ActionManager::ActionManager() : GameObject(true), action_infos(10), action_test_infos(10) {
    lpASManager = this;
    unk3 = 0;
    unk4 = 0;
    unk2 = 0;
    unk1 = 0;
}

// OFFSET: 0x005ce1a0, STATUS: COMPLETE
ActionManager::~ActionManager() {
    if (string_block_allocator != nullptr) {
        delete string_block_allocator;
        string_block_allocator = nullptr;
    }
}

// OFFSET: 0x0056ef30, STATUS: COMPLETE
int ActionManager::Tick(float deltaSeconds) {
    if ((lpGame->debug_flags & 8) != 0) {
        return 1;
    }

    return GameObject::Tick(deltaSeconds);
}

char EMPTY_STR[] = "";

// OFFSET: 0x005e7a20, STATUS: COMPLETE
ActionScript* ActionManager::CreateActionScript(char* unk_name, char* script_name, char* file_name) {
    ParameterBlock file{};
    if (file.OpenFile(file_name, 0, -1, nullptr, -1) != 0) {
        if (((unk_name == nullptr) || (*unk_name == '\0')) && ((script_name == nullptr || (*script_name == '\0')))) {
            script_name = EMPTY_STR;
            unk_name = file_name;
        }
        return CreateActionScript(unk_name, script_name, &file);
    }
    return nullptr;
}

// OFFSET: 0x005ce220, STATUS: WIP
ActionScript* ActionManager::CreateActionScript(char* unk_name, char* script_name, ParameterBlock* file) {
    char* pcVar4;

    pcVar4 = script_name;
    if ((unk_name != nullptr) && (*unk_name != '\0')) {
      pcVar4 = unk_name;
    }

    //RemoveActionScript(pcVar4);

    ActionScript* newActionScript = new ActionScript();

    newActionScript->ReadActionScript(script_name, file);
    return newActionScript;
}

// OFFSET: 0x005e7b80, STATUS: WIP
ActionSequence* ActionManager::CreateActionSequence(char* sequence_name, char* unk_name, char* file_name) {
    ParameterBlock file{};
    if (file.OpenFile(file_name, 0, -1, nullptr, -1) != 0) {
        if (((sequence_name == nullptr) || (*sequence_name == '\0')) && ((unk_name == nullptr || (*unk_name == '\0')))) {
            unk_name = EMPTY_STR;
            sequence_name = file_name;
        }
        return CreateActionSequence(sequence_name, unk_name, &file);
    }
    return nullptr;
}

// OFFSET: 0x005ce3d0, STATUS: WIP
ActionSequence* ActionManager::CreateActionSequence(char* param_1, char* param_2, ParameterBlock* param_3) {
    char* pcVar4;

    pcVar4 = param_2;
    if ((param_1 != nullptr) && (*param_1 != '\0')) {
        pcVar4 = param_1;
    }

    //RemoveActionSequence(pcVar4);

    ActionSequence* newActionSequence = new ActionSequence();
    newActionSequence->ReadActionScript(param_2,param_3);

    return newActionSequence;
}
