#include "x360_game.hpp"
#include "gfx/x360_video_card.hpp"
#include "gfx/x360_shader_manager.hpp"
#include <d3d9.h>

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
            }
        }
    }
}
