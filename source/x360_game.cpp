#include <d3d9.h>
#include "x360_game.hpp"
#include "gfx/x360_video_card.hpp"
#include "gfx/x360_shader_manager.hpp"

// OFFSET: 0x00421d10
X360Game::~X360Game()
{
}

// OFFSET: 0x00421ee0
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

// OFFSET: 0x00421d90
int X360Game::PreGameInitialize(DisplayMode*)
{
    return 0;
}


// OFFSET: 0x004221c0
int X360Game::SetBasicRenderStates()
{
    return 0;
}

// OFFSET: 0x00423060
void X360Game::PrepareFrame()
{
}

// OFFSET: 0x00422010
int X360Game::KeyUpHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

// OFFSET: 0x00422030
int X360Game::KeyDownHandler(ProcessNode*, KeyInfo*)
{
    return 0;
}

// OFFSET: 0x00422fd0
int X360Game::Terminate()
{
    return 0;
}

// OFFSET: 0x00422050
int X360Game::SetDisplayMode(DisplayMode*)
{
    return 0;
}

// OFFSET: 0x00422110
RenderTarget* X360Game::CreateRenderTarget()
{
    return nullptr;
}
