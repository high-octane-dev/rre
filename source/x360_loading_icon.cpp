#include "x360_loading_icon.hpp"
#include "cars_game.hpp"
#include "gfx/renderer.hpp"
#include "gfx/x360_tex_map.hpp"
#include "util/rsstring_util.hpp"
#include <gfx\x360_render_target.hpp>

// OFFSET: 0x00404c40, STATUS: COMPLETE
static unsigned int __stdcall LoadingIconThreadEntryPoint(X360LoadingIcon* _this) {
    DWORD result_code = WaitForSingleObject(_this->present_frame_lock, 17);
    while (result_code != 0) {
        if (result_code == WAIT_TIMEOUT) {
            int index = Renderer::PrepareNextRenderFrame();
            Renderer::g_RenderFrameData[index].fullscreen_render_pass = static_cast<X360FullScreenRenderPass*>(_this);
            Renderer::CompleteNextRenderFrame();
            static_cast<CarsGame*>(lpGame)->PresentFrame(0);
            Sleep(0);
        }
        result_code = WaitForSingleObject(_this->present_frame_lock, 17);
    }
    ExitThread(0);
}

// OFFSET: 0x00405450, STATUS: TODO
static void ClearLoadingTips() {
}

// OFFSET: 0x00405fe0, STATUS: COMPLETE
X360LoadingIcon::~X360LoadingIcon() {
    if (should_lock != 0) {
        Disable();
    }
    ClearLoadingTips();
}

// OFFSET: 0x00405370, STATUS: COMPLETE
void X360LoadingIcon::Enable() {
    if (loading_screen == NULL) {
        char loading_screen_name[260]{};
        // This is an intentional innacuracy to fix load_logo erroneously always being used (this bug is in the original game).
        RSStringUtil::Ssnprintf(loading_screen_name, sizeof(loading_screen_name), "%s\\%s", g_UILocalizedTextureContentDirectory, lpCarsGame->loading_screen_name);
        loading_screen = static_cast<X360TextureMap*>(X360TexMap::GetTextureMapFromResourceName(loading_screen_name, 888, 0));
    }
    InitFooterResources();
    present_frame_lock = CreateEventA(nullptr, FALSE, FALSE, "END EVENT");
    InitializeCriticalSection(&lock);
    if (duplicate_proc_handle != nullptr) {
        CloseHandle(duplicate_proc_handle);
    }
    DuplicateHandle(nullptr, GetCurrentThread(), nullptr, &duplicate_proc_handle, 0, FALSE, 0);
    thread_handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadingIconThreadEntryPoint), this, 0, nullptr);
    LoadingIcon::Enable();
}

// OFFSET: 0x00405d30, STATUS: TODO
void X360LoadingIcon::Disable() {
}

// OFFSET: 0x004050c0, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationLocation(char* name, float x, float y) {
    if (should_lock != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationLocation(name, x, y);
    if (should_lock != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x00405110, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationSize(char* name, float size_x, float size_y) {
    if (should_lock != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationSize(name, size_x, size_y);
    if (should_lock != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x00405160, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationOpacity(char* name, float opacity) {
    if (should_lock != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationOpacity(name, opacity);
    if (should_lock != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x004051a0, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationColor(char* name, unsigned int color) {
    if (should_lock != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationColor(name, color);
    if (should_lock != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x00405270, STATUS: COMPLETE
void X360LoadingIcon::Update() {
    EnterCriticalSection(&this->lock);
    if (loading_screen != nullptr) {
        g_RenderTarget->Blt(nullptr, loading_screen, nullptr, 0, 1, 1, 0xFFFFFFFF);
    }
    LoadingIcon::Update();
    for (std::size_t i = 0; i < animation_count; i++) {
        UpdateAnimationState(&animations[i]);
        RenderAnimation(&animations[i]);
    }
    LeaveCriticalSection(&this->lock);
}

// OFFSET: 0x004051e0, STATUS: TODO
void X360LoadingIcon::RenderIcon(LoadingIconAnim*, LoadingImageIcon*, float, float, float, float, unsigned int) {
}

// OFFSET: 0x00404d50, STATUS: WIP
int X360LoadingIcon::Load(ParameterBlock* file) {
    if (LoadingIcon::Load(file) != 0) {
        for (std::size_t i = 0; i < frame_count; i++) {
            char texture_name[260]{};
            // FIXME: make texture loader change extension to match original game, and change it here to .xnt
            RSStringUtil::Ssnprintf(texture_name, sizeof(texture_name), "frame%02d.dds", i + 1);
            frames[i].texture_map = X360TexMap::GetTextureMapFromResourceName(texture_name, 1555, 0);
            D3DSURFACE_DESC desc{};
            static_cast<X360TextureMap*>(frames[i].texture_map)->texture->GetLevelDesc(0, &desc);
            frames[i].format = desc.Format;
            frames[i].width = desc.Width;
            frames[i].height = desc.Height;
        }
        return 1;
    }
	return 0;
}

// OFFSET: 0x00405d10, STATUS: COMPLETE
void X360LoadingIcon::Draw(int index) {
    if (should_lock != 0) {
        Update();
        SetRenderStates();
    }
}

// OFFSET: 0x00405db0, STATUS: TODO
void X360LoadingIcon::InitLoadingTips(unsigned int, char*) {
}

// OFFSET: 0x00404e30, STATUS: TODO
void X360LoadingIcon::InitFooterResources() {
}

// OFFSET: 0x005509e0, STATUS: TODO
void X360LoadingIcon::RenderAnimation(LoadingIconAnim*) {
}

// OFFSET: 0x00405550, STATUS: TODO
void X360LoadingIcon::SetRenderStates()	{
}

// OFFSET: 0x005505f0, STATUS: TODO
void X360LoadingIcon::UpdateAnimationState(LoadingIconAnim*) {
}
