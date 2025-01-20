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

// OFFSET: 0x0056ef30, STATUS: COMPLETE
int ActionManager::Tick(float deltaSeconds) {
    if ((lpGame->debug_flags & 8) != 0) {
        return 1;
    }

    return GameObject::Tick(deltaSeconds);
}

// OFFSET: 0x005e7a20, STATUS: WIP
ActionScript* ActionManager::CreateActionScript(char* param_1, char* param_2, char* param_3) {
    char* local_860;
    ParameterBlock local_85c;

    local_860 = param_3;

    ActionScript *pActionScript;

    if (local_85c.OpenFile(param_3, 0, -1, nullptr, -1) == 0)
    {
        local_85c.FreeKeyAndHeaderMemory();
        pActionScript = nullptr;
    }
    else
    {
        if (((param_1 == nullptr) || (*param_1 == '\0')) && ((param_2 == nullptr || (*param_2 == '\0'))))
        {
            param_2 == "";
            param_1 = local_860;
        }
        pActionScript = CreateActionScript(param_1, param_2, &local_85c);
        local_85c.FreeKeyAndHeaderMemory();
    }
    return pActionScript;
}

// OFFSET: 0x005ce220, STATUS: WIP
ActionScript* ActionManager::CreateActionScript(char* param_1, char* param_2, ParameterBlock* param_3) {
    char* pcVar4;

    pcVar4 = param_2;
    if ((param_1 != nullptr) && (*param_1 != '\0')) {
      pcVar4 = param_1;
    }

    //RemoveActionScript(pcVar4);

    ActionScript* newActionScript = new ActionScript();

    newActionScript->ReadActionScript(param_2,param_3);
    return newActionScript;
}

// OFFSET: 0x005e7b80, STATUS: WIP
ActionSequence* ActionManager::CreateActionSequence(char* param_1, char* param_2, char* param_3) {
    char* local_860;
    ParameterBlock local_85c;  
    
    local_860 = param_3;

    ActionSequence *pActionSequence;
    if (local_85c.OpenFile(param_3, 0, -1, nullptr, -1) == 0)
    {
        local_85c.FreeKeyAndHeaderMemory();
        pActionSequence = nullptr;
    }
    else
    {
        if (((param_1 == nullptr) || (*param_1 == '\0')) && ((param_2 == nullptr || (*param_2 == '\0'))))
        {
            param_2 == "";
            param_1 = local_860;
        }
        pActionSequence = CreateActionSequence(param_1, param_2, &local_85c);
        local_85c.FreeKeyAndHeaderMemory();
    }
    return pActionSequence;
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
