#include "action_script.hpp"
#include <format>

int ActionScript::Tick(float deltaSeconds)
{
    return GameObject::Tick(deltaSeconds);
}

int ActionScript::IsPlaying()
{
    return unk2 != 3;
}

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

void ActionScript::ReadActionScript(char* param_1, ParameterBlock* param_2)
{
    char local_50[64];

    snprintf(local_50, 64, "%sPreAction", param_1);
    ReadNonBodySequence(param_2, local_50);
    snprintf(local_50, 64, "%sPostAction", param_1);
    ReadNonBodySequence(param_2, local_50);
}

void ActionScript::ReadNonBodySequence(ParameterBlock* param_1, char* param_2)
{
    char local_30[32];

    snprintf(local_30, 32, "%s1", param_2);

    if (param_1->ReadParameterBlock(local_30) != 0)
    {
    }
}

void ActionScript::Reset()
{
    unk4 = unk4 | 1;
}

void ActionScript::Run()
{
    unk4 = unk4 & 0xfe;
    unk2 = -1;
}
