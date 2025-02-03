#include <d3d9.h>
#include "x360_game.hpp"
#include "gfx/x360_video_card.hpp"
#include "gfx/x360_shader_manager.hpp"
#include "gfx/d3d_state_manager.hpp"
#include "gfx/cursor.hpp"
#include "gfx/x360_render_target.hpp"

// importing from main.cpp
extern HWND g_HWNDReal;
extern int g_WindowWidth;
extern int g_WindowHeight;
extern float g_TargetFrameRate;

unsigned int g_ViewportWidth = 0;
unsigned int g_ViewportHeight = 0;
int g_ScreenEffectWidth = 0;
int g_ScreenEffectHeight = 0;
int g_CameraWidth = 0;
int g_CameraHeight = 0;
int g_ScreenMode = 2;
int PlatformGraphics_BackBufferViewport_OffsetX = 0;
int PlatformGraphics_BackBufferViewport_OffsetY = 0;
float g_UIScaleX = 1.0f;
float g_UIScaleY = 1.0f;
float g_ViewportFadeWidth = 1280.0f;
float g_ViewportFadeHeight = 720.0f;
float g_AspectRatios[3] = {
    4.0f / 3.0f,
    16.0f / 9.0f,
    16.0f / 9.0f
};

// OFFSET: 0x00559f20, STATUS: COMPLETE
void SetDisplayModeGlobal(DisplayMode* display_mode) {
    g_TargetFrameRate = display_mode->fps;
    if (display_mode->field0_0x0 == 0 && display_mode->screen_mode != 2) {
        g_ScreenMode = 1;
        g_ScreenEffectWidth = 640;
        g_ViewportWidth = 640;
        if (display_mode->fps <= 54.97f) {
            g_ViewportHeight = 576;
        }
        else {
            g_ViewportHeight = 480;
        }
    }
    else {
        g_ScreenEffectWidth = 1280;
        g_ScreenMode = 2;
        g_ViewportWidth = 1280;
        g_ViewportHeight = 720;
    }
    g_ScreenEffectHeight = g_ViewportHeight;
    g_CameraWidth = g_ScreenEffectWidth;
    g_CameraHeight = g_ViewportHeight;
    PlatformGraphics_BackBufferViewport_OffsetX = 0;
    PlatformGraphics_BackBufferViewport_OffsetY = 0;
    if (g_ScreenMode == 1) {
        g_UIScaleX = g_ViewportWidth / 640.0f;
        g_UIScaleY = g_ViewportHeight / 448.0f;
    }
    else {
        g_UIScaleX = g_ViewportWidth / 1280.0f;
        g_UIScaleY = g_ViewportHeight / 720.0f;
    }
    g_AspectRatios[1] = 16.0f / 9.0f;
    g_AspectRatios[2] = 16.0f / 9.0f;
    return;
}

// OFFSET: 0x00421cd0, STATUS: COMPLETE
X360Game::X360Game() : Game() {
    unused17 = 0;
    d3d9 = nullptr;
    unused18 = 0;
    unused19 = 0;
    unused20 = 0;
    hwnd = nullptr;
}

// OFFSET: 0x00421d10, STATUS: TODO
X360Game::~X360Game() {
}

// OFFSET: 0x00421ee0, STATUS: WIP
int X360Game::InitializeRenderer(char(&quit_message)[260]) {
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d9 != nullptr) {
        // FUN_004134e0(); // This function is responsible for initializing a shit-load of globals.
        g_VideoCard = new X360VideoCard();
        if (g_VideoCard != nullptr) {
            g_VideoCard->Create();
            Game::Initialize();
            g_lpX360ShaderManager = new X360ShaderManager();
            if (g_lpX360ShaderManager != nullptr) {
                g_lpX360ShaderManager->Create();
                SetDisplayMode(nullptr);
                return 1;
            }
        }
    }
    return 0;
}

// OFFSET: 0x00421d90, STATUS: COMPLETE
int X360Game::PreGameInitialize(DisplayMode* desired_display_mode) {
    hwnd = g_HWNDReal;
    UpdateLocalizedPaths();
    if (desired_display_mode == nullptr) {
        DisplayMode display_mode{ .field0_0x0 = 0, .screen_mode = 1, .fps = 60.0 };
        SetDisplayModeGlobal(&display_mode);
        g_ViewportWidth = g_WindowWidth;
        g_ViewportHeight = g_WindowHeight;
        g_ScreenEffectWidth = g_WindowWidth;
        g_ScreenEffectHeight = g_WindowHeight;
        g_CameraWidth = g_WindowWidth;
        g_CameraHeight = g_WindowHeight;
        PlatformGraphics_BackBufferViewport_OffsetX = 0;
        PlatformGraphics_BackBufferViewport_OffsetY = 0;
        if (g_ScreenMode == 1) {
            g_UIScaleX = g_WindowWidth / 640.0f;
            g_UIScaleY = g_WindowHeight / 448.0f;
            UpdateTextureContentDirectories();
            Game::PreGameInitialize(nullptr);
            return 1;
        }
        g_UIScaleX = g_WindowWidth / 1280.0f;
        g_UIScaleY = g_WindowHeight / 720.0f;
    }
    else {
        SetDisplayModeGlobal(desired_display_mode);
    }
    if (g_ScreenMode == 2) {
        g_ViewportFadeWidth = 1280.0;
        g_ViewportFadeHeight = 720.0;
    }
    UpdateTextureContentDirectories();
    Game::PreGameInitialize(nullptr);
    return 1;
}

// OFFSET: 0x004221c0, STATUS: WIP
int X360Game::SetBasicRenderStates() {
    g_lpD3DStateManager->SetRenderState(D3DRS_ZENABLE, flags >> 6 & 1);
    g_lpD3DStateManager->SetRenderState(D3DRS_ZWRITEENABLE, flags >> 6 & 1);
    g_lpD3DStateManager->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
    g_lpD3DStateManager->SetRenderState(D3DRS_ALPHABLENDENABLE, 0);
    g_lpD3DStateManager->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
    g_lpD3DStateManager->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    g_lpD3DStateManager->SetRenderState(D3DRS_ALPHAREF, 0);
    for (std::size_t i = 0; i < g_VideoCard->capabilities.MaxSimultaneousTextures; i++) {
        if (i < 6) {
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);

            if (i > 5) {
                g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
                g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
                g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
            }
            else {
                g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
                g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
                g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
            }
        }
        else if (i < 7) {
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);

            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        }
        else {
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_BORDER);

            if (i == 7) {
                g_D3DDevice9->SetSamplerState(i, D3DSAMP_BORDERCOLOR, 0xFFFFFFFF);
            }
            else {
                g_D3DDevice9->SetSamplerState(i, D3DSAMP_BORDERCOLOR, 0);
            }

            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            g_lpD3DStateManager->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
        }
    }
    // FIXME: Handle Lights and EcoSystem
    if (g_RenderTarget->used_camera != nullptr) {
        g_D3DDevice9->SetVertexShaderConstantF(24, &g_RenderTarget->used_camera->position.x, 1);
        g_D3DDevice9->SetPixelShaderConstantF(9, &g_RenderTarget->used_camera->position.x, 1);
    }
    // FIXME: Handle Fog and WorldShadowMap
    if (g_RenderTarget->used_camera != nullptr) {
        if (g_RenderTarget->used_camera->enable_perspective == 0) {
            g_D3DDevice9->SetTexture(5, g_RenderTarget->unk_texture);
        }
    }
    // FIXME: Handle DistanceFade
    return 1;
}

// OFFSET: 0x00423060, STATUS: TODO
void X360Game::PrepareFrame() {
}

// OFFSET: 0x00422010, STATUS: TODO
int X360Game::KeyUpHandler(ProcessNode*, KeyInfo*) {
    return 0;
}

// OFFSET: 0x00422030, STATUS: TODO
int X360Game::KeyDownHandler(ProcessNode*, KeyInfo*) {
    return 0;
}

// OFFSET: 0x00422fd0, STATUS: WIP
int X360Game::Terminate() {
    int return_value = Game::Terminate();
    if (g_RenderTarget != nullptr) {
        delete g_RenderTarget;
        g_RenderTarget = nullptr;
    }
    if (g_VideoCard != nullptr) {
        g_VideoCard->FreeCursor();
        delete g_VideoCard;
        g_VideoCard = nullptr;
    }
    if (g_lpX360ShaderManager != nullptr) {
        delete g_lpX360ShaderManager;
        g_lpX360ShaderManager = nullptr;
    }
    if (d3d9 != nullptr) {
        d3d9->Release();
    }
    return 0;
}

// OFFSET: 0x00422050, STATUS: COMPLETE
int X360Game::SetDisplayMode(DisplayMode* mode) {
    if (mode != nullptr) {
        InitPlatformGraphicsCallback(mode);
    }
    if (g_VideoCard->ResetStateManager() == 0) {
        return 0;
    }
    g_lpCursor = new Cursor();
    return PostDisplayModeChange() != 0;
}

// OFFSET: 0x00422110, STATUS: COMPLETE
RenderTarget* X360Game::CreateRenderTarget() {
    if (g_RenderTarget != nullptr) {
        g_RenderTarget->SetDimensions(g_VideoCard);
        return g_RenderTarget;
    }
    g_RenderTarget = new X360RenderTarget();
    g_RenderTarget->Create(g_VideoCard);
    g_RenderTarget->SetDimensions(g_VideoCard);
    return g_RenderTarget;
}
