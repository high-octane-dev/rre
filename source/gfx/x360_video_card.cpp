#include "gfx/x360_video_card.hpp"
#include "gfx/emotion_engine.hpp"
#include "gfx/d3d_state_manager.hpp"
#include "cars_game.hpp"
#include "globals.hpp"
#include "cursor.hpp"

X360VideoCard* g_VideoCard = nullptr;
IDirect3DDevice9* g_D3DDevice9 = nullptr;

// OFFSET: 0x00414ea0, STATUS: COMPLETE
X360VideoCard::X360VideoCard() : VideoCard()
{
    unused = nullptr;
    g_D3DDevice9 = nullptr;
    g_lpD3DStateManager = nullptr;
    flags = flags & 0xfc;
    current_frame = 0;
    unk_refresh_rate_param = 2;
    enable_fullscreen_effects = true;
}

// OFFSET: 0x00673260, STATUS: COMPLETE
void free_device_manager() {
    if (g_lpD3DDeviceManager != nullptr) {
        g_lpD3DDeviceManager->Release();
        g_lpD3DDeviceManager = nullptr;
    }
}

// OFFSET: 0x00414fc0, STATUS: COMPLETE
void X360VideoCard::Create() {
    g_lpD3DDeviceManager = new EE::D3DDeviceManager(lpCarsGame->d3d9);
    g_lpD3DDeviceManager->AddRef();
    g_lpD3DDeviceManager->behavior_flag_1 = g_lpD3DDeviceManager->behavior_flag_1 | 4;
    g_lpD3DDeviceManager->windowed = g_IsWindowed;
    g_lpD3DDeviceManager->back_buffer_width = g_WindowWidth;
    g_lpD3DDeviceManager->back_buffer_height = g_WindowHeight;
    // This is an innacuracy. In the original game, this atexit call occurs during static initialization.
    atexit(free_device_manager);

    if (g_lpD3DDeviceManager->Create() != 0) {
        if (g_lpD3DDeviceManager->CreateAdapters(lpCarsGame->hwnd) != 0) {
            g_D3DDevice9 = g_lpD3DDeviceManager->device;
            g_D3DDevice9->AddRef();
            capabilities = g_lpD3DDeviceManager->adapter_list.data[g_lpD3DDeviceManager->adapter_index]->capabilities;
            g_TargetFrameRate = 60.0;
            g_lpD3DStateManager = new D3DStateManager();
        }
    }
}

// OFFSET: 0x00415440, STATUS: COMPLETE
bool X360VideoCard::IsFXEnabled() {
    auto pixel_shader_version_satisfied = 0xffff01ff < (g_lpD3DDeviceManager->adapter_list.data[g_lpD3DDeviceManager->adapter_index]->capabilities).PixelShaderVersion;
    auto vertex_shader_version_satisfied = 0xfffe01ff < (g_lpD3DDeviceManager->adapter_list.data[g_lpD3DDeviceManager->adapter_index]->capabilities).VertexShaderVersion;
    return enable_fullscreen_effects && pixel_shader_version_satisfied && vertex_shader_version_satisfied;
}

// OFFSET: 0x00414f10, STATUS: COMPLETE
X360VideoCard::~X360VideoCard() {
    if (g_D3DDevice9 != nullptr) {
        g_D3DDevice9->Release();
    }

    if (g_lpD3DStateManager != nullptr) {
        delete g_lpD3DStateManager;
        g_lpD3DDeviceManager = nullptr;
    }

    if (g_lpD3DDeviceManager != nullptr) {
        g_lpD3DDeviceManager->ReleaseResources();
    }

    if (unused != nullptr) {
        unused->Release();
    }
}

// OFFSET: 0x004153e0, STATUS: COMPLETE
void X360VideoCard::FreeCursor() {
    if (g_lpCursor != nullptr) {
        delete g_lpCursor;
        g_lpCursor = nullptr;
    }
}

// OFFSET: 0x00414e00, STATUS: COMPLETE
int X360VideoCard::ResetStateManager() {
    g_lpD3DStateManager->Reset();
    return 1;
}

// OFFSET: 0x00414e20, STATUS: COMPLETE
int X360VideoCard::DisplayToScreen(int inc_frame_count) {
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

// OFFSET: INLINE, STATUS: COMPLETE
void X360VideoCard::Unk4() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360VideoCard::Unk5() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360VideoCard::Unk6() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360VideoCard::Unk7() {
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360VideoCard::Unk8() {
}
