#include "x360_vertex_shader_manager.hpp"
#include "x360_vertex_attribute_format_string_table.hpp"
#include "x360_video_card.hpp"
#include "x360_render_target.hpp"

// OFFSET: 0x00411050, STATUS: COMPLETE
X360VertexShaderManager::X360VertexShaderManager() {
	current_shader = nullptr;
	current_vertex_format_index = -1;
	g_D3DDevice9->SetVertexShader(nullptr);
	g_D3DDevice9->SetVertexDeclaration(nullptr);
	is_fullscreen_effect = 0;
}

// OFFSET: INLINE, STATUS: COMPLETE
X360VertexShaderManager::~X360VertexShaderManager() {
	Reset();
}

// OFFSET: 0x00411090, STATUS: COMPLETE
void X360VertexShaderManager::Reset() {
	current_shader = nullptr;
	current_vertex_format_index = -1;
}

// OFFSET: 0x00411190, STATUS: COMPLETE
void X360VertexShaderManager::SetVertexFormatIndex(int vertex_format_index) {
	if (vertex_format_index != current_vertex_format_index) {
		g_D3DDevice9->SetVertexDeclaration(reinterpret_cast<IDirect3DVertexDeclaration9*>(g_lpVertexAttributeFormatStringTable->GetUserDataAt(vertex_format_index)));
		current_vertex_format_index = vertex_format_index;
	}
}

// OFFSET: INLINE, STATUS: COMPLETE
void X360VertexShaderManager::SetIsFullscreenEffect(int _is_fullscreen_effect) {
	is_fullscreen_effect = _is_fullscreen_effect;
}

// OFFSET: 0x004110a0, STATUS: COMPLETE
void X360VertexShaderManager::SetVertexShader(X360VertexShader* shader, int index) {
    if (is_fullscreen_effect == 0) {
        if (shader == nullptr) {
            g_D3DDevice9->SetVertexShader(nullptr);
            current_shader = nullptr;
        }
        else if (current_shader != shader->Get(index)) {
            g_D3DDevice9->SetVertexShader(shader->Get(index));
            return;
        }
        return;
    }
    if (strstr(shader->GetName(), "skinned") != nullptr) {
        g_D3DDevice9->SetVertexShader(g_RenderTarget->shadow_skinned_vertex->Get(0));
        return;
    }
    if (strstr(shader->GetName(), "puppet") != nullptr) {
        g_D3DDevice9->SetVertexShader(g_RenderTarget->shadow_puppet_vertex->Get(0));
        return;
    }
    g_D3DDevice9->SetVertexShader(g_RenderTarget->shadow_vertex->Get(0));
    return;
}
