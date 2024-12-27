#include <d3d9.h>
#include "x360_game.hpp"
#include "gfx/x360_video_card.hpp"
#include "gfx/x360_shader_manager.hpp"

// importing from main.cpp
extern HWND g_HWNDReal;
extern int g_WindowWidth;
extern int g_WindowHeight;
extern float g_TargetFrameRate;

int g_ViewportWidth = 0;
int g_ViewportHeight = 0;
int g_ScreenEffectWidth = 0;
int g_ScreenEffectHeight = 0;
int g_CameraWidth = 0;
int g_CameraHeight = 0;
int g_ScreenMode = 2;
int PlatformGraphics_BackBufferViewport_OffsetX = 0;
int PlatformGraphics_BackBufferViewport_OffsetY = 0;
float g_UIScaleX = 1.0;
float g_UIScaleY = 1.0;
float g_ViewportFadeWidth = 1080.0;
float g_ViewportFadeHeight = 720.0;
float g_CameraAspect = 1.0;
float g_CameraAspect2 = 1.0;

void SetDisplayModeGlobal(DisplayMode* display_mode) {
    g_TargetFrameRate = display_mode->fps;
    if (display_mode->field0_0x0 == 0 && display_mode->screen_mode != 2) {
        g_ScreenMode = 1;
        g_ScreenEffectWidth = 640;
        g_ViewportWidth = 640;
        if (display_mode->fps <= 54.97) {
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
        g_UIScaleX = g_ViewportWidth / 640.0;
        g_UIScaleY = g_ViewportHeight / 448.0;
    }
    else {
        g_UIScaleX = g_ViewportWidth / 1280.0;
        g_UIScaleY = g_ViewportHeight / 720.0;
    }
    g_CameraAspect = 1.777778;
    g_CameraAspect2 = 1.777778;
    return;
}

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
int X360Game::PreGameInitialize(DisplayMode* desired_display_mode) {
    hwnd = g_HWNDReal;
    UpdateLocalizedPaths();
    if (desired_display_mode == nullptr) {
        DisplayMode display_mode{ .field0_0x0 = 0, .screen_mode = 1, .fps = 60.0 };
        // (*(this->Game).vtable[0x20])(&DStack_c);
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
            g_UIScaleX = g_WindowWidth / 640.0;
            g_UIScaleY = g_WindowHeight / 448.0;
            UpdateTextureContentDirectories();
            Game::PreGameInitialize(nullptr);
            return 1;
        }
        g_UIScaleX = g_WindowWidth / 1280.0;
        g_UIScaleY = g_WindowHeight / 720.0;
    }
    else {
        // (*(this->Game).vtable[0x20])(param_1);
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
