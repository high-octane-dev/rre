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

// OFFSET: 0x004052f0, STATUS: WIP
X360LoadingIcon::X360LoadingIcon() : LoadingIcon() {
    loading_screen = nullptr;
    thread_handle = nullptr;
    duplicate_proc_handle = nullptr;
    present_frame_lock = nullptr;
    footer_bar = nullptr;
    font_texture = nullptr;
    text_icons[0] = nullptr;
    text_icons[1] = nullptr;
    icon_vertex = nullptr;
    icon_pixel = nullptr;
}

// OFFSET: 0x00405fe0, STATUS: COMPLETE
X360LoadingIcon::~X360LoadingIcon() {
    if (enabled != 0) {
        Disable();
    }
    ClearLoadingTips();
}

// OFFSET: 0x00405370, STATUS: COMPLETE
void X360LoadingIcon::Enable() {
    if (loading_screen == nullptr) {
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

// OFFSET: 0x00405d30, STATUS: WIP
void X360LoadingIcon::Disable() {
    if (enabled != 0) {
        SetEvent(present_frame_lock);
        WaitForSingleObject(thread_handle, INFINITE);
        CloseHandle(thread_handle);
        CloseHandle(present_frame_lock);
        DeleteCriticalSection(&lock);
        if (loading_screen != nullptr) {
            loading_screen->Release();
            loading_screen = nullptr;
        }
        if (duplicate_proc_handle != nullptr) {
            CloseHandle(duplicate_proc_handle);
        }
        thread_handle = nullptr;
        duplicate_proc_handle = nullptr;
        present_frame_lock = nullptr;
        LoadingIcon::Disable();
        if (footer_bar != nullptr) {
            footer_bar->Release();
            footer_bar = nullptr;
        }
        if (font_texture != nullptr) {
            font_texture->Release();
            font_texture = nullptr;
        }
        /*
        if (text_icons[0] != nullptr) {
            delete text_icons[0];
        }
        if (text_icons[1] != nullptr) {
            delete text_icons[1];
        }
        */
        if (icon_vertex != nullptr) {
            delete icon_vertex;
            icon_vertex = nullptr;
        }
        if (icon_pixel != nullptr) {
            delete icon_pixel;
            icon_pixel = nullptr;
        }
        return;
    }
    return;
}

// OFFSET: 0x004050c0, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationLocation(char* name, float x, float y) {
    if (enabled != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationLocation(name, x, y);
    if (enabled != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x00405110, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationSize(char* name, float size_x, float size_y) {
    if (enabled != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationSize(name, size_x, size_y);
    if (enabled != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x00405160, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationOpacity(char* name, float opacity) {
    if (enabled != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationOpacity(name, opacity);
    if (enabled != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x004051a0, STATUS: COMPLETE
void X360LoadingIcon::SetAnimationColor(char* name, unsigned int color) {
    if (enabled != 0) {
        EnterCriticalSection(&lock);
    }
    LoadingIcon::SetAnimationColor(name, color);
    if (enabled != 0) {
        LeaveCriticalSection(&lock);
    }
}

// OFFSET: 0x00405270, STATUS: COMPLETE
void X360LoadingIcon::Update() {
    EnterCriticalSection(&lock);
    if (loading_screen != nullptr) {
        g_RenderTarget->Blt(nullptr, loading_screen, nullptr, 0, 1, 1, 0xFFFFFFFF);
    }
    LoadingIcon::Update();
    for (std::size_t i = 0; i < animation_count; i++) {
        UpdateAnimationState(&animations[i]);
        RenderAnimation(&animations[i]);
    }
    LeaveCriticalSection(&lock);
}

// OFFSET: 0x004051e0, STATUS: COMPLETE
void X360LoadingIcon::RenderIcon(LoadingIconAnim* anim, LoadingImageIcon* icon, float x, float y, float width, float height, unsigned int color) {
    FRECT rect{
        g_UIScaleX * x,
        g_UIScaleY * y,
        (x + width) * g_UIScaleX,
        g_UIScaleY * (y + height),
    };
    g_RenderTarget->Blt(&rect, icon->texture_map, nullptr, 0, 1, anim->enable_alpha_blending, color);
}

// OFFSET: 0x00404d50, STATUS: COMPLETE
int X360LoadingIcon::Load(ParameterBlock* file) {
    if (LoadingIcon::Load(file) != 0) {
        for (std::size_t i = 0; i < frame_count; i++) {
            char texture_name[260]{};
            RSStringUtil::Ssnprintf(texture_name, sizeof(texture_name), "frame%02d.xnt", i + 1);
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
    if (enabled != 0) {
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

// OFFSET: 0x005509e0, STATUS: COMPLETE
void X360LoadingIcon::RenderAnimation(LoadingIconAnim* anim) {
    int frame = static_cast<int>(std::nearbyint(std::roundf(anim->current_frame)));
    unsigned int color = 0xFFFFFFFF;
    if (anim->color_set) {
        color = anim->color;
    }
    RenderIcon(anim, &frames[frame], anim->location_x - anim->size_x * 0.5f, anim->location_y - anim->size_y * 0.5f, anim->size_x, anim->size_y, color);
}

// OFFSET: 0x00405550, STATUS: TODO
void X360LoadingIcon::SetRenderStates()	{
}

// OFFSET: 0x005505f0, STATUS: COMPLETE
void X360LoadingIcon::UpdateAnimationState(LoadingIconAnim* anim) {
    if (anim->flags == 0x7F) {
        return;
    }

    if ((anim->flags & 0x40) != 0) {
        if (timeGetTime() - enable_timestamp < anim->start_time_ms) {
            return;
        }
        anim->flags &= 0xFFBF;
    }

    if (!(anim->flags & 1)) {
        if (!anim->end_frame_lt_start) {
            anim->current_frame += anim->animate_time;
            if ((anim->end_frame < anim->current_frame) != (anim->end_frame == anim->current_frame)) {
                if (!anim->loop_anim) {
                    anim->flags |= 1;
                    anim->current_frame = anim->end_frame;
                }
                else if (!anim->reversable_anim) {
                    anim->current_frame -= (anim->end_frame - anim->start_frame);
                }
                else {
                    anim->end_frame_lt_start = true;
                    anim->current_frame = anim->end_frame;
                }
            }
        }
        else {
            anim->current_frame -= anim->animate_time;
            if (anim->current_frame <= anim->start_frame) {
                if (!anim->loop_anim) {
                    anim->flags |= 1;
                    anim->current_frame = anim->start_frame;
                }
                else if (!anim->reversable_anim) {
                    anim->current_frame = anim->end_frame;
                }
                else {
                    anim->end_frame_lt_start = false;
                    anim->current_frame = anim->start_frame;
                }
            }
        }
    }

    if (!(anim->flags & 2)) {
        if (!anim->end_pos_x_lt_start) {
            anim->location_x += anim->move_increment_x;
            if ((anim->end_pos_x < anim->location_x) != (anim->end_pos_x == anim->location_x)) {
                if (!anim->loop_movement) {
                    anim->flags |= 2;
                }
                else if (!anim->reversable_movement) {
                    anim->location_x = anim->start_pos_x;
                }
                else {
                    anim->end_pos_x_lt_start = true;
                    anim->location_x = anim->end_pos_x;
                }
            }
        }
        else {
            anim->location_x -= anim->move_increment_x;
            if (anim->location_x <= anim->start_pos_x) {
                if (!anim->loop_movement) {
                    anim->flags |= 2;
                    anim->location_x = anim->start_pos_x;
                }
                else if (!anim->reversable_movement) {
                    anim->location_x = anim->end_pos_x;
                }
                else {
                    anim->end_pos_x_lt_start = false;
                    anim->location_x = anim->start_pos_x;
                }
            }
        }
    }

    if (!(anim->flags & 4)) {
        anim->location_y += anim->move_increment_y;
        if ((anim->end_pos_y_lt_start && anim->location_y <= anim->end_pos_y) ||
            (!anim->end_pos_y_lt_start && anim->location_y >= anim->end_pos_y)) {
            if (!anim->loop_movement) {
                anim->flags |= 4;
            }
            else if (!anim->reversable_movement) {
                anim->location_y = anim->start_pos_y;
            }
            else {
                anim->end_pos_y_lt_start = !anim->end_pos_y_lt_start;
                anim->location_y = anim->end_pos_y;
            }
        }
    }

    if (!(anim->flags & 0x20)) {
        anim->opacity += anim->opacity_time;
        if ((anim->end_opacity_lt_start && anim->opacity <= anim->start_opacity) ||
            (!anim->end_opacity_lt_start && anim->opacity >= anim->end_opacity)) {
            if (!anim->loop_opacity) {
                anim->flags |= 0x20;
            }
            else if (!anim->reversable_opacity) {
                anim->opacity = anim->start_opacity;
            }
            else {
                anim->end_opacity_lt_start = !anim->end_opacity_lt_start;
                anim->opacity = anim->end_opacity;
            }
        }
    }
}
