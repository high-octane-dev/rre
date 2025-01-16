#include "x360_full_screen_texture_render.hpp"
#include "panic.hpp"

// OFFSET: INLINE
X360FullScreenTextureRender::X360FullScreenTextureRender() {
    textures[0] = nullptr;
}

// OFFSET: INLINE
X360FullScreenTextureRender::~X360FullScreenTextureRender() {
    if (textures[0] != nullptr) {
        textures[0]->Release();
        textures[0] = nullptr;
    }
}

// OFFSET: 0x004134a0
void X360FullScreenTextureRender::FUN_004134a0(int index) {
    if (index != 0) {
        panic("Index is not 0!");
    }
    if (textures[index] != nullptr) {
        // g_RenderTarget->vt[5](0, textures[index], 0, 0, 1, 1, -1);
    }
}

// OFFSET: 0x00413460
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