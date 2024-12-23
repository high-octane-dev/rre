#include "gfx/x360_video_card.hpp"
#include "gfx/emotion_engine.hpp"
#include "gfx/d3d_state_manager.hpp"
#include "cars_game.hpp"
#include "globals.hpp"

X360VideoCard* g_VideoCard = nullptr;
IDirect3DDevice9* g_D3DDevice9 = nullptr;

// OFFSET: 0x00414ea0
X360VideoCard::X360VideoCard() : VideoCard()
{
    unused = nullptr;
    g_D3DDevice9 = nullptr;
    lpD3DStateManager = nullptr;
    flags = flags & 0xfc;
    current_frame = 0;
    unk_refresh_rate_param = 2;
    enable_fullscreen_effects = true;
}

// OFFSET: 0x00673260
void free_device_manager() {
    delete lpD3DDeviceManager;
}

// OFFSET: 0x00414fc0
void X360VideoCard::Create()
{
    lpD3DDeviceManager = new EE::D3DDeviceManager(lpCarsGame->d3d9);
    lpD3DDeviceManager->behavior_flag_1 = lpD3DDeviceManager->behavior_flag_1 | 4;
    lpD3DDeviceManager->windowed = g_IsWindowed;
    lpD3DDeviceManager->back_buffer_width = g_WindowWidth;
    lpD3DDeviceManager->back_buffer_height = g_WindowHeight;
    atexit(free_device_manager);
    if (lpD3DDeviceManager->Create() != 0) {
        if (lpD3DDeviceManager->CreateAdapters(lpCarsGame->hwnd) != 0) {
            g_D3DDevice9 = lpD3DDeviceManager->device;
            g_D3DDevice9->AddRef();
            capabilites = lpD3DDeviceManager->adapter_list.data[lpD3DDeviceManager->adapter_index]->capabilities;
            g_TargetFrameRate = 60.0;
            lpD3DStateManager = new D3DStateManager();
        }
    }
}

// OFFSET: 0x00415440
bool X360VideoCard::IsFXEnabled()
{
    auto pixel_shader_version_satisfied = 0xffff01ff < (lpD3DDeviceManager->adapter_list.data[lpD3DDeviceManager->adapter_index]->capabilities).PixelShaderVersion;
    auto vertex_shader_version_satisfied = 0xfffe01ff < (lpD3DDeviceManager->adapter_list.data[lpD3DDeviceManager->adapter_index]->capabilities).VertexShaderVersion;
    return enable_fullscreen_effects && pixel_shader_version_satisfied && vertex_shader_version_satisfied;
}

// OFFSET: 0x00414f10
X360VideoCard::~X360VideoCard()
{
    if (g_D3DDevice9 != nullptr) {
        g_D3DDevice9->Release();
    }

    if (lpD3DStateManager != nullptr) {
        delete lpD3DStateManager;
    }

    lpD3DDeviceManager->ReleaseResources();
    if (unused != nullptr) {
        unused->Release();
    }
}

// OFFSET: 0x004153e0
void X360VideoCard::FreeCursor()
{
    // UNIMPLEMENTED
}

// OFFSET: 0x00414e00
int X360VideoCard::ResetStateManager()
{
    lpD3DStateManager->Reset();
    return 1;
}

// OFFSET: 0x00414e20
int X360VideoCard::DisplayToScreen(int inc_frame_count)
{
    g_D3DDevice9->Present(0, 0, 0, 0);
    for (std::size_t i = 0; i < 16; i++) {
        g_D3DDevice9->SetTexture(i, nullptr);
    }
    if (should_inc_frame_count == 0 && inc_frame_count == 0) {
        current_frame = current_frame + 1;
    }
    Sleep(0);
    return 1;
}

// OFFSET: INLINE
void X360VideoCard::Unk4()
{
}

// OFFSET: INLINE
void X360VideoCard::Unk5()
{
}

// OFFSET: INLINE
void X360VideoCard::Unk6()
{
}

// OFFSET: INLINE
void X360VideoCard::Unk7()
{
}

// OFFSET: INLINE
void X360VideoCard::Unk8()
{
}
