#include <d3d9.h>
#include "x360_game.hpp"
#include "gfx/x360_video_card.hpp"
#include "gfx/x360_shader_manager.hpp"

X360Game::~X360Game()
{
}

int X360Game::InitializeRenderer(const char*)
{
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d9 != nullptr) {
        g_VideoCard = new X360VideoCard();
        if (g_VideoCard != nullptr) {
            g_VideoCard->Create();
            Game::Initialize();
            lpX360ShaderManager = new X360ShaderManager();
            if (lpX360ShaderManager != nullptr) {
                lpX360ShaderManager->Create();
                this->SetDisplayMode(nullptr);
                return 1;
            }
        }
    }
    return 0;
}

int X360Game::PreGameInitialize(DisplayMode*)
{
    return 0;
}

int X360Game::SetBasicRenderStates()
{
    return 0;
}

void X360Game::PrepareFrame()
{
}

int X360Game::KeyUpHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

int X360Game::KeyDownHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

int X360Game::Terminate()
{
    return 0;
}

int X360Game::SetDisplayMode(DisplayMode*)
{
    return 0;
}

RenderTarget* X360Game::CreateRenderTarget()
{
    return nullptr;
}
