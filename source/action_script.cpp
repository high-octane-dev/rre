#include "action_script.hpp"
#include <format>

ActionScript::ActionScript() : GameObject(true)
{
}

// OFFSET: 0x005cdd00
int ActionScript::Tick(float deltaSeconds)
{
    return GameObject::Tick(deltaSeconds);
}

// OFFSET: 0x0053aa50
int ActionScript::IsPlaying()
{
    return unk2 != 3;
}

// OFFSET: 0x005e78d0
void ActionScript::ReadActionScript(char* param_1, char* param_2)
{
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

// OFFSET: 0x005cdf10
void ActionScript::ReadActionScript(char* param_1, ParameterBlock* param_2)
{
    char local_50[64];

    snprintf(local_50, 64, "%sPreAction", param_1);
    ReadNonBodySequence(param_2, local_50);
    snprintf(local_50, 64, "%sPostAction", param_1);
    ReadNonBodySequence(param_2, local_50);
}

// OFFSET: 0x005cdd80
void ActionScript::ReadNonBodySequence(ParameterBlock* param_1, char* param_2)
{
    char local_30[32];

    snprintf(local_30, 32, "%s1", param_2);

    if (param_1->ReadParameterBlock(local_30) != 0)
    {
    }
}

// OFFSET: 0x005a0d90
void ActionScript::Reset()
{
    unk4 = unk4 | 1;
}

// OFFSET: 0x005cdd70
void ActionScript::Run()
{
    unk4 = unk4 & 0xfe;
    unk2 = -1;
}
