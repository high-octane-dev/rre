#include "x360_full_screen_texture_render.hpp"
#include "panic.hpp"
#include "x360_render_target.hpp"

// OFFSET: INLINE, STATUS: COMPLETE
X360FullScreenTextureRender::X360FullScreenTextureRender() {
    textures[0] = nullptr;
}

// OFFSET: INLINE, STATUS: COMPLETE
X360FullScreenTextureRender::~X360FullScreenTextureRender() {
    if (textures[0] != nullptr) {
        textures[0]->Release();
        textures[0] = nullptr;
    }
}

// OFFSET: 0x004134a0, STATUS: COMPLETE
void X360FullScreenTextureRender::Draw(int index) {
    if (index != 0) {
        panic("Index is not 0!");
    }
    if (textures[index] != nullptr) {
        g_RenderTarget->Blt(nullptr, textures[index], nullptr, 0, 1, 1, 0xFFFFFFFF);
    }
}

// OFFSET: 0x00413460, STATUS: COMPLETE
void X360FullScreenTextureRender::SetTexture(int index, TextureMap* texture) {
    if (index != 0) {
        panic("Index is not 0!");
    }

    if (texture != nullptr) {
        texture->AddRef();
    }
    if (textures[index] != nullptr) {
        textures[index]->Release();
    }
    textures[index] = texture;
}

X360FullScreenTextureRender g_FullscreenRender{};