#include "x360_pixel_shader_manager.hpp"
#include "x360_video_card.hpp"
#include "x360_render_target.hpp"

// OFFSET: 0x004114f0, STATUS: COMPLETE
X360PixelShaderManager::X360PixelShaderManager() {
	g_D3DDevice9->SetPixelShader(nullptr);
	current_shader = nullptr;
	is_fullscreen_effect = 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
X360PixelShaderManager::~X360PixelShaderManager() {
	Reset();
}

// OFFSET: 0x00411520, STATUS: COMPLETE
void X360PixelShaderManager::Reset() {
	current_shader = nullptr;
}

// OFFSET: 0x004115a0, STATUS: COMPLETE
void X360PixelShaderManager::SetIsFullscreenEffect(int _is_fullscreen_effect) {
	is_fullscreen_effect = _is_fullscreen_effect;
}

// OFFSET: 0x00411530, STATUS: COMPLETE
void X360PixelShaderManager::SetPixelShader(X360PixelShader* shader) {
	if (is_fullscreen_effect != 0) {
		g_D3DDevice9->SetPixelShader(g_RenderTarget->shadow_pixel->Get());
		current_shader = g_RenderTarget->shadow_pixel->Get();
		return;
	}
	IDirect3DPixelShader9* inner = nullptr;
	if (shader != nullptr) {
		inner = shader->Get();
	}
	if (current_shader != inner) {
		g_D3DDevice9->SetPixelShader(inner);
		current_shader = inner;
	}
	return;
}
